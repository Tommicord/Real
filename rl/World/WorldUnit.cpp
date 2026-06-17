#include "rl/World/WorldUnit.h"
#include "rl/World/WorldUnitResourceName.h"
#include <cstring>
#include <vector>
#include <memory>
#include <algorithm>

namespace Rl::World {

WorldUnitResourceName::WorldUnitResourceName(const std::vector<const char*>& name)
{
    constexpr int maxSize = 255;
    this->name = new char[maxSize + strnlen(BASE, 32)];
    ConstructResourceName(name, maxSize);
}

WorldUnitResourceName::~WorldUnitResourceName()
{
    delete[] name;
}

void WorldUnitResourceName::ConstructResourceName(const std::vector<const char*>& base,
                                                   const size_t maxSize)
{
    if (!this->name)
        return;
    this->nameLen = 0;
    int count;
    for (int i = 0; i < base.size(); ++i)
    {
        if ((count = std::strlen( base[i] )) > 255)
        {
            count = 255;
        }
        this->nameLen = this->nameLen + count;
        strcpy_s(
            this->name,
            count - 1,
            base[i]);
        this->name[this->nameLen] = '.';
        if (this->nameLen + 1 >= maxSize)
        {
            break;
        }
    }
}
std::vector<char*> WorldUnitResourceName::SplitResourceName() const
{
    std::string nm(name);
    std::vector<char*> res;
    // Reserve space to avoid reallocations
    res.reserve(
        std::ranges::count_if(nm,
            []( const char c )
            {
                return c == '.';
            }
        ) + 1
    );
    for (int i = 0; i < nm.size(); ++i)
    {
        std::string str;
        for (int j = 0; i < nm.size(); ++i)
        {
            if (nm[ i ] == '.')
            {
                res[i] = str.data();
                break;
            }
            str.append( &nm[i] );
        }
    }
    return res;
}

const char* WorldUnitResourceName::GetBaseResourceString()
{
    return BASE;
}

char* WorldUnitResourceName::GetResourceName() const {
    return name;
}

size_t WorldUnitResourceName::GetResourceNameLength() const {
    return nameLen;
}

bool WorldUnitResourceName::Equals(const WorldUnitResourceName& resource) const
{
    if (&resource == this)
        return true;
    if (this->nameLen != resource.nameLen)
        return false;
    return std::memcmp(
        this->name,
        resource.name,
        this->nameLen
    );
}

}