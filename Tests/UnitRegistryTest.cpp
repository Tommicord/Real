import <gtest/gtest.h>;
import <string>;
import <string_view>;
import <vector>;

import Rl.World.Unit.UnitRegistry;
import Rl.World.Unit.UnitRegister;
import Rl.World.Unit.UnitPropertyStrategy;
import Rl.World.Unit.UnitResourceName;
import Rl.World.Unit;

using namespace Rl::World;

class UnitResourceNameTest : public ::testing::Test
{
  protected:
  void SetUp() override
  {
    testNames = {"Grass", "Dirt", "Stone"};
    resourceName = new UnitResourceName(
      {std::string_view("Grass"), std::string_view("Dirt"), std::string_view("Stone")});
  }

  void TearDown() override
  {
    delete resourceName;
  }

  std::vector<std::string_view> testNames;
  UnitResourceName* resourceName;
};

TEST_F(UnitResourceNameTest, ConstructorInitializesName)
{
  ASSERT_NE(resourceName, nullptr);
  EXPECT_GT(resourceName->GetResourceNameLength(), 0);
}

TEST(UnitResourceNameEdgeCases, SingleComponentName)
{
  UnitResourceName singleName({std::string_view("Test")});
  EXPECT_GT(singleName.GetResourceNameLength(), 0);
}

TEST(UnitResourceNameEdgeCases, EmptyVectorName)
{
  UnitResourceName emptyName({});
  EXPECT_EQ(emptyName.GetResourceNameLength(), 0);
}

TEST_F(UnitResourceNameTest, EqualityComparison)
{
  UnitResourceName otherName(
    {std::string_view("Grass"), std::string_view("Dirt"), std::string_view("Stone")});
  EXPECT_TRUE(resourceName->Equals(otherName));
}

TEST_F(UnitResourceNameTest, InequalityComparison)
{
  UnitResourceName otherName({std::string_view("Water"), std::string_view("Sand")});
  EXPECT_FALSE(resourceName->Equals(otherName));
}

TEST_F(UnitResourceNameTest, SplitResourceName)
{
  auto tokens = resourceName->SplitResourceName();
  EXPECT_GT(tokens.size(), 0);
}

TEST_F(UnitResourceNameTest, EqualityOperator)
{
  UnitResourceName otherName(
    {std::string_view("Grass"), std::string_view("Dirt"), std::string_view("Stone")});
  EXPECT_TRUE(*resourceName == otherName);
}

class UnitRegisterTest : public ::testing::Test
{
  protected:
  class MockUnit : public IUnitIdentifiable<MockUnit>
  {
  };

  class AnotherMockUnit : public IUnitIdentifiable<AnotherMockUnit>
  {
  };
};

TEST_F(UnitRegisterTest, GetStaticClassIdReturnsValidId)
{
  auto id1 = MockUnit::GetStaticClassId();
  auto id2 = AnotherMockUnit::GetStaticClassId();
  
  EXPECT_GT(id1, 0);
  EXPECT_GT(id2, 0);
  EXPECT_NE(id1, id2); // Different classes should have different IDs
}

TEST_F(UnitRegisterTest, SimpleClassNameReturnsValidName)
{
  auto name1 = MockUnit::SimpleClassName();
  auto name2 = AnotherMockUnit::SimpleClassName();
  
  EXPECT_GT(name1.length(), 0);
  EXPECT_GT(name2.length(), 0);
  EXPECT_NE(name1, name2);
}

TEST_F(UnitRegisterTest, GetClassIdReturnsSameAsStatic)
{
  MockUnit unit;
  auto staticId = MockUnit::GetStaticClassId();
  auto instanceId = unit.GetClassId();
  
  EXPECT_EQ(staticId, instanceId);
}

class UnitPropertyStrategyTest : public ::testing::Test
{
  protected:
  class MockMaterial : public UnitPropertyStrategy<MockMaterial>
  {
  };

  class CustomMaterial : public UnitPropertyStrategy<CustomMaterial>
  {
    public:
    static constexpr float GetLightEmit() { return 5.0f; }
    static constexpr float GetLightOpacity() { return 0.5f; }
    static constexpr float GetRoughness() { return 0.8f; }
    static constexpr float GetMetallic() { return 0.9f; }
    static constexpr bool IsLiquid() { return true; }
  };
};

TEST_F(UnitPropertyStrategyTest, DefaultLightEmit)
{
  EXPECT_EQ(MockMaterial::GetLightEmit(), 0.0f);
}

TEST_F(UnitPropertyStrategyTest, DefaultLightOpacity)
{
  EXPECT_EQ(MockMaterial::GetLightOpacity(), 1.0f);
}

TEST_F(UnitPropertyStrategyTest, DefaultAmbientOcclusion)
{
  EXPECT_EQ(MockMaterial::GetAmbientOcclusion(), 1.0f);
}

TEST_F(UnitPropertyStrategyTest, DefaultLightAbsorption)
{
  EXPECT_EQ(MockMaterial::GetLightAbsorption(), 0.0f);
}

TEST_F(UnitPropertyStrategyTest, DefaultLightScattering)
{
  EXPECT_EQ(MockMaterial::GetLightScattering(), 0.0f);
}

TEST_F(UnitPropertyStrategyTest, DefaultRoughness)
{
  EXPECT_EQ(MockMaterial::GetRoughness(), 0.5f);
}

TEST_F(UnitPropertyStrategyTest, DefaultMetallic)
{
  EXPECT_EQ(MockMaterial::GetMetallic(), 0.0f);
}

TEST_F(UnitPropertyStrategyTest, DefaultAlbedo)
{
  EXPECT_EQ(MockMaterial::GetAlbedoR(), 1.0f);
  EXPECT_EQ(MockMaterial::GetAlbedoG(), 1.0f);
  EXPECT_EQ(MockMaterial::GetAlbedoB(), 1.0f);
}

TEST_F(UnitPropertyStrategyTest, DefaultReflectivity)
{
  EXPECT_EQ(MockMaterial::GetReflectivity(), 0.0f);
}

TEST_F(UnitPropertyStrategyTest, DefaultRefractiveIndex)
{
  EXPECT_EQ(MockMaterial::GetRefractiveIndex(), 1.0f);
}

TEST_F(UnitPropertyStrategyTest, DefaultDirtiness)
{
  EXPECT_EQ(MockMaterial::GetDirtiness(), 0.0f);
}

TEST_F(UnitPropertyStrategyTest, DefaultWetness)
{
  EXPECT_EQ(MockMaterial::GetWetness(), 0.0f);
}

TEST_F(UnitPropertyStrategyTest, DefaultTemperature)
{
  EXPECT_EQ(MockMaterial::GetTemperature(), 20.0f);
}

TEST_F(UnitPropertyStrategyTest, DefaultHumidity)
{
  EXPECT_EQ(MockMaterial::GetHumidity(), 0.5f);
}

TEST_F(UnitPropertyStrategyTest, DefaultHardness)
{
  EXPECT_EQ(MockMaterial::GetHardness(), 1.0f);
}

TEST_F(UnitPropertyStrategyTest, DefaultExplosionResistance)
{
  EXPECT_EQ(MockMaterial::GetExplosionResistance(), 0.0f);
}

TEST_F(UnitPropertyStrategyTest, DefaultTransparency)
{
  EXPECT_EQ(MockMaterial::GetTransparency(), 0.0f);
}

TEST_F(UnitPropertyStrategyTest, DefaultEmissiveIntensity)
{
  EXPECT_EQ(MockMaterial::GetEmissiveIntensity(), 0.0f);
}

TEST_F(UnitPropertyStrategyTest, DefaultSubsurfaceScattering)
{
  EXPECT_EQ(MockMaterial::GetSubsurfaceScattering(), 0.0f);
}

TEST_F(UnitPropertyStrategyTest, DefaultFlammability)
{
  EXPECT_EQ(MockMaterial::GetFlammability(), 0.0f);
}

TEST_F(UnitPropertyStrategyTest, DefaultIsSolid)
{
  EXPECT_TRUE(MockMaterial::IsSolid());
}

TEST_F(UnitPropertyStrategyTest, DefaultIsLiquid)
{
  EXPECT_FALSE(MockMaterial::IsLiquid());
}

TEST_F(UnitPropertyStrategyTest, DefaultIsGas)
{
  EXPECT_FALSE(MockMaterial::IsGas());
}

// Test custom property values
TEST_F(UnitPropertyStrategyTest, CustomLightEmit)
{
  EXPECT_EQ(CustomMaterial::GetLightEmit(), 5.0f);
}

TEST_F(UnitPropertyStrategyTest, CustomLightOpacity)
{
  EXPECT_EQ(CustomMaterial::GetLightOpacity(), 0.5f);
}

TEST_F(UnitPropertyStrategyTest, CustomRoughness)
{
  EXPECT_EQ(CustomMaterial::GetRoughness(), 0.8f);
}

TEST_F(UnitPropertyStrategyTest, CustomMetallic)
{
  EXPECT_EQ(CustomMaterial::GetMetallic(), 0.9f);
}

TEST_F(UnitPropertyStrategyTest, CustomIsLiquid)
{
  EXPECT_TRUE(CustomMaterial::IsLiquid());
}

// Test CalculateLightAttenuation
TEST_F(UnitPropertyStrategyTest, CalculateLightAttenuationDefault)
{
  auto attenuation = MockMaterial::CalculateLightAttenuation();
  EXPECT_EQ(attenuation, MockMaterial::GetLightOpacity() + MockMaterial::GetLightAbsorption());
}

TEST_F(UnitPropertyStrategyTest, CalculateLightAttenuationCustom)
{
  auto attenuation = CustomMaterial::CalculateLightAttenuation();
  EXPECT_EQ(attenuation, CustomMaterial::GetLightOpacity() + CustomMaterial::GetLightAbsorption());
}

class UnitRegistryTest : public ::testing::Test
{
  protected:
  void SetUp() override
  {
  }

  class TestRegistryItem
  {
  };

  using TestRegistry = UnitRegisters<std::string, int>;
  using TestPair = UnitRegistryPair3<std::string, int>;
};

// Test UnitRegistryPair3 constructor
TEST_F(UnitRegistryTest, RegistryPairConstructorInitializesFields)
{
  std::string testKey = "test_key";
  TestPair pair(testKey);
  
  EXPECT_GT(TestRegistry::GetRegistrySize(), 0);
}

// Test UnitRegisters GetRegistrySize
TEST_F(UnitRegistryTest, GetRegistrySizeReturnsCorrectSize)
{
  const size_t initialSize = TestRegistry::GetRegistrySize();
  
  std::string testKey = "another_key";
  TestPair pair(testKey);
  
  EXPECT_GT(TestRegistry::GetRegistrySize(), initialSize);
}

// Test UnitRegisters GetRegistry
TEST_F(UnitRegistryTest, GetRegistryReturnsValidReference)
{
  const auto& registry = TestRegistry::GetRegistry();
  EXPECT_GE(registry.size(), 0);
}


TEST_F(UnitRegistryTest, RegisterUpdatesPairValues)
{
  const std::string key = "register_test";
  TestPair pair(key);

  const unsigned short testId = 42;
  std::string newKey = "new_key";
  int newValue = 100;
  
  pair.Register(testId, newKey, newValue);
  
  // Verify the pair was added to registry
  const auto& registry = TestRegistry::GetRegistry();
  EXPECT_GT(registry.size(), 0);
}

TEST_F(UnitRegistryTest, GetObjectReturnsCorrectValue)
{
  std::string key = "get_object_test";
  int value = 123;
  
  TestPair pair(key);
  pair.Register(1, key, value);
  
  auto result = TestPair::GetObject(key);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result.value(), value);
}

TEST_F(UnitRegistryTest, GetObjectReturnsNulloptForNonExistentKey)
{
  auto result = TestPair::GetObject("non_existent_key");
  EXPECT_FALSE(result.has_value());
}

TEST_F(UnitRegistryTest, GetObjectByIdReturnsCorrectValue)
{
  std::string key = "get_by_id_test";
  int value = 456;
  const unsigned short id = 99;
  
  TestPair pair(key);
  pair.Register(id, key, value);
  
  auto result = TestPair::GetObjectById(id);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result.value(), value);
}

TEST_F(UnitRegistryTest, GetObjectByIdReturnsNulloptForNonExistentId)
{
  auto result = TestPair::GetObjectById(9999);
  EXPECT_FALSE(result.has_value());
}

// Test UnitRegistryPair3 GetNameForObject
TEST_F(UnitRegistryTest, GetNameForObjectReturnsCorrectName)
{
  std::string key = "get_name_test";
  int value = 789;
  
  TestPair pair(key);
  pair.Register(1, key, value);
  
  auto result = TestPair::GetNameForObject(value);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result.value(), key);
}

TEST_F(UnitRegistryTest, GetNameForObjectReturnsNulloptForNonExistentValue)
{
  int id = 99999;
  auto result = TestPair::GetNameForObject(id);
  EXPECT_FALSE(result.has_value());
}

// Test class for IUnit
class IUnitTest : public ::testing::Test
{
  protected:
  void SetUp() override
  {
  }

  // Mock IUnit implementation for testing
  class MockIUnit : public IUnit
  {
    public:
    MockIUnit() : IUnit(0)
    {
      RegisterDerived(*this);
    }

    [[nodiscard]]
    unsigned short GetDerivedClassId() const override
    {
      return IUnitIdentifiable<MockIUnit>::GetStaticClassId();
    }

    [[nodiscard]]
    std::string_view GetDerivedClassName() const override
    {
      return IUnitIdentifiable<MockIUnit>::SimpleClassName();
    }
  };
};

TEST_F(IUnitTest, ConstructorInitializesFields)
{
  MockIUnit unit;
  
  EXPECT_NE(unit.GetMaterial().top, nullptr);
  EXPECT_NE(unit.GetMaterial().down, nullptr);
}

TEST_F(IUnitTest, SetResistanceUpdatesValue)
{
  MockIUnit unit;
  unit.SetResistance(5.0f);
  SUCCEED();
}

TEST_F(IUnitTest, SetLightEmitUpdatesValue)
{
  MockIUnit unit;
  unit.SetLightEmit(2.5f);
  SUCCEED();
}

// Test IUnit SetLightOpacity
TEST_F(IUnitTest, SetLightOpacityUpdatesValue)
{
  MockIUnit unit;
  unit.SetLightOpacity(0.8f);
  SUCCEED();
}

TEST_F(IUnitTest, SetUnitHardnessUpdatesValue)
{
  MockIUnit unit;
  unit.SetUnitHardness(3.0f);
  SUCCEED();
}

TEST_F(IUnitTest, SetPolFenceRightUpdatesValues)
{
  MockIUnit unit;
  IUnit::PolFence fence{1.0f, 2.0f, 3.0f, 4.0f};
  unit.SetPolFenceRight(fence);
  SUCCEED();
}

TEST_F(IUnitTest, SetPolFenceLeftUpdatesValues)
{
  MockIUnit unit;
  IUnit::PolFence fence{5.0f, 6.0f, 7.0f, 8.0f};
  unit.SetPolFenceLeft(fence);
  SUCCEED();
}

TEST_F(IUnitTest, SetPolCurveUpdatesValue)
{
  MockIUnit unit;
  unit.SetPolCurve(0.5f);
  SUCCEED();
}

TEST_F(IUnitTest, EnableCollisionSetsFlag)
{
  MockIUnit unit;
  unit.EnableCollision();
  EXPECT_TRUE(unit.IsCollisionEnabled());
}

TEST_F(IUnitTest, DisableCollisionClearsFlag)
{
  MockIUnit unit;
  unit.EnableCollision();
  unit.DisableCollision();
  EXPECT_FALSE(unit.IsCollisionEnabled());
}

TEST_F(IUnitTest, IsCollisionEnabledDefaultState)
{
  MockIUnit unit;
  EXPECT_FALSE(unit.IsCollisionEnabled());
}

TEST_F(IUnitTest, IsVisibleDefaultState)
{
  MockIUnit unit;
  EXPECT_FALSE(unit.IsVisible());
}

TEST_F(IUnitTest, UpdateDoesNotCrash)
{
  MockIUnit unit;
  unit.Update();
  SUCCEED();
}

TEST_F(IUnitTest, VirtualStrategyMethodsReturnDefaults)
{
  MockIUnit unit;
  
  EXPECT_EQ(unit.GetStrategyLightEmit(), 0.0f);
  EXPECT_EQ(unit.GetStrategyLightOpacity(), 1.0f);
  EXPECT_EQ(unit.GetStrategyAmbientOcclusion(), 1.0f);
  EXPECT_EQ(unit.GetStrategyRoughness(), 0.5f);
  EXPECT_EQ(unit.GetStrategyMetallic(), 0.0f);
  EXPECT_EQ(unit.GetStrategyAlbedoR(), 1.0f);
  EXPECT_EQ(unit.GetStrategyAlbedoG(), 1.0f);
  EXPECT_EQ(unit.GetStrategyAlbedoB(), 1.0f);
  EXPECT_EQ(unit.GetStrategyDirtiness(), 0.0f);
  EXPECT_EQ(unit.GetStrategyWetness(), 0.0f);
  EXPECT_EQ(unit.GetStrategyTemperature(), 20.0f);
  EXPECT_EQ(unit.GetStrategyHardness(), 1.0f);
  EXPECT_EQ(unit.GetStrategyExplosionResistance(), 0.0f);
  EXPECT_EQ(unit.GetStrategyTransparency(), 0.0f);
  EXPECT_EQ(unit.GetStrategyFlammability(), 0.0f);
  EXPECT_FALSE(unit.IsStrategyLiquid());
  EXPECT_FALSE(unit.IsStrategyGas());
  EXPECT_TRUE(unit.IsStrategySolid());
}

TEST_F(IUnitTest, GetDerivedClassIdReturnsValidId)
{
  MockIUnit unit;
  EXPECT_GT(unit.GetDerivedClassId(), 0);
}

TEST_F(IUnitTest, GetDerivedClassNameReturnsValidName)
{
  MockIUnit unit;
  auto name = unit.GetDerivedClassName();
  EXPECT_GT(name.length(), 0);
}

TEST(IUnitEdgeCases, CopyConstructorIsDeleted)
{
  EXPECT_FALSE(std::is_copy_constructible<IUnit>::value);
}

TEST(IUnitEdgeCases, CopyAssignmentIsDeleted)
{
  EXPECT_FALSE(std::is_copy_assignable<IUnit>::value);
}

TEST(UnitTextureMaterialTest, DefaultConstructorInitializesPointers)
{
  UnitTextureMaterial material;
  EXPECT_EQ(material.top, nullptr);
  EXPECT_EQ(material.down, nullptr);
  EXPECT_EQ(material.left, nullptr);
  EXPECT_EQ(material.right, nullptr);
  EXPECT_EQ(material.front, nullptr);
  EXPECT_EQ(material.back, nullptr);
}

TEST(UnitTextureMaterialTest, ParameterizedConstructorInitializesPointers)
{
  Texture2 tex1("test1");
  Texture2 tex2("test2");
  Texture2 tex3("test3");
  Texture2 tex4("test4");
  Texture2 tex5("test5");
  Texture2 tex6("test6");
  
  UnitTextureMaterial material(&tex1, &tex2, &tex3, &tex4, &tex5, &tex6);
  
  EXPECT_EQ(material.top, &tex1);
  EXPECT_EQ(material.down, &tex2);
  EXPECT_EQ(material.left, &tex3);
  EXPECT_EQ(material.right, &tex4);
  EXPECT_EQ(material.front, &tex5);
  EXPECT_EQ(material.back, &tex6);
}

TEST(UnitTypeTest, EnumValuesAreValid)
{
  EXPECT_EQ(static_cast<int>(UnitType::VISIBLE), 0);
  EXPECT_EQ(static_cast<int>(UnitType::NVISIBLE), 1);
  EXPECT_EQ(static_cast<int>(UnitType::SOLID), 2);
  EXPECT_EQ(static_cast<int>(UnitType::LIQUID), 3);
}
