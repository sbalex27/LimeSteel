#pragma once
#include <string>

using std::string;

typedef string Guid;

class GuidFactory
{
public:
	Guid create();
	bool is_guid(Guid guid);
	bool is_not_guid(Guid guid);
};

