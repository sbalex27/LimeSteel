#include "DirectoryService.h"

using std::string;
using std::system;
using std::iostream;
using std::ifstream;
using std::ofstream;

DirectoryService::DirectoryService(string path)
{
	this->path = path;
}

bool DirectoryService::create_directory() const
{
	string command = "mkdir " + string(path);
	auto result = system(command.c_str());
	return result == 0;
}

bool DirectoryService::drop_directory() const
{
	string command = "rmdir /s /q " + string(path);
	auto result = system(command.c_str());
	return result == 0;
}

bool DirectoryService::exists_directory() const
{
	struct stat sb;

	if (stat(path.c_str(), &sb) == 0)
		return true;
	else
		return false;
}

ofstream DirectoryService::create_file(string filename) const
{
	return ofstream(path + "\\" + filename);
}

bool DirectoryService::exists_file(string filename) const
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

bool DirectoryService::drop_file(string filename) const
{
	auto path = this->path + "\\" + filename;
	auto result = remove(path.c_str());
	return result == 0;
}
