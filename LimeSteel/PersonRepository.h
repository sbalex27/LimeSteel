#pragma once
#include "CsvDriver.h"
#include "Table.h"
#include "Person.h"
#include "Constants.h"

class PersonRepository : public TableFile
{
private:
	CsvDriver* csv;
public:
	PersonRepository(CsvDriver* csv) {
		this->csv = csv;
	}

	void up() {
		csv->new_csv(PEOPLE_TABLE);
	}

	void down() {
		csv->delete_csv(PEOPLE_TABLE);
	}

	Guid insert(Person* person) {
		auto insert = csv->insert(PEOPLE_TABLE, person->to_row());
		person->guid = insert[0];
		return person->guid;

	}

	vector<Person> select() {
		vector<Row> rows = csv->select(PEOPLE_TABLE);
		vector<Person> persons;
		for (Row row : rows) {
			persons.push_back(Person(row));
		}
		return persons;
	}

	void update(Person* person) {
		if(!person->has_guid()) throw std::invalid_argument("Person must have a GUID");
		csv->update(PEOPLE_TABLE, person->guid, person->to_row());
	}

	Person find(Guid guid) {
		Row row = csv->find(PEOPLE_TABLE, guid);
		return Person(row);
	}
};

