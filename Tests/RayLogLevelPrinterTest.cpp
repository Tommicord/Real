import <gtest/gtest.h>;

import Rl.RayLog.LevelPrinter;

using namespace Rl::RayLog;

class RayLogLevelPrinterTest : public ::testing::Test
{
  protected:
  void SetUp() override
  {
    printer = new RayLogLevelPrinter();
  }

  void TearDown() override
  {
    delete printer;
  }

  RayLogLevelPrinter* printer;
};

TEST_F(RayLogLevelPrinterTest, ToStringTrace)
{
  EXPECT_EQ(printer->ToString(RayLogLevel::Trace), "TRACE");
}

TEST_F(RayLogLevelPrinterTest, ToStringDebug)
{
  EXPECT_EQ(printer->ToString(RayLogLevel::Debug), "DEBUG");
}

TEST_F(RayLogLevelPrinterTest, ToStringInfo)
{
  EXPECT_EQ(printer->ToString(RayLogLevel::Info), "INFO");
}

TEST_F(RayLogLevelPrinterTest, ToStringWarning)
{
  EXPECT_EQ(printer->ToString(RayLogLevel::Warning), "WARN");
}

TEST_F(RayLogLevelPrinterTest, ToStringError)
{
  EXPECT_EQ(printer->ToString(RayLogLevel::Error), "ERROR");
}

TEST_F(RayLogLevelPrinterTest, ToStringFatal)
{
  EXPECT_EQ(printer->ToString(RayLogLevel::Fatal), "FATAL");
}

TEST_F(RayLogLevelPrinterTest, AllLevelsReturnNonEmpty)
{
  EXPECT_FALSE(printer->ToString(RayLogLevel::Trace).empty());
  EXPECT_FALSE(printer->ToString(RayLogLevel::Debug).empty());
  EXPECT_FALSE(printer->ToString(RayLogLevel::Info).empty());
  EXPECT_FALSE(printer->ToString(RayLogLevel::Warning).empty());
  EXPECT_FALSE(printer->ToString(RayLogLevel::Error).empty());
  EXPECT_FALSE(printer->ToString(RayLogLevel::Fatal).empty());
}
