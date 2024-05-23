#pragma once
#include <stdexcept>
#include "Model.h"
#include "string.h"

class Person : public Model
{
public:
	string name;
	string lastname;
	string nit;
	string address;
	string phone;
	int age = 0;

	Person() {};
	Person(Row row) {
		assert_row(row, 7);
		guid = row[0];
		name = row[1];
		lastname = row[2];
		nit = row[3];
		address = row[4];
		phone = row[5];
		age = stoi(row[6]);
	}
	Person(Guid guid, string name, string lastname, string nit, string address, string phone, int age) {
		this->guid = guid;
		this->name = name;
		this->lastname = lastname;
		this->nit = nit;
		this->address = address;
		this->phone = phone;
		this->age = age;
	}

	Person(string name, string lastname, string nit, string address, string phone, int age) {
		this->name = name;
		this->lastname = lastname;
		this->nit = nit;
		this->address = address;
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
		row.push_back(nit);
		row.push_back(address);
		row.push_back(phone);
		row.push_back(std::to_string(age));
		return row;
	}
};

