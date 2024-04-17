#include "CsvManager.h"

using std::string;
using std::system;
using std::iostream;
using std::ifstream;
using std::ofstream;

CsvManager::CsvManager(string path)
{
	this->path = path;
}

bool CsvManager::create_directory() const
{
	string command = "mkdir " + string(path);
	auto result = system(command.c_str());
	return result == 0;
}

bool CsvManager::drop_directory() const
{
	string command = "rmdir /s /q " + string(path);
	auto result = system(command.c_str());
	return result == 0;
}

bool CsvManager::exists_directory() const
{
	struct stat sb;

	if (stat(path.c_str(), &sb) == 0)
		return true;
	else
		return false;
}

ofstream CsvManager::create_file(string filename) const
{
	return ofstream(path + "\\" + filename);
}

bool CsvManager::exists_file(string filename) const
{
	auto path = this->path + "\\" + filename;
	ifstream file;
	file.open(path);
	if (file.is_open())
	{
		file.close();
		return true;
	}
	else
	{
		return false;
	}
}

bool CsvManager::drop_file(string filename) const
{
	auto path = this->path + "\\" + filename;
	auto result = remove(path.c_str());
	return result == 0;
}
