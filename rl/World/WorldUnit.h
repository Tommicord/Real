#pragma once

#include <algorithm>
#include <array>
#include <memory>
#include "rl/Base/Texture2.h"

namespace Rl::World {

using namespace Rl::Providers;

enum class WorldUnitType
{
    Visible,
    NoVisible,
    Solid,
    Liquid,
};

template<class K, class V>
class WorldUnitRegistry;

struct WorldUnitTexture2D : Texture2
{
    Texture2 *top,
               down,
               left, right,
               front, back;
    WorldUnitTexture2D();
    ~WorldUnitTexture2D();
};

class AbstractWorldUnit
{
    /* Internal Field: Stores the count of registered world units */
    static WorldUnitRegistry<>& registry_;
public:
    /* Count of Properties */

    /* Stores the properties of the world unit */
    struct
    {
        bool isSolid;
        bool isVisible;
    } props;

    /* Creates a basic WorldUnit, automatically registers the unit */
    AbstractWorldUnit();

    struct PolFence
    {
        float t, d, b, f; // Top, Down, Back, Front
    };

    /* Delete a world unit */
    virtual ~AbstractWorldUnit();

    /* Sets the resistance against TNT of the unit */
    virtual void SetResistance(float resistance);

    /* Sets the light quantity that emits the unit */
    virtual void SetLightEmit(float resistance);

    /* Sets the light quantity substraction for going the unit */
    virtual void SetLightOpacity(float resistance);

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
protected:
    /* Texture of the unit, back, front, left, right, bottom, top */
    std::unique_ptr<WorldUnitTexture2D> texture;

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

    /* Indicates how much this block can resist explosions */
    float unitResistance;
    bool translucent;
};

template<class K, class V>
class WorldUnitRegistry final
{
protected:
    K& regKey;
    V& regValue;
public:
    /* Creates a basic register of world unit */
    WorldUnitRegistry(K defaultRegKey);

    /* Registers a Unit into the registry */
    void Register();

    /* Gets the name we use to identify the object */
    K GetNameForObject(V value);

    /* Gets the object from the name identifier */
    K GetObject(K name);

    /* Gets the object from the id identifier */
    V GetObjectById(int id);
};

}