#include "GuidFactory.h"
#include <string>
#include <random>

Guid GuidFactory::create()
{
    static const std::string characters = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, characters.size() - 1);

    std::string random_string;
    for (size_t i = 0; i < 6; ++i) {
        random_string += characters[dis(gen)];
    }

    return random_string;
}

bool GuidFactory::is_guid(const Guid guid)
{
    if(guid.size() != 6) return false;
}

bool GuidFactory::is_not_guid(const Guid guid)
{
    return !is_guid(guid);
}

