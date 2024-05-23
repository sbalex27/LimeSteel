#include "pch.h"
#include "CppUnitTest.h"
#include "../LimeSteel/InvoiceRepository.h"
#include "../LimeSteel/Constants.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

constexpr auto TESTS_PATH = "C:\\Users\\alex2\\LimeSteel\\Tests";

namespace UnitTest
{
	TEST_CLASS(InvoiceRepositoryTest)
	{
	public:
		CsvDriver* csv = nullptr;
		GuidFactory* guidFactory = nullptr;
		DirectoryService* directory = nullptr;
		InvoiceRepository* repository = nullptr;

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
			this->csv = new CsvDriver(directory, guidFactory);
			this->repository = new InvoiceRepository(csv);
		}

		TEST_METHOD_CLEANUP(Teardown)
		{
			system(("rmdir /s /q " + string(TESTS_PATH)).c_str());
			delete this->directory;
			delete this->guidFactory;
			delete this->csv;
		}

		TEST_METHOD(InvoiceCreateGuid)
		{
			// Arrange
			Person* person = new Person();
			person->name = "Sergio";
			person->lastname = "Batres";
			person->phone = "41925033";
			person->age = 24;

			Invoice* invoice = new Invoice();
			invoice->person = person;
			invoice->invoice_number = 1;
			invoice->description = "Test invoice";
			invoice->amount_paid = 0;

			Product* product1 = new Product();
			product1->name = "Product 1";
			product1->price = 10;
			product1->quantity = 1;

			Product* product2 = new Product();
			product2->name = "Product 2";
			product2->price = 20;
			product2->quantity = 2;

			invoice->products.push_back(product1);
			invoice->products.push_back(product2);

			// Act
			auto invoice_guid = this->repository->insert(invoice);

			// Assert
			auto exists = this->csv->exists(INVOICES_TABLE, invoice_guid);
			Assert::IsTrue(exists);
		}

		TEST_METHOD(QueryAllInvoices)
		{
			// Arrange
			Person* person = new Person();
			person->name = "Sergio";
			person->lastname = "Batres";
			person->phone = "41925033";
			person->age = 24;

			Invoice* invoice = new Invoice();
			invoice->person = person;
			invoice->invoice_number = 1;
			invoice->description = "Test invoice";
			invoice->amount_paid = 0;

			Invoice* invoice2 = new Invoice();
			invoice2->person = person;
			invoice2->invoice_number = 2;
			invoice2->description = "Test invoice 2";
			invoice2->amount_paid = 0;

			Invoice* invoice3 = new Invoice();
			invoice3->person = person;
			invoice3->invoice_number = 3;
			invoice3->description = "Test invoice 3";
			invoice3->amount_paid = 0;

			// Act
			auto invoice_guid = this->repository->insert(invoice);
			auto invoice_guid2 = this->repository->insert(invoice2);
			auto invoice_guid3 = this->repository->insert(invoice3);

			// Assert
			auto invoices = this->repository->all();
			auto rowCount = invoices.size() == 3;
			auto message = L"Expected 3 row, got " + std::to_wstring(invoices.size());
			Assert::IsTrue(rowCount, message.c_str());

			auto first = invoices[0]->guid == invoice_guid;
			auto second = invoices[1]->guid == invoice_guid2;
			auto third = invoices[2]->guid == invoice_guid3;
			Assert::IsTrue(first && second && third);
		}

		TEST_METHOD(QueryAllIncludePerson)
		{
			// Arrange
			Person* person = new Person();
			person->name = "Sergio";
			person->lastname = "Batres";
			person->phone = "41925033";
			person->age = 24;

			Invoice* invoice = new Invoice();
			invoice->person = person;
			invoice->invoice_number = 1;
			invoice->description = "Test invoice";
			invoice->amount_paid = 0;

			Product* product1 = new Product();
			product1->name = "Product 1";
			product1->price = 10;
			product1->quantity = 1;

			Product* product2 = new Product();
			product2->name = "Product 2";
			product2->price = 20;
			product2->quantity = 2;

			invoice->products.push_back(product1);
			invoice->products.push_back(product2);

			// Act
			auto invoice_guid = this->repository->insert(invoice);

			// Assert
			vector<Invoice*> query = this->repository->all();
			auto first = query[0];
			auto has_guid = first->person->has_guid();
			Assert::IsTrue(has_guid);
		}

		TEST_METHOD(QueryAllIncludesProducts)
		{
			// Arrange
			Person* person = new Person();
			person->name = "Sergio";
			person->lastname = "Batres";
			person->phone = "41925033";
			person->age = 24;

			Invoice* invoice = new Invoice();
			invoice->person = person;
			invoice->invoice_number = 1;
			invoice->description = "Test invoice";
			invoice->amount_paid = 0;

			Product* product1 = new Product();
			product1->name = "Product 1";
			product1->price = 10;
			product1->quantity = 1;

			Product* product2 = new Product();
			product2->name = "Product 2";
			product2->price = 20;
			product2->quantity = 2;

			invoice->products.push_back(product1);
			invoice->products.push_back(product2);

			// Act
			auto invoice_guid = this->repository->insert(invoice);

			// Assert
			auto query = this->repository->all();

			for (auto& product : query[0]->products)
			{
				auto has_guid = product->has_guid();
				auto has_invoice_guid = product->invoice_guid == invoice_guid;
				Assert::IsTrue(has_guid && has_invoice_guid);
			}
		}

		TEST_METHOD(OnlyAsociatePerson)
		{
			// Arrange
			Person* person = new Person();
			person->name = "Sergio";
			person->lastname = "Batres";
			person->phone = "41925033";
			person->age = 24;

			this->csv->insert_model(PEOPLE_TABLE, person);

			Guid original_person_guid = person->guid;

			Invoice* invoice = new Invoice();
			invoice->person = person;
			invoice->invoice_number = 1;
			invoice->description = "Test invoice";
			invoice->amount_paid = 0;

			// Act
			auto invoice_guid = this->repository->insert(invoice);

			// Assert
			auto table = this->csv->select(PEOPLE_TABLE);
			int guid_count = 0;

			for (auto row : table)
			{
				auto person_selected = Person(row);
				if (person_selected.guid == original_person_guid)
				{
					guid_count++;
				}
			}

			Assert::IsTrue(guid_count == 1);
			Assert::IsTrue(original_person_guid == invoice->person->guid);
			Assert::IsTrue(original_person_guid == invoice->person_guid);
		}

		TEST_METHOD(InvoiceCreatesProducts)
		{
			// Arrange
			Person* person = new Person();
			person->name = "Sergio";
			person->lastname = "Batres";
			person->phone = "41925033";
			person->age = 24;

			Invoice* invoice = new Invoice();
			invoice->person = person;
			invoice->invoice_number = 1;
			invoice->description = "Test invoice";
			invoice->amount_paid = 0;

			Product* product1 = new Product();
			product1->name = "Product 1";
			product1->price = 10;
			product1->quantity = 1;

			Product* product2 = new Product();
			product2->name = "Product 2";
			product2->price = 20;
			product2->quantity = 2;

			invoice->products.push_back(product1);
			invoice->products.push_back(product2);

			// Act
			this->repository->insert(invoice);

			// Assert
			for (auto product : invoice->products)
			{
				auto exists = this->csv->exists(PRODUCTS_TABLE, product->guid);
				Assert::IsTrue(exists);
			}
		}

		TEST_METHOD(InvoiceCreatesPerson)
		{
			// Arrange
			Person* person = new Person();
			person->name = "Sergio";
			person->lastname = "Batres";
			person->phone = "41925033";
			person->age = 24;

			Invoice* invoice = new Invoice();
			invoice->person = person;
			invoice->invoice_number = 1;
			invoice->description = "Test invoice";
			invoice->amount_paid = 0;

			Product* product1 = new Product();
			product1->name = "Product 1";
			product1->price = 10;
			product1->quantity = 1;

			Product* product2 = new Product();
			product2->name = "Product 2";
			product2->price = 20;
			product2->quantity = 2;

			invoice->products.push_back(product1);
			invoice->products.push_back(product2);

			// Act
			this->repository->insert(invoice);

			// Assert
			auto exists = this->csv->exists(PEOPLE_TABLE, person->guid);
			Assert::IsTrue(exists);
		}
	};
}
