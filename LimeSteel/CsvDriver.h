#pragma once
#include "DirectoryService.h"
#include "GuidFactory.h"
#include "Table.h"

class CsvDriver
{
private:
	DirectoryService* directory;
	GuidFactory* guidFactory;
public:
	CsvDriver(DirectoryService* directory, GuidFactory* guidFactory);

	Row cast_line_to_row(string line);
	string cast_row_to_line(Row row);

	/// <summary>
	/// Obtiene todos los datos de un archivo CSV convertidos a tipo Table
	/// </summary>
	/// <param name="filename">Nombre del archivo CSV</param>
	/// <returns>Tabla no serializada</returns>
	Table select(string filename);

	/// <summary>
	/// Inserta una nueva fila en un archivo CSV (equivalente a un registro en una tabla).
	/// Este método genera un GUID para la fila en la primera columna.
	/// </summary>
	/// <param name="filename">Nombre del archivo</param>
	/// <param name="row">Fila a insertar</param>
	/// <returns>Fila insertada (con el GUID generado)</returns>
	Row insert(string filename, Row row);

	void update(string filename, Guid guid, Row row);
};

