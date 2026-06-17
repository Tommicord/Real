#pragma once

#include <vector>
#include <cstring>

namespace Rl::World {

class WorldUnitResourceName
{
    static constexpr auto BASE = "rl.world.unit";
protected:
    /* Identifies the unit resource name, for example: rl.world.GrassBlock */
    char *name;
    size_t nameLen;
public:
    /* Creates a basic unit resource name for registry identifiers */
    WorldUnitResourceName(const std::vector<const char*>& name);

    /* Destroys a basic resource name object */
    ~WorldUnitResourceName();

    /* Constructs the resource name from base identifier */
    void ConstructResourceName(const std::vector<const char *>& base, size_t maxSize);

    /* Splits the resource name into smaller tokens */
    std::vector<char *> SplitResourceName() const;

    /* Gets the base string resource name */
    static const char* GetBaseResourceString();

    /* Gets the stored resource name */
    char *GetResourceName() const;

    /* Gets the stored resource name length */
    size_t GetResourceNameLength() const;

    /* Compares the resource name with other resource name */
    bool Equals(const WorldUnitResourceName& resource) const;
};

} // namespace Rl::World
