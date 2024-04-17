#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		TEST_METHOD(AssertOkTest)
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
