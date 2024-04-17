#include "CsvDriver.h"
#include "DirectoryService.h"
#include "Table.h"
#include "iostream";
#include "fstream";
#include "string";

using std::ofstream;
using std::fstream;
using std::string;
using std::getline;

CsvDriver::CsvDriver(DirectoryService* directory, GuidFactory* guidFactory)
{
	this->directory = directory;
	this->guidFactory = guidFactory;
}

Table CsvDriver::Select(string filename)
{
	fstream file = directory->open_file(filename);

	Row row;
	Table table;
	Cell cell;

	string line;
	while (getline(file, filename))
	{
		row.clear();
		while (getline(file, cell, ',')) // Delimitador de CSV
		{
			row.push_back(cell); // Agrega la celda a la fila
		}
		table.push_back(row); // Agrega la fila a la tabla
	}

	return table;
}

Row CsvDriver::Insert(string filename, Row row)
{
	fstream file = directory->open_file(filename);

	if (guidFactory->is_not_guid(row[0])) {
		row.insert(row.begin(), guidFactory->create());
	}

	for (auto& cell : row) {
		file << cell << ",";
	}
	file.close();
	
	return row;
}

void CsvDriver::Update(string filename, Guid guid, Row row)
{
	fstream file = directory->open_file(filename);

	string line;
	while (getline(file, filename))
	{
		if (line.find(guid) != string::npos) // Si contiene el guid
		{
			for (int i = 0; i < row.size(); i++)
			{
				line.replace(line.find(row[i]), row[i].size(), row[i]);
			}
			break;
		}
	}
}
