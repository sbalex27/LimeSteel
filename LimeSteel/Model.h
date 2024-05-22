#pragma once
#include "GuidFactory.h"
#include "Primitives.h"

class Model
{
public:
	Guid guid = "";
	bool has_guid() const {
		return guid != "";
	}
	virtual Row to_row() = 0;
};

