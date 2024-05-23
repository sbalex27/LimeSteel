#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::cin;
using std::string;

int main()
{
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

		switch (menu_option)
		{
		case 1:
			cout << "Crear factura" << endl;
			break;
		case 2:
			cout << "Consultar factura" << endl;
			break;
		case 3:
			cout << "Modificar factura" << endl;
			break;
		case 4:
			cout << "Eliminar factura" << endl;
			break;
		case 5:
			cout << "Saliendo..." << endl;
			break;
		default:
			cout << "Opcion no valida" << endl;
			break;
		}
		system("pause");
		system("cls");
	} while (menu_option != 5);
}
