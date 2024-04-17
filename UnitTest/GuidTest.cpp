#include "pch.h"
#include "CppUnitTest.h"
#include "../LimeSteel/GuidFactory.h";
#include "../LimeSteel/GuidFactory.cpp";

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(GuidTest)
	{
	public:
		GuidFactory* guidFactory = nullptr;

		TEST_METHOD_INITIALIZE(Setup)
		{
			guidFactory = new GuidFactory();
		}

		TEST_METHOD_CLEANUP(Teardown)
		{
			delete guidFactory;
		}

		TEST_METHOD(CreateGuid)
		{
			auto guid = this->guidFactory->create();
			auto isNotEmpty = guid.size() > 0;

			Assert::IsTrue(isNotEmpty);
		}

		TEST_METHOD(IsGuid)
		{
			auto guid = this->guidFactory->create();
			auto isGuid = this->guidFactory->is_guid(guid);

			Assert::IsTrue(isGuid);
		}

		TEST_METHOD(IsNotGuidWord)
		{
			auto guid = "FooBar";
			auto isGuid = this->guidFactory->is_guid(guid);

			Assert::IsFalse(isGuid);
		}

		TEST_METHOD(IsNotGuidSize)
		{
			auto guid = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx-xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx";
			auto isGuid = this->guidFactory->is_guid(guid);

			Assert::IsFalse(isGuid);
		}

		TEST_METHOD(IsNotGuidDash)
		{
			auto guid = "xxxxxxxx-xxxx-4xxx-yxxxXxxxxxxxxxxxx";
			auto isGuid = this->guidFactory->is_guid(guid);

			Assert::IsFalse(isGuid);
		}
	};
}
