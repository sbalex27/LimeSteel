#include "pch.h"
#include "CppUnitTest.h"
#include <filesystem>
#include "../LimeSteel/CsvManager.h"
#include "../LimeSteel/CsvManager.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// Path to the directory where the tests will be executed, USE WITH CAUTION
constexpr auto TESTS_PATH = "C:\\Users\\alex2\\LimeSteel\\Tests";

using std::string;
using std::wstring;
using std::system;

namespace UnitTest
{
	TEST_CLASS(DirectoryTest)
	{
	public:

		CsvManager* manager = nullptr;

		TEST_METHOD_INITIALIZE(Setup)
		{
			this->manager = new CsvManager(TESTS_PATH);
		}

		TEST_METHOD_CLEANUP(Teardown)
		{
			system(("rmdir /s /q " + string(TESTS_PATH)).c_str());
			delete this->manager;
		}

		TEST_METHOD(CreateBaseDirectory)
		{
			// Arrange [No required]

			// Act
			auto created = manager->create_directory();

			// Assert
			wstring errorMessage = L"Directory was not created in " + wstring(manager->path.begin(), manager->path.end());
			Assert::IsTrue(created, errorMessage.c_str());
		}

		TEST_METHOD(DropBaseDirectory)
		{
			// Arrange
			system(("mkdir " + string(TESTS_PATH)).c_str());

			// Act
			auto dropped = manager->drop_directory();

			// Assert
			wstring errorMessage = L"Directory was not dropped in " + wstring(manager->path.begin(), manager->path.end());
			Assert::IsTrue(dropped, errorMessage.c_str());
		}

		TEST_METHOD(ExistsBaseDirectory)
		{
			// Arrange
			system(("mkdir " + string(TESTS_PATH)).c_str());

			// Act
			auto exists = manager->exists_directory();

			// Assert
			wstring errorMessage = L"Directory does not exist in " + wstring(manager->path.begin(), manager->path.end());
			Assert::IsTrue(exists, errorMessage.c_str());
		}

		TEST_METHOD(NotExistsBaseDirectory)
		{
			// Arrange
			system(("rmdir /s /q " + string(TESTS_PATH)).c_str());

			// Act
			auto exists = manager->exists_directory();

			// Assert
			wstring errorMessage = L"Directory exists in " + wstring(manager->path.begin(), manager->path.end());
			Assert::IsFalse(exists, errorMessage.c_str());
		}

		TEST_METHOD(CreateFile)
		{
			// Arrange
			system(("mkdir " + string(TESTS_PATH)).c_str());
			auto filename = "test.csv";

			// Act
			auto file = manager->create_file(filename);

			// Assert
			auto isOpen = file.is_open();
			wstring errorMessage = L"File was not created in " + wstring(manager->path.begin(), manager->path.end());
			Assert::IsTrue(isOpen, errorMessage.c_str());
		}

		TEST_METHOD(ExistsFile)
		{
			// Arrange
			system(("mkdir " + string(TESTS_PATH)).c_str());
			system(("echo. 2> " + string(TESTS_PATH) + "\\test.csv").c_str());
			auto filename = "test.csv";

			// Act
			auto exists = manager->exists_file(filename);

			// Assert
			wstring errorMessage = L"Operation invalid in: " + wstring(manager->path.begin(), manager->path.end());
			Assert::IsTrue(exists, errorMessage.c_str());
		}

		TEST_METHOD(NotExistsFile)
		{
			// Arrange
			system(("mkdir " + string(TESTS_PATH)).c_str());
			auto filename = "test.csv";

			// Act
			auto exists = manager->exists_file(filename);

			// Assert
			wstring errorMessage = L"Operation invalid in: " + wstring(manager->path.begin(), manager->path.end());
			Assert::IsFalse(exists, errorMessage.c_str());
		}

		TEST_METHOD(DropFile)
		{
			// Arrange
			system(("mkdir " + string(TESTS_PATH)).c_str());
			system(("echo. 2> " + string(TESTS_PATH) + "\\test_to_drop.csv").c_str());
			auto filename = "test_to_drop.csv";

			// Act
			auto dropped = manager->drop_file(filename);

			// Assert
			wstring errorMessage = L"Operation invalid in: " + wstring(manager->path.begin(), manager->path.end());
			Assert::IsTrue(dropped, errorMessage.c_str());
		}

		TEST_METHOD(NotDropedFile)
		{
			// Arrange
			system(("mkdir " + string(TESTS_PATH)).c_str());
			auto filename = "test_to_drop.csv";

			// Act
			auto dropped = manager->drop_file(filename);

			// Assert
			wstring errorMessage = L"Operation invalid in: " + wstring(manager->path.begin(), manager->path.end());
			Assert::IsFalse(dropped, errorMessage.c_str());
		}
	};
}
