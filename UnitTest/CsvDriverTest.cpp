#include "pch.h"
#include "CppUnitTest.h"
#include "../LimeSteel/CsvDriver.h"
#include "../LimeSteel/CsvDriver.cpp"
#include "../LimeSteel/DataSeeder.h"
#include "../LimeSteel/DataSeeder.cpp"
#include "../LimeSteel/Person.h"
#include <string>
#include "vector"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

//using std::vector;

// Path to the directory where the tests will be executed, USE WITH CAUTION
constexpr auto TESTS_PATH = "C:\\Users\\alex2\\LimeSteel\\Tests";
constexpr auto TESTS_FILE = "test.csv";

namespace UnitTest
{
	TEST_CLASS(CsvDriverTest)
	{
	public:
		CsvDriver* csv = nullptr;
		GuidFactory* guidFactory = nullptr;
		DirectoryService* directory = nullptr;
		DataSeeder* dataSeeder = nullptr;

		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			system(("rmdir /s /q " + string(TESTS_PATH)).c_str());
		}

		TEST_METHOD_INITIALIZE(Setup)
		{
			system(("mkdir " + string(TESTS_PATH)).c_str());
			// Instantiate the objects
			this->directory = new DirectoryService(TESTS_PATH);
			this->guidFactory = new GuidFactory();
			this->dataSeeder = new DataSeeder();
			this->csv = new CsvDriver(directory, guidFactory);
		}

		TEST_METHOD_CLEANUP(Teardown)
		{
			system(("rmdir /s /q " + string(TESTS_PATH)).c_str());
			delete this->directory;
			delete this->guidFactory;
			delete this->dataSeeder;
			delete this->csv;
		}

		TEST_METHOD(insert_generate_guid)
		{
			// Arrange
			this->directory->create_directory();
			this->directory->create_file(TESTS_FILE);
			auto row = dataSeeder->get_random_row(3);

			// Act
			auto created = this->csv->insert(TESTS_FILE, row);

			// Assert
			auto columnCount = row.size() + 1 == created.size();
			auto firstColumn = created[0];
			auto isGuid = this->guidFactory->is_guid(firstColumn);
			Assert::IsTrue(isGuid && columnCount);
		}

		TEST_METHOD(seed)
		{
			// Arrange
			auto rows = 5;
			auto filename = "seed.csv";
			this->directory->create_directory();
			this->directory->create_file(filename);

			// Act
			for (size_t i = 0; i < rows; i++)
			{
				auto row = dataSeeder->get_random_row(3);
				this->csv->insert(filename, row);
				Logger::WriteMessage(L"Inserted row");
			}

			// Assert
			auto table = this->csv->select(filename);
			auto rowCount = table.size() == rows;
			auto message = L"Expected " + std::to_wstring(rows) + L" rows, got " + std::to_wstring(table.size());
			Assert::IsTrue(rowCount, message.c_str());
		}

		TEST_METHOD(cast_line_to_row)
		{
			// Arrange
			string content = "1,2,3";

			// Act
			auto row = this->csv->cast_line_to_row(content);

			// Assert
			auto first = row[0] == "1";
			auto second = row[1] == "2";
			auto third = row[2] == "3";
			Assert::IsTrue(first && second && third);
		}

		TEST_METHOD(cast_row_to_line)
		{
			// Arrange
			vector<string> row = { "1", "2", "3" };

			// Act
			auto line = this->csv->cast_row_to_line(row);

			// Assert
			Assert::AreEqual(line.c_str(), "1,2,3");
		}

		TEST_METHOD(insert_models)
		{
			// Arrange
			vector<Person*> people;
			people.push_back(new Person("Alex", "Smith", "CF", "Ciudad", "123456", 30));
			people.push_back(new Person("John", "Doe", "CF", "Ciudad", "654321", 40));
			people.push_back(new Person("Jane", "Doe", "CF", "Ciudad", "987654", 50));

			// Act
			for (auto person : people)
			{
				this->csv->insert_model(TESTS_FILE, person);
			}

			// Assert
			auto table = this->csv->select(TESTS_FILE);
			auto rowCount = table.size() == people.size();
			auto message = L"Expected " + std::to_wstring(people.size()) + L" rows, got " + std::to_wstring(table.size());
			Assert::IsTrue(rowCount, message.c_str());

			auto names_message = L"Names do not match";
			auto first = table[0][1] == "Alex";
			auto second = table[1][1] == "John";
			auto third = table[2][1] == "Jane";
			Assert::IsTrue(first && second && third, names_message);

			auto guids_message = L"Guids do not match";
			auto first_guid = this->guidFactory->is_guid(people[0]->guid);
			auto second_guid = this->guidFactory->is_guid(people[1]->guid);
			auto third_guid = this->guidFactory->is_guid(people[2]->guid);

			Assert::IsTrue(first_guid && second_guid && third_guid, guids_message);
		}
	};
}
