#include "DataSeeder.h"

string DataSeeder::random_string(int length)
{
	char chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	string string = "";

	for (int i = 0; i < length; i++)
	{
		int index = rand() % (sizeof(chars) - 1);
		char c = chars[index];
		string += c;
	}

	return string;
}

Row DataSeeder::get_random_row(int columns)
{
	Row row;
	for (int i = 0; i < columns; i++)
	{
		row.push_back(this->random_string(10));
	}
	return row;
}

vector<Row> DataSeeder::get_random_table(int rows, int columns)
{
	vector<Row> rowsVector;
	for (int i = 0; i < rows; i++)
	{
		rowsVector.push_back(this->get_random_row(columns));
	}
	return rowsVector;
}

