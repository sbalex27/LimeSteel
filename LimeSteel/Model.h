#pragma once
#include "GuidFactory.h"
#include "Primitives.h"
#include "InvalidRowSizeException.h"

class Model
{
public:
	Guid guid = "";
	bool has_guid() const {
		return guid != "";
	}
	bool has_not_guid() const {
		return guid == "";
	}
	virtual Row to_row() = 0;

	void assert_row(Row row, int size) {
		if (row.size() != size) throw InvalidRowSizeException("Row size must be " + size);
	}
};

