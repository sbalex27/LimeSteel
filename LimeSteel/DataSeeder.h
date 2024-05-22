#pragma once

#include "Primitives.h"
#include <string>

using std::string;
using std::vector;

class DataSeeder
{
public:
	string random_string(int length);
	Row get_random_row(int columns);
	vector<Row> get_random_table(int rows, int columns);
};

