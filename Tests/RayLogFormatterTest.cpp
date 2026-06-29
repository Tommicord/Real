import <gtest/gtest.h>;

import Rl.RayLog.Formatter;

using namespace Rl::RayLog;

class RayLogFormatterTest : public ::testing::Test
{
};

TEST_F(RayLogFormatterTest, FormatInt)
{
  EXPECT_EQ(RayLogFormatter::FormatInt(42), "42");
  EXPECT_EQ(RayLogFormatter::FormatInt(-123), "-123");
  EXPECT_EQ(RayLogFormatter::FormatInt(0), "0");
}

TEST_F(RayLogFormatterTest, FormatFloat)
{
  EXPECT_EQ(RayLogFormatter::FormatFloat(3.14159f, 2), "3.14");
  EXPECT_EQ(RayLogFormatter::FormatFloat(1.0f, 4), "1.0000");
  EXPECT_EQ(RayLogFormatter::FormatFloat(-2.5f, 1), "-2.5");
}

TEST_F(RayLogFormatterTest, FormatString)
{
  EXPECT_EQ(RayLogFormatter::FormatString("hello"), "hello");
  EXPECT_EQ(RayLogFormatter::FormatString(""), "");
}

TEST_F(RayLogFormatterTest, FormatHex)
{
  EXPECT_EQ(RayLogFormatter::FormatHex(255), "0xff");
  EXPECT_EQ(RayLogFormatter::FormatHex(0), "0x0");
  EXPECT_EQ(RayLogFormatter::FormatHex(16), "0x10");
}

TEST_F(RayLogFormatterTest, FormatPtr)
{
  int value = 42;
  void* ptr = &value;
  std::string result = RayLogFormatter::FormatPtr(ptr);
  EXPECT_TRUE(result.starts_with("0x"));
  EXPECT_GT(result.length(), 2);
}

TEST_F(RayLogFormatterTest, FormatBool)
{
  EXPECT_EQ(RayLogFormatter::FormatBool(true), "true");
  EXPECT_EQ(RayLogFormatter::FormatBool(false), "false");
}

TEST_F(RayLogFormatterTest, FormatIntArray)
{
  std::vector<int> arr = {1, 2, 3};
  EXPECT_EQ(RayLogFormatter::FormatArray(arr), "[1, 2, 3]");
}

TEST_F(RayLogFormatterTest, FormatFloatArray)
{
  std::vector<float> arr = {1.5f, 2.5f, 3.5f};
  EXPECT_EQ(RayLogFormatter::FormatArray(arr), "[1.5, 2.5, 3.5]");
}

TEST_F(RayLogFormatterTest, FormatStringArray)
{
  std::vector<std::string> arr = {"a", "b", "c"};
  EXPECT_EQ(RayLogFormatter::FormatArray(arr), "[\"a\", \"b\", \"c\"]");
}

TEST_F(RayLogFormatterTest, FormatEmptyArray)
{
  std::vector<int> arr;
  EXPECT_EQ(RayLogFormatter::FormatArray(arr), "[]");
}

TEST_F(RayLogFormatterTest, FormatSingleElementArray)
{
  std::vector<int> arr = {42};
  EXPECT_EQ(RayLogFormatter::FormatArray(arr), "[42]");
}
