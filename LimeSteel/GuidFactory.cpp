#include "GuidFactory.h"

Guid GuidFactory::create()
{
	string guid = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx";
	for (int i = 0; i < guid.size(); i++)
	{
		auto& c = guid[i];
		if (c == 'x')
		{
			c = rand() % 16;
			c = c < 10 ? '0' + c : 'a' + c - 10;
		}
		else if (c == 'y')
		{
			c = rand() % 4;
			c = '8' + c;
		}
	}
	return guid;
}

bool GuidFactory::is_guid(Guid guid)
{
	auto isSize = guid.size() == 36;
	if(!isSize) return false;
	auto isDash = guid[8] == '-' && guid[13] == '-' && guid[18] == '-' && guid[23] == '-';

	return isDash;
}

bool GuidFactory::is_not_guid(Guid guid)
{
	return !is_guid(guid);
}

