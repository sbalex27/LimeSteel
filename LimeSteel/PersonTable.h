#pragma once
#include "CsvDriver.h"
#include "Table.h"
#include "Person.h"


class PersonTable : public TableFile
{
private:
	CsvDriver* csvDriver;
public:
	string table_name = "people.csv";
	PersonTable(CsvDriver* csvDriver) {
		this->csvDriver = csvDriver;
	}

	void up() {
		csvDriver->new_csv(table_name);
	}

	void down() {
		csvDriver->delete_csv(table_name);
	}

	Guid insert(Person* person) {
		auto new_row = csvDriver->insert(table_name, person->to_row());
		person->guid = new_row[0];
		return person->guid;

	}

	vector<Person> select() {
		vector<Row> rows = csvDriver->select(table_name);
		vector<Person> persons;
		for (Row row : rows) {
			persons.push_back(Person(row));
		}
		return persons;
	}

	void update(Person* person) {
		if(!person->has_guid()) throw std::invalid_argument("Person must have a GUID");
		csvDriver->update(table_name, person->guid, person->to_row());
	}

	Person find(Guid guid) {
		Row row = csvDriver->find(table_name, guid);
		return Person(row);
	}
};

