#pragma once
#include "CsvDriver.h"
#include "Table.h"
#include <functional>
#include "Person.h"
#include "Invoice.h"
#include "Constants.h"

using std::function;

class InvoiceRepository
{
private:
	CsvDriver* csv;

public:
	InvoiceRepository(CsvDriver* csv) {
		this->csv = csv;
	}

	Guid insert(Invoice* invoice) {
		// Check if person exists, if not, insert it
		if (invoice->person->has_not_guid()) {
			csv->insert_model(PEOPLE_TABLE, invoice->person);
		}

		invoice->person_guid = invoice->person->guid;

		csv->insert_model(INVOICES_TABLE, invoice);

		for (auto product : invoice->products) {
			product->invoice_guid = invoice->guid;
			csv->insert_model(PRODUCTS_TABLE, product);
		}

		return invoice->guid;
	}

	vector<Invoice*> all() {
		// Get all invoices
		vector<Row> rows = csv->select(INVOICES_TABLE);
		vector<Invoice*> invoices;
		for (Row row : rows) {
			auto invoice = new Invoice(row);

			// Get all products for each invoice
			auto products_datatable = csv->select(PRODUCTS_TABLE);

			vector<Product*> products = {};
			for (Row product_row : products_datatable) {
				auto product = new Product(product_row);
				if (product->invoice_guid == invoice->guid) {
					products.push_back(product);
				}
			}

			invoice->products = products;

			// Get person for each invoice
			auto person_row = csv->find(PEOPLE_TABLE, invoice->person_guid);
			if (person_row.size() != 0) {
				invoice->person = new Person(person_row);
				invoice->person_guid = invoice->person->guid;
				invoices.push_back(invoice);
			}
		}

		return invoices;
	}

	int next_invoice_number() {
		auto invoices = all();
		int max = 0;
		for (auto invoice : invoices) {
			if (invoice->invoice_number > max) {
				max = invoice->invoice_number;
			}
		}
		return max + 1;
	}

	void update_invoice(Invoice* invoice)
	{
		csv->update(INVOICES_TABLE, invoice->guid, invoice->to_row());
	}

	Invoice* find_by_invoice_number(double number)
	{
		auto all = this->all();
		for (auto invoice : all) {
			if (invoice->invoice_number == number) {
				return invoice;
			}
		}
		return nullptr;
	}
};
