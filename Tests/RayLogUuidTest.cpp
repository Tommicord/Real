import <gtest/gtest.h>;

import Rl.RayLog.Uuid;

using namespace Rl::RayLog;

class RayLogUuidTest : public ::testing::Test
{
  protected:
  void SetUp() override
  {
    uuid = new RayLogUuid();
  }

  void TearDown() override
  {
    delete uuid;
  }

  RayLogUuid* uuid;
};

TEST_F(RayLogUuidTest, ToStringReturnsValidFormat)
{
  std::string uuidStr = uuid->ToString();
  
  EXPECT_EQ(uuidStr.length(), 36);
  EXPECT_EQ(uuidStr[8], '-');
  EXPECT_EQ(uuidStr[13], '-');
  EXPECT_EQ(uuidStr[18], '-');
  EXPECT_EQ(uuidStr[23], '-');
}

TEST_F(RayLogUuidTest, ToStringContainsHexChars)
{
  std::string uuidStr = uuid->ToString();
  
  for (size_t i = 0; i < uuidStr.length(); ++i)
  {
    if (uuidStr[i] != '-')
    {
      EXPECT_TRUE(std::isxdigit(uuidStr[i]));
    }
  }
}

TEST(RayLogUuidEdgeCases, UniqueUuids)
{
  RayLogUuid uuid1;
  RayLogUuid uuid2;
  
  EXPECT_NE(uuid1.ToString(), uuid2.ToString());
}

TEST(RayLogUuidEdgeCases, Version4Variant)
{
  RayLogUuid uuid;
  std::string uuidStr = uuid.ToString();
  
  EXPECT_EQ(uuidStr[14], '4');
  char variantChar = uuidStr[19];
  EXPECT_TRUE(variantChar == '8' || variantChar == '9' || 
              variantChar == 'a' || variantChar == 'b');
}
