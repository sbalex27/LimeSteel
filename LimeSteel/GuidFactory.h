#pragma once
#include "Primitives.h"

using std::string;

class GuidFactory
{
public:
	Guid create();
	bool is_guid(Guid guid);
	bool is_not_guid(Guid guid);
};

