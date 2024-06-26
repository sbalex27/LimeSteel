#include "pch.h"
#include "CppUnitTest.h"
#include "../LimeSteel/PersonRepository.h"
#include "../LimeSteel/DataSeeder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(PersonTest)
	{
	public:
		PersonRepository* table = nullptr;
		GuidFactory* guidFactory = nullptr;
		CsvDriver* csv = nullptr;
		DataSeeder* seeder = nullptr;

		TEST_METHOD_INITIALIZE(Setup)
		{
			this->seeder = new DataSeeder();
			auto directory = new DirectoryService(TEST_DIRECTORY);
			guidFactory = new GuidFactory();
			this->csv = new CsvDriver(directory, guidFactory);
			this->table = new PersonRepository(csv);
			this->table->refresh();
		}

		TEST_METHOD_CLEANUP(Teardown)
		{
			delete this->table;
		}

		TEST_METHOD(SerializeToRow)
		{
			// Arrange
			auto person = Person();
			person.name = "John";
			person.lastname = "Doe";
			person.nit = "CF";
			person.address = "Ciudad";
			person.phone = "123456789";
			person.age = 30;
			auto expected = Row{ "John", "Doe", "CF", "Ciudad", "123456789", "30" };

			// Act
			auto actual = person.to_row();

			// Assert
			Assert::IsTrue(vector_equals(expected, actual));
		}

		TEST_METHOD(DeserializeFromRow)
		{
			// Arrange
			auto guid = guidFactory->create();
			auto row = Row{ guid, "John", "Doe", "CF", "Ciudad", "123456789", "30" };
			auto expected = Person();
			expected.guid = guid;
			expected.name = "John";
			expected.lastname = "Doe";
			expected.nit = "CF";
			expected.address = "Ciudad";
			expected.phone = "123456789";
			expected.age = 30;

			// Act
			auto actual = Person(row);

			// Assert
			Assert::AreEqual(expected.guid, actual.guid);
			Assert::AreEqual(expected.name, actual.name);
			Assert::AreEqual(expected.lastname, actual.lastname);
			Assert::AreEqual(expected.nit, actual.nit);
			Assert::AreEqual(expected.address, actual.address);
			Assert::AreEqual(expected.phone, actual.phone);
			Assert::AreEqual(expected.age, actual.age);
		}

		TEST_METHOD(SerializeWithGuid) {
			// Arrange
			auto guid = guidFactory->create();
			auto person = Person(guid, "John", "Doe", "CF", "Ciudad", "123456789", 30);
			auto expected = Row{ guid, "John", "Doe", "CF", "Ciudad", "123456789", "30" };

			// Act
			auto actual = person.to_row();

			// Assert
			Assert::IsTrue(vector_equals(expected, actual));
		}

		TEST_METHOD(Insert)
		{
			// Arrange
			auto person = new Person();
			person->name = "John";
			person->lastname = "Doe";
			person->phone = "123456789";
			person->age = 30;

			// Act
			auto guid = table->insert(person);

			// Assert
			auto actual = table->find(guid);
			Assert::AreEqual(person->name, actual.name);
			Assert::AreEqual(person->lastname, actual.lastname);
			Assert::AreEqual(person->phone, actual.phone);
			Assert::AreEqual(person->age, actual.age);
		}

		TEST_METHOD(Update)
		{
			// Arrange
			auto original = new Person();
			original->name = "John";
			original->lastname = "Doe";
			original->phone = "123456789";
			original->age = 30;
			auto guid = table->insert(original);

			// Act
			original->name = "Sergio";
			original->lastname = "Batres";
			original->phone = "41925033";
			original->age = 24;
			table->update(original);

			// Assert
			auto actual = table->find(guid);
			Assert::AreEqual(original->name, actual.name);
			Assert::AreEqual(original->lastname, actual.lastname);
			Assert::AreEqual(original->phone, actual.phone);
			Assert::AreEqual(original->age, actual.age);
		}

		TEST_METHOD(SeedBeforeUpdate)
		{
			// Arrange
			auto seed = this->seeder->get_random_table(20, 6);
			auto seeded = this->csv->insert(PEOPLE_TABLE, seed);
			auto original = new Person();
			original->name = "John";
			original->lastname = "Doe";
			original->nit = "CF";
			original->address = "Ciudad";
			original->phone = "123456789";
			original->age = 30;
			auto guid = table->insert(original);

			// Act
			original->name = "Any";
			original->lastname = "Escobedo";
			original->nit = "104903805";
			original->address = "City";
			original->phone = "55667788";
			original->age = 50;
			table->update(original);

			// Assert
			auto actual = table->find(guid);
			Assert::AreEqual(original->name, actual.name);
			Assert::AreEqual(original->lastname, actual.lastname);
			Assert::AreEqual(original->nit, actual.nit);
			Assert::AreEqual(original->address, actual.address);
			Assert::AreEqual(original->phone, actual.phone);
			Assert::AreEqual(original->age, actual.age);
		}

		TEST_METHOD(InsertFullName)
		{
			// Arrange
			auto person = new Person();
			person->name = "Sergio Alexander";
			person->lastname = "Batres Escobedo";
			person->phone = "41925033";
			person->age = 24;

			// Act
			auto guid = table->insert(person);

			// Assert
			auto actual = table->find(guid);
			Assert::AreEqual(person->get_full_name(), actual.get_full_name());
		}
	};
}
