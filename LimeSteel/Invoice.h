#pragma once
#include "Model.h"
#include "InvalidRowSizeException.h"


class Product : public Model
{
public:
	Guid invoice_guid;
	string name;
	double price = 0.0;
	int quantity = 0;

	Product() {};
	Product(Row row) {
		if (row.size() != 5) throw InvalidRowSizeException("Product row size must be 5");
		guid = row[0];
		invoice_guid = row[1];
		name = row[2];
		price = stod(row[3]);
		quantity = stoi(row[4]);
	}

	Product(Guid guid, Guid invoice_guid, string name, double price, int quantity) {
		this->guid = guid;
		this->invoice_guid = invoice_guid;
		this->name = name;
		this->price = price;
		this->quantity = quantity;
	}

	Row to_row() {
		Row row;
		if (has_guid()) {
			row.push_back(guid);
		}
		row.push_back(invoice_guid);
		row.push_back(name);
		row.push_back(std::to_string(price));
		row.push_back(std::to_string(quantity));
		return row;
	}
};

class Invoice : public Model
{
public:
	Guid person_guid;
	double invoice_number = 0.0;
	string description;
	double amount_paid = 0.0;

	// Navigation properties
	Person* person = nullptr;
	vector<Product*> products = {};

	Invoice() {};
	Invoice(Row row) {
		if (row.size() != 5) throw InvalidRowSizeException("Invoice row size must be 4");
		this->guid = row[0];
		this->person_guid = row[1];
		this->invoice_number = stod(row[2]);
		this->description = row[3];
		this->amount_paid = stod(row[4]);
	}

	Invoice(Guid guid, Guid person_guid, double invoice_number, string description, double amount_paid) {
		this->guid = guid;
		this->person_guid = person_guid;
		this->invoice_number = invoice_number;
		this->description = description;
		this->amount_paid = amount_paid;
	}

	double get_products_total() const {
		double total = 0.0;
		for (auto product : products) {
			total += product->price * product->quantity;
		}
		return total;
	}

	bool remaining_to_pay() const {
		return get_products_total() - amount_paid;
	}

	bool is_paid() const {
		return get_products_total() >= amount_paid;
	}

	bool is_not_paid() const {
		return !is_paid();
	}

	void pay(double amount) {
		if (amount > remaining_to_pay())
		{
			throw std::invalid_argument("El pago debe ser menor o igual al saldo pendiente");
		}
		amount_paid += amount;
	}

	Row to_row() {
		Row row;
		if (has_guid()) {
			row.push_back(guid);
		}
		row.push_back(person_guid);
		row.push_back(std::to_string(invoice_number));
		row.push_back(description);
		row.push_back(std::to_string(amount_paid));
		return row;
	}
};
