#pragma once
#include "CsvDriver.h"
#include "Table.h"
#include <functional>
#include "Person.h"
#include "Invoice.h"

using std::function;

class InvoiceRepository
{
private:
	CsvDriver* csv;
	string invoices_table_name = "invoices.csv";
	string products_table_name = "products.csv";
	string persons_table_name = "people.csv";

public:
	InvoiceRepository(CsvDriver* csv) {
		this->csv = csv;
	}

	Guid insert(Invoice* invoice) {
		// Check if person exists, if not, insert it
		if (invoice->person->has_not_guid()) {
			csv->insert_model(persons_table_name, invoice->person);
		}

		invoice->person_guid = invoice->person->guid;

		csv->insert_model(invoices_table_name, invoice);

		for (auto product : invoice->products) {
			product->invoice_guid = invoice->guid;
			csv->insert_model(products_table_name, product);
		}

		return invoice->guid;
	}

	vector<Invoice*> all() {
		// Get all invoices
		vector<Row> rows = csv->select(invoices_table_name);
		vector<Invoice*> invoices;
		for (Row row : rows) {
			auto invoice = new Invoice(row);

			// Get all products for each invoice
			auto products_datatable = csv->select(products_table_name);

			vector<Product*> products = {};
			for (Row product_row : products_datatable) {
				auto product = new Product(product_row);
				if (product->invoice_guid == invoice->guid) {
					products.push_back(product);
				}
			}

			invoice->products = products;

			// Get person for each invoice
			auto person_row = csv->find(persons_table_name, invoice->person_guid);
			invoice->person = new Person(person_row);
			invoice->person_guid = invoice->person->guid;
			invoices.push_back(invoice);
		}

		return invoices;
	}
};
