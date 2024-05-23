#include "CsvDriver.h"
#include "DirectoryService.h"
#include "Primitives.h"
#include "iostream"
#include "fstream"
#include "string"
#include <sstream>
#include "GuidFactory.h"
#include "Model.h"

using std::ofstream;
using std::fstream;
using std::string;
using std::endl;
using std::getline;

CsvDriver::CsvDriver(DirectoryService* directory, GuidFactory* guidFactory)
{
	this->directory = directory;
	this->guidFactory = guidFactory;
}

Row CsvDriver::cast_line_to_row(string line)
{
	Row row;
	string token;

	std::istringstream tokenStream(line);
	while (getline(tokenStream, token, ',')) {
		row.push_back(token);
	}

	return row;
}

string CsvDriver::cast_row_to_line(Row row)
{
	string line;
	for (auto& cell : row) {
		line += cell + ",";
	}
	line.pop_back(); // Elimina la ultima coma

	return line;
}

Table CsvDriver::select(string filename)
{
	auto file = this->directory->open_read_file(filename);

	Table table;
	string line;

	while (getline(file, line))
	{
		Row row = cast_line_to_row(line);
		table.push_back(row);
	}

	file.close();
	return table;
}

Row CsvDriver::find(string filename, Guid guid)
{
	auto file = this->directory->open_read_file(filename);

	string line;
	while (getline(file, line))
	{
		Row row = cast_line_to_row(line);
		if (row[0] == guid) // Si contiene el guid
		{
			return row;
		}
	}
}

bool CsvDriver::exists(string filename, Guid guid) {
	auto file = this->directory->open_read_file(filename);

	string line;
	while (getline(file, line))
	{
		Row row = cast_line_to_row(line);
		if (row[0] == guid) // Si contiene el guid
		{
			return true;
		}
	}
	return false;
}

Row CsvDriver::insert(string filename, Row row)
{
	if (this->directory->exists_file(filename) == false) {
		ofstream outputFile = this->directory->create_file(filename);
		outputFile.close();
	}

	auto outputFile = this->directory->open_write_to_file(filename);

	if (!outputFile.is_open()) {
		throw "No se pudo abrir el archivo";
	}
	row.insert(row.begin(), guidFactory->create());

	string parsedFile;
	for (auto& cell : row) {
		parsedFile += cell + ",";
	}
	parsedFile.pop_back(); // Elimina la ultima coma

	outputFile << parsedFile.c_str() << std::endl;
	outputFile.close();

	return row;
}

Row CsvDriver::insert_model(string filename, Model* model)
{
	auto row_to_insert = model->to_row();
	auto inserted_row = this->insert(filename, row_to_insert);
	model->guid = inserted_row[0];
	return inserted_row;
}

vector<Row> CsvDriver::insert(string filename, vector<Row> rows)
{
	vector<Row> insertedRows;
	for (auto& row : rows) {
		insertedRows.push_back(insert(filename, row));
	}
	return insertedRows;
}

void CsvDriver::update(string filename, Guid guid, Row newRow)
{
	auto file = this->directory->open_modify_file(filename);

	string line;
	string newLine;
	std::streampos pos;
	//long startLine = 0;
	long shouldStartLine = 0;

	while (getline(file, line))
	{
		Row row = cast_line_to_row(line);
		if (row[0] == guid)
		{
			// Volver al primer caracter de la linea

			pos = file.tellg();
			// Obtener la posición como un entero
			std::streamoff posInt = static_cast<std::streamoff>(pos) - 1;

			// Calcular la posición de inicio de la línea
			auto startLine = posInt - static_cast<std::streamoff>(line.length()) - 1;
			startLine = startLine < 0 ? 0 : startLine;
			shouldStartLine = startLine;
			// Modificamos la línea utilizando replace
			newLine = line;
			for (int i = 0; i < newRow.size(); ++i) {
				size_t startPos = newLine.find(row[i]);
				newLine.replace(startPos, row[i].length(), newRow[i]);
			}
			break; // Salir del bucle después de encontrar la línea a modificar
		}
	}

	// Volver al principio de la línea para sobrescribirla
	file.seekp(shouldStartLine);

	// Escribir la línea modificada en el archivo
	file << newLine << endl;

	file.close();
}

void CsvDriver::new_csv(string filename)
{
	auto endsWithCsv = filename.find(".csv") != string::npos;

	if (!endsWithCsv) {
		filename += ".csv";
	}

	auto exists = this->directory->exists_directory();

	if (!exists) {
		this->directory->create_directory();
	}

	if (!this->directory->exists_file(filename)) {
		this->directory->create_file(filename);
	}
}

void CsvDriver::delete_csv(string filename)
{
	this->directory->drop_file(filename);
}
