#include "pch.h"
#include "CppUnitTest.h"
#include "../LimeSteel/CsvDriver.h"
#include "../LimeSteel/CsvDriver.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// Path to the directory where the tests will be executed, USE WITH CAUTION
constexpr auto TESTS_PATH = "C:\\Users\\alex2\\LimeSteel\\Tests";

namespace UnitTest
{
	TEST_CLASS(CsvDriverTest)
	{
	public:
		CsvDriver* csv = nullptr;
		GuidFactory* guidFactory = nullptr;
		DirectoryService* directory = nullptr;

		TEST_METHOD_INITIALIZE(Setup)
		{
			this->directory = new DirectoryService(TESTS_PATH);
			this->guidFactory = new GuidFactory();
			this->csv = new CsvDriver(directory, guidFactory);
		}

		TEST_METHOD_CLEANUP(Teardown)
		{
			directory->drop_directory();
			delete this->csv;
		}

		TEST_METHOD(Insert)
		{
			// Arrange
			auto expected = "Hello World!\n";

			// Act
			std::ostringstream out;
			out << "Hello World!\n";

			// Assert
			Assert::AreEqual(expected, out.str().c_str());
		}
	};
}
