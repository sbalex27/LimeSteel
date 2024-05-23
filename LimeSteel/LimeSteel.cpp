#include <iostream>
#include <string>
#include "CsvDriver.h"
#include "Constants.h"
#include "Migrations.h"
#include "InvoiceRepository.h"
#include "PersonRepository.h"

using std::cout;
using std::endl;
using std::cin;
using std::string;

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
		cout << "3. Modificar factura" << endl;
		cout << "4. Eliminar factura" << endl;
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
				cout << "Solvente: " << is_paid << endl;
				cout << endl;
			}
			break;
		}
		case 3:
			cout << "Modificar factura" << endl;
			break;
		case 4:
			cout << "Eliminar factura" << endl;
			break;
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
