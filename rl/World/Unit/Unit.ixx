export module Rl.World.Unit;

import Rl.Base.IUpdatable;
import Rl.Base.Texture2;
import Rl.World.Unit.UnitRegister;
import Rl.World.Unit.UnitDynamicTexture;
import Rl.World.Unit.UnitPropertyStrategy;
import Rl.World.Unit.UnitRegistry;
import Rl.World.Unit.UnitResourceName;

import <algorithm>;
import <string_view>;
import <array>;
import <memory>;
import <vector>;
import <cstring>;

namespace Rl::World
{

using namespace Rl::Providers;

export enum class UnitType {
  VISIBLE, // Unit Visible
  NVISIBLE, // Unit No Visible
  SOLID, // Is Unit Solid
  LIQUID, // Is Unit Solid
};

export class UnitTextureMaterial
{
  /* Stores if all fields are initialized */
  bool hasMaterial = false;

  public:
  Texture2 *top, *down, *left, *right, *front, *back;
  UnitTextureMaterial() = default;
  UnitTextureMaterial(Texture2* top,
      Texture2*                 down,
      Texture2*                 left,
      Texture2*                 right,
      Texture2*                 front,
      Texture2*                 back);
  ~UnitTextureMaterial();
};

// Forward reference to default block
class UnitAir;

export template <class Derived = void> class IUnit : public IUpdatable
{
  /* The registry value type */
  using RegistryV = IUnit*;

  /* Internal Field: Stores the count of registered world units */
  using Registry = UnitRegistryPair3<UnitResourceName, RegistryV>;
  inline static std::vector<std::string_view> defaultName = {std::string_view("Unknown")};
  inline static auto registry = Registry(UnitResourceName(defaultName));

  public:
  /* Stores the properties of the world unit */
  struct
  {
    bool isSolid;
    bool isVisible;
  } props;

  /* Creates a basic WorldUnit, automatically registers the unit */
  explicit IUnit(unsigned short id) noexcept : IUnit()
  {
    RegisterDerived(id);

    static Texture2 texture("rl.unit.Unknown");

    textures = std::make_unique<UnitTextureMaterial>();
    textures->front = textures->back = &texture;
    textures->left = textures->right = &texture;
    textures->top = textures->down = &texture;
  }
  IUnit(const IUnit& other) = delete;
  IUnit(const IUnit&& other) = delete;
  IUnit& operator=(const IUnit& other) = delete;

  /* Delete a world unit */
  ~IUnit() override;

  struct PolFence
  {
    float t, d, b, f; // Top, Down, Back, Front
  };
  [[nodiscard]]
  UnitTextureMaterial& GetMaterial() const;

  /* Sets the resistance against TNT of the unit */
  void SetResistance(float resistance);

  /* Sets the light quantity that emits the unit */
  void SetLightEmit(float emit);

  /* Sets the light quantity substraction for going the unit */
  void SetLightOpacity(float opacity);

  /* Sets the unit hardness, how many times wait to break the unit */
  void SetUnitHardness(float hardness);

  /* Sets the right Polygon Fence, the polygons for the rendering of the Unit */
  void SetPolFenceRight(const PolFence& fence);

  /* Sets the left Polygon Fence, the polygons for the rendering of the Unit */
  void SetPolFenceLeft(const PolFence& fence);

  /* Sets the Polygon curve */
  void SetPolCurve(float curve);

  /* Enables the collision of this unit */
  void EnableCollision();

  /* Disable the collision of this unit */
  void DisableCollision();

  /* Returns true if the collision is enabled for this unit */
  [[nodiscard]]
  bool IsCollisionEnabled() const;

  /* Returns true if the unit is visible */
  [[nodiscard]]
  bool IsVisible() const;

  /* Updates the Base Unit properties */
  void Update() override;

  /* Gets the light emission from property strategy */
  [[nodiscard]]
  virtual float GetStrategyLightEmit() const
  {
    return 0.0f;
  }

  /* Gets the light opacity from property strategy */
  [[nodiscard]]
  virtual float GetStrategyLightOpacity() const
  {
    return 1.0f;
  }

  /* Gets the ambient occlusion from property strategy */
  [[nodiscard]]
  virtual float GetStrategyAmbientOcclusion() const
  {
    return 1.0f;
  }

  /* Gets the roughness from property strategy */
  [[nodiscard]]
  virtual float GetStrategyRoughness() const
  {
    return 0.5f;
  }

  /* Gets the metallic from property strategy */
  [[nodiscard]]
  virtual float GetStrategyMetallic() const
  {
    return 0.0f;
  }

  /* Gets the albedo R from property strategy */
  [[nodiscard]]
  virtual float GetStrategyAlbedoR() const
  {
    return 1.0f;
  }

  /* Gets the albedo G from property strategy */
  [[nodiscard]]
  virtual float GetStrategyAlbedoG() const
  {
    return 1.0f;
  }

  /* Gets the albedo B from property strategy */
  [[nodiscard]]
  virtual float GetStrategyAlbedoB() const
  {
    return 1.0f;
  }

  /* Gets the dirtiness from property strategy */
  [[nodiscard]]
  virtual float GetStrategyDirtiness() const
  {
    return 0.0f;
  }

  /* Gets the wetness from property strategy */
  [[nodiscard]]
  virtual float GetStrategyWetness() const
  {
    return 0.0f;
  }

  /* Gets the temperature from property strategy */
  [[nodiscard]]
  virtual float GetStrategyTemperature() const
  {
    return 20.0f;
  }

  /* Gets the hardness from property strategy */
  [[nodiscard]]
  virtual float GetStrategyHardness() const
  {
    return 1.0f;
  }

  /* Gets the explosion resistance from property strategy */
  [[nodiscard]]
  virtual float GetStrategyExplosionResistance() const
  {
    return 0.0f;
  }

  /* Gets the transparency from property strategy */
  [[nodiscard]]
  virtual float GetStrategyTransparency() const
  {
    return 0.0f;
  }

  /* Gets the flammability from property strategy */
  [[nodiscard]]
  virtual float GetStrategyFlammability() const
  {
    return 0.0f;
  }

  /* Checks if unit is liquid from property strategy */
  [[nodiscard]]
  virtual bool IsStrategyLiquid() const
  {
    return false;
  }

  /* Checks if unit is gas from property strategy */
  [[nodiscard]]
  virtual bool IsStrategyGas() const
  {
    return false;
  }

  /* Checks if unit is solid from property strategy */
  [[nodiscard]]
  virtual bool IsStrategySolid() const
  {
    return true;
  }

  protected:
  /* Registers in compile-time a Unit id into the registry */
  static void RegisterDerived(unsigned short& id)
  {
    using P = UnitRegistryPair3<UnitResourceName, IUnit*>;
    RegisterDerivedInRegistry(id);
  }

  /* Registers in a Unit id, name, and constant value into the registry */
  static void RegisterDerivedInRegistry(unsigned short& id)
  {
    std::vector<std::string_view> v;
    v.push_back(IUnitIdentifiable<Derived>::SimpleClassName());
    UnitResourceName resourceName(v);
    auto*            base = static_cast<Derived*>(new IUnit());
    auto             ref = static_cast<RegistryV>(base);
    registry.Register(id, resourceName, ref);
  }

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

  /* Indicates the vertical curvature of the unit polygons, negative or positive */
  float polCurveV;

  /* Indicates the horizontal curvature of the unit polygons, negative or positive */
  float polCurveH;

  /* Property to enable collision */
  bool mustCollide;

  /* Property to make the unit visible */
  bool mustVisible;

  /* Indicates how much this unit can resist explosions */
  float unitResistance;

  /* Indicates if the unit is translucent */
  bool translucent;

  private:
  /* Private IUnit constructor, only to init the fields */
  IUnit() = default;
};

// Template implementations for IUnit
template <class Derived> IUnit<Derived>::~IUnit()
{
  textures.reset();
}

template <class Derived> UnitTextureMaterial& IUnit<Derived>::GetMaterial() const
{
  return *textures;
}

template <class Derived> void IUnit<Derived>::SetResistance(const float resistance)
{
  this->unitResistance = resistance;
}

template <class Derived> void IUnit<Derived>::SetLightEmit(const float emit)
{
  this->lightEmit = emit;
}

template <class Derived> void IUnit<Derived>::SetLightOpacity(const float opacity)
{
  this->lightOpacity = opacity;
}

template <class Derived> void IUnit<Derived>::SetUnitHardness(const float hardness)
{
  this->unitHardness = hardness;
}

template <class Derived> void IUnit<Derived>::SetPolFenceRight(const PolFence& fence)
{
  std::memcpy(&polTr, &fence, sizeof(fence));
}

template <class Derived> void IUnit<Derived>::SetPolFenceLeft(const PolFence& fence)
{
  std::memcpy(&polTl, &fence, sizeof(fence));
}

template <class Derived> void IUnit<Derived>::SetPolCurve(const float curve)
{
  this->polCurveV = curve;
}

template <class Derived> void IUnit<Derived>::EnableCollision()
{
  mustCollide = true;
}

template <class Derived> void IUnit<Derived>::DisableCollision()
{
  mustCollide = false;
}

template <class Derived> bool IUnit<Derived>::IsCollisionEnabled() const
{
  return mustCollide;
}

template <class Derived> bool IUnit<Derived>::IsVisible() const
{
  return mustVisible;
}

template <class Derived> void IUnit<Derived>::Update()
{
}

} // namespace Rl::World
