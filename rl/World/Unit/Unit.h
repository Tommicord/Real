#pragma once

#include <algorithm>
#include <array>
#include <memory>

#include "rl/World/Unit/UnitDynamicTexture.h"
#include "rl/World/Unit/UnitResourceName.h"
#include "rl/World/Unit/UnitRegistry.h"
#include "rl/Base/Texture2.h"

namespace Rl::World {

using namespace Rl::Providers;

enum class UnitType
{
    Visible,    // Unit Visible
    NoVisible,  // Unit No Visible
    Solid,      // Is Unit Solid
    Liquid,     // Is Unit Solid
};

struct UnitTextureMaterial
{
    std::unique_ptr<Texture2>
             top,
             down,
             left, right,
             front, back;
    ~UnitTextureMaterial();
};

class AbstractUnit
{
    /* Internal Field: Stores the count of registered world units */
    using Registry = UnitRegistryKVPair<UnitResourceName, AbstractUnit*>;
    inline static auto
        defaultName = std::vector( { "Unknown" } );
    inline static auto
        registry = Registry(UnitResourceName(defaultName));
public:
    /* Stores the properties of the world unit */
    struct
    {
        bool isSolid;
        bool isVisible;
    } props;

    /* Creates a basic WorldUnit, automatically registers the unit */
    template<typename T>
        requires(std::is_base_of_v<AbstractUnit, std::decay_t<T>>)
    AbstractUnit(T* type) noexcept : AbstractUnit()
    {
        using pair = UnitRegistryKVPair<UnitResourceName, AbstractUnit*>;
        int id = 1;
        if (pair::GetObjectById(id)
            .has_value())
        {
            while (
                pair::GetObjectById(id)
                    .has_value()
                )
            {
                id++;
            }
        }
        if (!
            pair::GetObjectById(id)
                .has_value())
        {
            std::vector<const char*> v;
            v.reserve(1);
            v.push_back(typeid(T).name());
            UnitResourceName resourceName(v);
            AbstractUnit* base = type;
            registry.Register(id, resourceName, base);
        };
    }

    /* Delete a world unit */
    virtual ~AbstractUnit();

    struct PolFence
    {
        float t, d, b, f; // Top, Down, Back, Front
    };

    /* Sets the resistance against TNT of the unit */
    virtual void SetResistance(float resistance);

    /* Sets the light quantity that emits the unit */
    virtual void SetLightEmit(float emit);

    /* Sets the light quantity substraction for going the unit */
    virtual void SetLightOpacity(float opacity);

    /* Sets the unit hardness, how many times wait to break the unit */
    virtual void SetUnitHardness(float resistance);

    /* Sets the right Polygon Fence, the polygons for the rendering of the Unit */
    virtual void SetPolFenceRight(PolFence& fence);

    /* Sets the left Polygon Fence, the polygons for the rendering of the Unit */
    virtual void SetPolFenceLeft(PolFence& fence);

    /* Enables the collision of this unit */
    virtual void EnableCollision();

    /* Disable the collision of this unit */
    virtual void DisableCollision();

    /* Returns true if the collision is enabled for this unit */
    virtual bool IsCollisionEnabled();

    /* Returns true if the unit is visible */
    virtual bool IsVisible();
protected:
    /* Texture of the unit, back, front, left, right, bottom, top */
    std::unique_ptr<UnitTextureMaterial> textures;

    /* Width, Height, Depth of the unit */
    float inWidth, inHeight, inDepth;

    /* Amount of light emitted */
    float lightEmit;

    /* How much light is subtracted for going through this block */
    float lightOpacity;

    /* Indicates how many hits it takes to break a unit */
    float unitHardness;

    /*
     * The T, D, B, F means the direction
     * T = Top, D = Down, B = Back, F = Front
     * The r suffix means right
     *
     * Store Polygons for non-fixed Grid World */
    float polTr, polDr, polBr, polFr;

    /*
     * The T, D, B, F means the direction
     * T = Top, D = Down, B = Back, F = Front
     * The l suffix means right
     *
     * Store Polygons for non-fixed Grid World */
    float polTl, polDl, polBl, polFl;

    /* Property to enable collision */
    bool mustCollide;

    /* Property to make the unit visible */
    bool mustVisible;

    /* Indicates how much this unit can resist explosions */
    float unitResistance;

    /* Indicates if the unit is translucent */
    bool translucent;
private:
    AbstractUnit() = default;
};

} // namespace Rl::World