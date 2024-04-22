#include "CsvDriver.h"
#include "DirectoryService.h"
#include "Table.h"
#include "iostream"
#include "fstream"
#include "string"
#include <sstream>

using std::ofstream;
using std::fstream;
using std::string;
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

void CsvDriver::update(string filename, Guid guid, Row row)
{
	auto file = this->directory->open_modify_file(filename);

	string line;
	while (getline(file, line))
	{
		Row row = cast_line_to_row(line);
		if (row[0] == guid) // Si contiene el guid
		{
			for (int i = 0; i < row.size(); i++)
			{
				line.replace(line.find(row[i]), row[i].size(), row[i]);
			}
			break;
		}
	}
}
