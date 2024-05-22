#pragma once
#include <stdexcept>
#include "Model.h"
#include "InvalidRowSizeException.h"
#include "string.h"

class Person : public Model
{
public:
	string name;
	string lastname;
	string phone;
	int age = 0;

	Person() {};
	Person(Row row) {
		if (row.size() != 5) throw InvalidRowSizeException("Person row size must be 5");
		guid = row[0];
		name = row[1];
		lastname = row[2];
		phone = row[3];
		age = stoi(row[4]);
	}
	Person(Guid guid, string name, string lastname, string phone, int age) {
		this->guid = guid;
		this->name = name;
		this->lastname = lastname;
		this->phone = phone;
		this->age = age;
	}

	string get_full_name() const {
		return name + " " + lastname;
	}

	Row to_row() {
		Row row;
		if (has_guid()) {
			row.push_back(guid);
		}
		row.push_back(name);
		row.push_back(lastname);
		row.push_back(phone);
		row.push_back(std::to_string(age));
		return row;
	}
};

