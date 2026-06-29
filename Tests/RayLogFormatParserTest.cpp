import <gtest/gtest.h>;

import Rl.RayLog.FormatParser;

using namespace Rl::RayLog;

class RayLogFormatParserTest : public ::testing::Test
{
};

TEST_F(RayLogFormatParserTest, ParseSimpleString)
{
  auto tokens = RayLogFormatParser::Parse("hello world");
  EXPECT_EQ(tokens.size(), 1);
  EXPECT_FALSE(tokens[0].isSpecifier);
  EXPECT_EQ(tokens[0].value, "hello world");
}

TEST_F(RayLogFormatParserTest, ParseSingleSpecifier)
{
  auto tokens = RayLogFormatParser::Parse("%s");
  EXPECT_EQ(tokens.size(), 1);
  EXPECT_TRUE(tokens[0].isSpecifier);
  EXPECT_EQ(tokens[0].value, "s");
}

TEST_F(RayLogFormatParserTest, ParseMultipleSpecifiers)
{
  auto tokens = RayLogFormatParser::Parse("%d %s %f");
  EXPECT_EQ(tokens.size(), 5);
  EXPECT_TRUE(tokens[0].isSpecifier);
  EXPECT_EQ(tokens[0].value, "d");
  EXPECT_FALSE(tokens[1].isSpecifier);
  EXPECT_EQ(tokens[1].value, " ");
  EXPECT_TRUE(tokens[2].isSpecifier);
  EXPECT_EQ(tokens[2].value, "s");
  EXPECT_FALSE(tokens[3].isSpecifier);
  EXPECT_EQ(tokens[3].value, " ");
  EXPECT_TRUE(tokens[4].isSpecifier);
  EXPECT_EQ(tokens[4].value, "f");
}

TEST_F(RayLogFormatParserTest, ParseMixedFormat)
{
  auto tokens = RayLogFormatParser::Parse("Value: %d, Name: %s");
  EXPECT_EQ(tokens.size(), 4);
  EXPECT_FALSE(tokens[0].isSpecifier);
  EXPECT_EQ(tokens[0].value, "Value: ");
  EXPECT_TRUE(tokens[1].isSpecifier);
  EXPECT_EQ(tokens[1].value, "d");
  EXPECT_FALSE(tokens[2].isSpecifier);
  EXPECT_EQ(tokens[2].value, ", Name: ");
  EXPECT_TRUE(tokens[3].isSpecifier);
  EXPECT_EQ(tokens[3].value, "s");
}

TEST_F(RayLogFormatParserTest, ParsePrecisionSpecifier)
{
  auto tokens = RayLogFormatParser::Parse("%f.2");
  EXPECT_EQ(tokens.size(), 1);
  EXPECT_TRUE(tokens[0].isSpecifier);
  EXPECT_EQ(tokens[0].value, "%f.2");
}

TEST_F(RayLogFormatParserTest, ParseComplexPrecision)
{
  auto tokens = RayLogFormatParser::Parse("%f.10");
  EXPECT_EQ(tokens.size(), 1);
  EXPECT_TRUE(tokens[0].isSpecifier);
  EXPECT_EQ(tokens[0].value, "%f.10");
}

TEST_F(RayLogFormatParserTest, ParseAllSpecifiers)
{
  auto tokens = RayLogFormatParser::Parse("%s %d %f %h %p %b %a");
  EXPECT_EQ(tokens.size(), 13);
  
  std::vector<std::string> expectedSpecs = {"s", "d", "f", "h", "p", "b", "a"};
  size_t specIndex = 0;
  for (const auto& token : tokens)
  {
    if (token.isSpecifier)
    {
      EXPECT_EQ(token.value, expectedSpecs[specIndex]);
      specIndex++;
    }
  }
  EXPECT_EQ(specIndex, expectedSpecs.size());
}

TEST_F(RayLogFormatParserTest, ParseEscapedPercent)
{
  const auto tokens = RayLogFormatParser::Parse("100%% complete");
  EXPECT_EQ(tokens.size(), 2);
  EXPECT_FALSE(tokens[0].isSpecifier);
  EXPECT_EQ(tokens[0].value, "100%");
  EXPECT_FALSE(tokens[1].isSpecifier);
  EXPECT_EQ(tokens[1].value, " complete");
}

TEST_F(RayLogFormatParserTest, ParseEmptyString)
{
  auto tokens = RayLogFormatParser::Parse("");
  EXPECT_EQ(tokens.size(), 0);
}

TEST_F(RayLogFormatParserTest, ParseOnlyText)
{
  auto tokens = RayLogFormatParser::Parse("no specifiers here");
  EXPECT_EQ(tokens.size(), 1);
  EXPECT_FALSE(tokens[0].isSpecifier);
  EXPECT_EQ(tokens[0].value, "no specifiers here");
}
