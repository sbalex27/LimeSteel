#pragma once
#include "DirectoryService.h"
#include "GuidFactory.h"
#include "Primitives.h"
#include "Model.h"

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
	Row find(string file, Guid guid);
	bool exists(string file, Guid guid);
	/// <summary>
	/// Inserta una nueva fila en un archivo CSV (equivalente a un registro en una tabla).
	/// Este método genera un GUID para la fila en la primera columna.
	/// </summary>
	/// <param name="filename">Nombre del archivo</param>
	/// <param name="row">Fila a insertar</param>
	/// <returns>Fila insertada (con el GUID generado)</returns>
	Row insert(string filename, Row row);

	Row insert_model(string filename, Model* model);

	vector<Row> insert(string filename, vector<Row> rows);

	void update(string filename, Guid guid, Row row);

	void new_csv(string filename);
	void delete_csv(string filename);

};

