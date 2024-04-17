#pragma once
#include "DirectoryService.h";
#include "GuidFactory.h";
#include "Table.h"

class CsvDriver
{
private:
	DirectoryService* directory;
	GuidFactory* guidFactory;
public:
	CsvDriver(DirectoryService* directory, GuidFactory* guidFactory);

	/// <summary>
	/// Obtiene todos los datos de un archivo CSV convertidos a tipo Table
	/// </summary>
	/// <param name="filename">Nombre del archivo CSV</param>
	/// <returns>Tabla no serializada</returns>
	Table Select(string filename);

	Row Insert(string filename, Row row);

	void Update(string filename, Guid guid, Row row);
};

