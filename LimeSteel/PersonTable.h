#pragma once
#include "CsvDriver.h"
#include "Table.h"
#include "Person.h"


class PersonTable : public TableFile
{
private:
	CsvDriver* csv;
public:
	string invoices_table_name = "people.csv";
	PersonTable(CsvDriver* csv) {
		this->csv = csv;
	}

	void up() {
		csv->new_csv(invoices_table_name);
	}

	void down() {
		csv->delete_csv(invoices_table_name);
	}

	Guid insert(Person* person) {
		auto insert = csv->insert(invoices_table_name, person->to_row());
		person->guid = insert[0];
		return person->guid;

	}

	vector<Person> select() {
		vector<Row> rows = csv->select(invoices_table_name);
		vector<Person> persons;
		for (Row row : rows) {
			persons.push_back(Person(row));
		}
		return persons;
	}

	void update(Person* person) {
		if(!person->has_guid()) throw std::invalid_argument("Person must have a GUID");
		csv->update(invoices_table_name, person->guid, person->to_row());
	}

	Person find(Guid guid) {
		Row row = csv->find(invoices_table_name, guid);
		return Person(row);
	}
};

