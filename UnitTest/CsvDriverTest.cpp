#include "pch.h"
#include "CppUnitTest.h"
#include "../LimeSteel/CsvDriver.h"
#include "../LimeSteel/CsvDriver.cpp"
#include "../LimeSteel/DataSeeder.h"
#include "../LimeSteel/DataSeeder.cpp"
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
			//system(("mkdir " + string(TESTS_PATH)).c_str());
		}

		TEST_METHOD_INITIALIZE(Setup)
		{
			// Instantiate the objects
			this->directory = new DirectoryService(TESTS_PATH);
			this->guidFactory = new GuidFactory();
			this->dataSeeder = new DataSeeder();
			this->csv = new CsvDriver(directory, guidFactory);
		}

		TEST_METHOD_CLEANUP(Teardown)
		{
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
	};
}
