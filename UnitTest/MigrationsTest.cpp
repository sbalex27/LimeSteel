#include "pch.h"
#include "CppUnitTest.h"
#include "../LimeSteel/Migrations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

constexpr auto TESTS_PATH = "C:\\Users\\alex2\\LimeSteel\\Tests";

namespace UnitTest
{
	TEST_CLASS(MigrationsTest)
	{
	public:
		TEST_METHOD(UpCreateDirectory)
		{
			// Arrange
			DirectoryService* directory = new DirectoryService(TESTS_PATH);
			Migrations* migrations = new Migrations(directory);

			// Act
			migrations->Up();

			// Assert
			bool exists = directory->exists_directory();
			Assert::IsTrue(exists);
		}

		TEST_METHOD(UpCreatePeopleTable)
		{
			// Arrange
			DirectoryService* directory = new DirectoryService(TESTS_PATH);
			Migrations* migrations = new Migrations(directory);

			// Act
			migrations->Up();

			// Assert
			bool exists = directory->exists_file(PEOPLE_TABLE);
			Assert::IsTrue(exists);
		}

		TEST_METHOD(UpCreateInvoicesTable)
		{
			// Arrange
			DirectoryService* directory = new DirectoryService(TESTS_PATH);
			Migrations* migrations = new Migrations(directory);

			// Act
			migrations->Up();

			// Assert
			bool exists = directory->exists_file(INVOICES_TABLE);
			Assert::IsTrue(exists);
		}

		TEST_METHOD(UpCreateProductsTable)
		{
			// Arrange
			DirectoryService* directory = new DirectoryService(TESTS_PATH);
			Migrations* migrations = new Migrations(directory);

			// Act
			migrations->Up();

			// Assert
			bool exists = directory->exists_file(PRODUCTS_TABLE);
			Assert::IsTrue(exists);
		}

		TEST_METHOD(DownDropPeopleTable)
		{
			// Arrange
			DirectoryService* directory = new DirectoryService(TESTS_PATH);
			Migrations* migrations = new Migrations(directory);
			migrations->Up();

			// Act
			migrations->Down();

			// Assert
			bool exists = directory->exists_file(PEOPLE_TABLE);
			Assert::IsFalse(exists);
		}

		TEST_METHOD(DownDropInvoicesTable)
		{
			// Arrange
			DirectoryService* directory = new DirectoryService(TESTS_PATH);
			Migrations* migrations = new Migrations(directory);
			migrations->Up();

			// Act
			migrations->Down();

			// Assert
			bool exists = directory->exists_file(INVOICES_TABLE);
			Assert::IsFalse(exists);
		}

		TEST_METHOD(DownDropProductsTable)
		{
			// Arrange
			DirectoryService* directory = new DirectoryService(TESTS_PATH);
			Migrations* migrations = new Migrations(directory);
			migrations->Up();

			// Act
			migrations->Down();

			// Assert
			bool exists = directory->exists_file(PRODUCTS_TABLE);
			Assert::IsFalse(exists);
		}
	};
}
