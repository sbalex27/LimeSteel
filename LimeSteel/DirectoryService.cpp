#include "DirectoryService.h"
#include <fstream>

using std::string;
using std::system;
using std::iostream;
using std::ifstream;
using std::ofstream;
using std::fstream;

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
	auto file = ofstream(path + "\\" + filename);

	if (file.is_open()) {
		return file;
	}
	else {
		throw std::runtime_error("Could not create file");
	}

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

ifstream  DirectoryService::open_read_file(string filename) const {
	ifstream file(path + "\\" + filename);
	return file;
}

ofstream DirectoryService::open_write_to_file(string filename) const
{
	auto full_path = path + "\\" + filename;
	ofstream file(full_path, std::ios::app);

	return file;
}

fstream DirectoryService::open_modify_file(string filename) const
{
	// Abre el archivo en modo de lectura y escritura
	fstream file(path + "\\" + filename, std::ios::in | std::ios::out);

	return file;
}