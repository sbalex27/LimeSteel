#pragma once
#include <iostream>
#include <fstream>
#include <string>

enum DirectoryType
{
	Database,
	Files
};

constexpr auto PROGRAM_BASE_PATH = "C:\\Users\\alex2\\LimeSteel\\Data";

using std::string;
using std::ifstream;
using std::ofstream;
using std::fstream;

class DirectoryService
{
public:
	string path;
	//string path_type(DirectoryType type);
	string files_path;
	DirectoryService(string path, string files_path = "");
	/// <summary>
	/// Crea un directorio en la ruta con la que la instancia fue creada
	/// </summary>
	/// <returns>Verdadero si fue creada exitosamente</returns>
	bool create_directory(DirectoryType type = DirectoryType::Database) const;

	/// <summary>
	/// Borra el directorio en la ruta con la que la instancia fue creada
	/// </summary>
	/// <returns>Verdadero si fue eliminada exitosamente</returns>
	bool drop_directory(DirectoryType type = DirectoryType::Database) const;

	/// <summary>
	/// Valida si el directorio en la ruta con la que la instancia fue creada existe
	/// </summary>
	/// <returns>Verdadero si sí existe, caso contrario falso</returns>
	bool exists_directory(DirectoryType type = DirectoryType::Database) const;

	/// Crea y devuelve el archivo en la ruta con la que la instancia fue creada
	/// </summary>
	/// <param name="filename">Nombre del archivo a crear (debe incluir extension, ej .csv, .txt, etc...)</param>
	/// <returns>Archivo recién creado</returns>
	ofstream create_file(string filename, DirectoryType type = DirectoryType::Database) const;

	/// <summary>
	/// Busca si el archivo con el nombre dado existe en la ruta con la que la instancia fue creada
	/// </summary>
	/// <param name="filename">Nombre del archivo a buscar</param>
	/// <returns>Verdadero si existe, caso contrario falso</returns>
	bool exists_file(string filename, DirectoryType type = DirectoryType::Database) const;

	/// <summary>
	/// Borra el archivo con el nombre dado en la ruta con la que la instancia fue creada
	/// </summary>
	/// <param name="filename">Nombre del archivo</param>
	/// <returns>Verdadero si fue borrado exitosamente</returns>
	bool drop_file(string filename) const;

	/// <summary>
	/// Abre el archivo con el nombre dado en la ruta con la que la instancia fue creada
	/// </summary>
	/// <param name="filename">Nombre del archivo</param>
	/// <returns>Archivo existente que se ha abierto</returns>
	ifstream open_read_file(string filename) const;

	ofstream open_write_to_file(string filename) const;

	fstream open_modify_file(string filename) const;
};

