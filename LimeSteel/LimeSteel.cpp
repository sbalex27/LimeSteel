#include <iostream>
#include <string>
#include "CsvDriver.h"
#include "Constants.h"
#include "Migrations.h"
#include "InvoiceRepository.h"
#include "PersonRepository.h"
#include <iomanip>
#include <vector>

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::setw;

static void title(string title)
{
	cout << title << endl;
	cout << "----------------------------------------" << endl;
	cout << endl;
}

static void subtitle(string subtitle)
{
	cout << endl;
	cout << "-- " << subtitle << " --" << endl;
	cout << endl;
}

static void print_invoice_ui(Invoice* invoice)
{
	string is_paid = invoice->is_paid() ? "Si" : "No";
	std::cout << std::string(50, '-') << std::endl;
	cout << "FACTURA #" << invoice->invoice_number << endl;
	cout << "CLIENTE: " << invoice->person->get_full_name() << endl;
	cout << "NIT: " << invoice->person->nit << endl;
	cout << "CIUDAD: " << invoice->person->address << endl;
	cout << "TELEFONO: " << invoice->person->phone << endl;
	cout << "EDAD: " << invoice->person->age << endl;
	cout << endl;
	cout << "PRODUCTOS" << endl;
	cout << endl;
	// Encabezados de la tabla
	std::cout << std::left << std::setw(20) << "Nombre"
		<< std::right << std::setw(10) << "Precio"
		<< std::setw(10) << "Cantidad"
		<< std::setw(10) << "Total" << std::endl;
	std::cout << std::string(50, '-') << std::endl;

	for (auto& product : invoice->products)
	{
		std::cout << std::left << std::setw(20) << product->name
			<< std::right << std::setw(10) << std::fixed << std::setprecision(2) << product->price
			<< std::setw(10) << product->quantity
			<< std::setw(10) << product->total() << std::endl;
	}
	cout << endl;
	std::cout << std::left << std::setw(40) << "TOTAL: " << invoice->get_products_total() << std::endl;
	std::cout << std::left << std::setw(40) << "SALDO PENDIENTE: " << invoice->remaining_to_pay() << std::endl;
	std::cout << std::left << std::setw(40) << "ANULADA: " << (invoice->is_active ? "No" : "Si") << std::endl;
	std::cout << std::left << std::setw(40) << "SOLVENTE: " << is_paid << std::endl;
	std::cout << std::endl;
}

int main()
{
	// Register
	DirectoryService* directoryService = new DirectoryService(DATABASE_PATH);
	Migrations* migrations = new Migrations(directoryService);
	GuidFactory* guidFactory = new GuidFactory();
	CsvDriver* csvDriver = new CsvDriver(directoryService, guidFactory);
	InvoiceRepository* invoiceRepository = new InvoiceRepository(csvDriver);
	PersonRepository* personRepository = new PersonRepository(csvDriver);

	// Bootstrap
	migrations->Up();

	int menu_option;
	do
	{
		cout << "ADMINISTRADOR DE FACTURAS - LIMESTEEL" << endl;
		cout << "----------------------------------------" << endl;
		cout << "1. Crear factura" << endl;
		cout << "2. Consultar factura" << endl;
		cout << "3. Hacer un pago" << endl;
		cout << "4. Anular factura" << endl;
		cout << "5. Salir" << endl;
		cout << "----------------------------------------" << endl;
		cout << "Seleccione una opcion: ";
		cin >> menu_option;
		system("cls");
		switch (menu_option)
		{
		case 1:
		{
			Invoice* invoice = new Invoice();

			title("Crear factura");
			Person* person = new Person();
			cout << "Nombre del Cliente: ";
			cin >> person->name;
			cout << "Apellido del Cliente: ";
			cin >> person->lastname;
			cout << "NIT: ";
			cin >> person->nit;
			cout << "Direccion: ";
			cin >> person->address;
			cout << "Telefono: ";
			cin >> person->phone;
			cout << "Edad: ";
			cin >> person->age;

			invoice->person = person;

			int continue_adding_products = 1;
			subtitle("Productos");

			do
			{
				Product* product = new Product();
				cout << "Nombre del Producto: ";
				cin >> product->name;
				cout << "Precio del Producto: ";
				cin >> product->price;
				cout << "Cantidad: ";
				cin >> product->quantity;

				invoice->products.push_back(product);
				cout << "Desea agregar otro producto? (1 = Si, 0 = No): ";
				cin >> continue_adding_products;
			} while (continue_adding_products == 1);

			cout << "Descripcion de la factura: ";
			cin >> invoice->description;
			invoice->invoice_number = invoiceRepository->next_invoice_number();

			invoiceRepository->insert(invoice);
			delete person;
			delete invoice;
			break;
		}
		case 2:
		{
			cout << "Consultar factura" << endl;
			vector<Invoice*> invoices = invoiceRepository->all();

			for (auto& invoice : invoices) {
				string is_paid = invoice->is_paid() ? "Si" : "No";
				cout << "----------------------------------------" << endl;
				cout << "Factura #" << invoice->invoice_number << endl;
				cout << "Cliente: " << invoice->person->get_full_name() << endl;
				cout << "NIT: " << invoice->person->nit << endl;
				cout << "TOTAL: " << invoice->get_products_total() << endl;
				cout << "Saldo pendiente: " << invoice->remaining_to_pay() << endl;
				cout << "Anulada: " << (invoice->is_active ? "No" : "Si") << endl;
				cout << "Solvente: " << is_paid << endl;
				cout << endl;
			}

			double invoice_number;
			cout << "Ingrese el numero de factura: ";
			cin >> invoice_number;

			Invoice* invoice = invoiceRepository->find_by_invoice_number(invoice_number);
			if (invoice == nullptr)
			{
				cout << "Factura no encontrada" << endl;
				break;
			}
			system("cls");
			print_invoice_ui(invoice);
			break;
		}
		case 3:
		{
			title("Realizar un pago");
			cout << "Ingrese el numero de factura: ";
			double invoice_number;
			cin >> invoice_number;
			Invoice* invoice = invoiceRepository->find_by_invoice_number(invoice_number);
			if (invoice == nullptr)
			{
				cout << "Factura no encontrada" << endl;
				break;
			}

			cout << "Cliente: " << invoice->person->get_full_name() << endl;
			cout << "Monto total: " << invoice->get_products_total() << endl;
			cout << "Saldo pendiente: " << invoice->remaining_to_pay() << endl;
			cout << "Ingrese el monto a pagar: ";
			double amount;
			cin >> amount;

			invoice->pay(amount);

			invoiceRepository->update_invoice(invoice);
			delete invoice;
			cout << "Pago realizado con exito" << endl;
			break;
		}
		case 4:
		{
			title("Anular factura");
			cout << "Ingrese el numero de factura: ";
			double invoice_number;
			cin >> invoice_number;
			Invoice* invoice = invoiceRepository->find_by_invoice_number(invoice_number);
			if (invoice == nullptr)
			{
				cout << "Factura no encontrada" << endl;
				break;
			}
			invoice->is_active = false;
			invoiceRepository->update_invoice(invoice);
			delete invoice;
			cout << "Factura anulada con exito" << endl;
		}
		case 5:
			cout << "Saliendo..." << endl;
			break;
		case 100:
			migrations->Down();
			break;
		default:
			cout << "Opcion no valida" << endl;
			break;
		}
		system("pause");
		system("cls");
	} while (menu_option != 5);
}
