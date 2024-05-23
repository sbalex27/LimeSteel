#pragma once
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include "Invoice.h"
#include "DirectoryService.h"

class Printer
{
private:
	DirectoryService* directory;
public:
	Printer(DirectoryService* directory)
	{
		this->directory = directory;
	}

	void Up() {
		if (!directory->exists_directory(DirectoryType::Files))
			directory->create_directory(DirectoryType::Files);
	}

	string print_invoice_to_file(Invoice* invoice) {
		std::string filename = invoice->person->get_full_name() + "_" + std::to_string(invoice->invoice_number) + ".txt";

		auto file = directory->create_file(filename, DirectoryType::Files);

		std::string is_paid = invoice->is_paid() ? "Si" : "No";
		file << std::string(50, '-') << std::endl;
		file << "FACTURA #" << invoice->invoice_number << std::endl;
		file << "CLIENTE: " << invoice->person->get_full_name() << std::endl;
		file << "NIT: " << invoice->person->nit << std::endl;
		file << "CIUDAD: " << invoice->person->address << std::endl;
		file << "TELEFONO: " << invoice->person->phone << std::endl;
		file << "EDAD: " << invoice->person->age << std::endl;
		file << std::endl;
		file << "PRODUCTOS" << std::endl;
		file << std::endl;
		// Encabezados de la tabla
		file << std::left << std::setw(20) << "Nombre"
			<< std::right << std::setw(10) << "Precio"
			<< std::setw(10) << "Cantidad"
			<< std::setw(10) << "Total" << std::endl;
		file << std::string(50, '-') << std::endl;

		for (const auto& product : invoice->products) {
			file << std::left << std::setw(20) << product->name
				<< std::right << std::setw(10) << std::fixed << std::setprecision(2) << product->price
				<< std::setw(10) << product->quantity
				<< std::setw(10) << product->total() << std::endl;
		}
		file << std::endl;
		file << std::left << std::setw(40) << "TOTAL: " << invoice->get_products_total() << std::endl;
		file << std::left << std::setw(40) << "SALDO PENDIENTE: " << invoice->remaining_to_pay() << std::endl;
		file << std::left << std::setw(40) << "ANULADA: " << (invoice->is_active ? "No" : "Si") << std::endl;
		file << std::left << std::setw(40) << "SOLVENTE: " << is_paid << std::endl;
		file << std::endl;

		file.close();

		auto save_path = directory->files_path + "\\" + filename;
		return save_path;
	}
};

