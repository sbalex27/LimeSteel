#pragma once
#include "Constants.h"
#include "DirectoryService.h"

class Migrations
{
private:
	DirectoryService* directory;
public:
	Migrations(DirectoryService* directory) {
		this->directory = directory;
	}

	void Up() {
		if (!this->directory->exists_directory()) {
			this->directory->create_directory();
		}

		if (!this->directory->exists_file(PEOPLE_TABLE)) this->directory->create_file(PEOPLE_TABLE);
		if (!this->directory->exists_file(INVOICES_TABLE)) this->directory->create_file(INVOICES_TABLE);
		if (!this->directory->exists_file(PRODUCTS_TABLE)) this->directory->create_file(PRODUCTS_TABLE);
	}

	void Down() {
		this->directory->drop_file(PRODUCTS_TABLE);
		this->directory->drop_file(INVOICES_TABLE);
		this->directory->drop_file(PEOPLE_TABLE);
	}
};

