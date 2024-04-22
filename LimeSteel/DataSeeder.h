#pragma once

#include "Table.h"
#include <string>

using std::string;
using std::vector;

class DataSeeder
{
public:
	string random_string(int length);
	Row get_random_row(int columns);
};

