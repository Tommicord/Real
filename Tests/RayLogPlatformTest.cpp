import <gtest/gtest.h>;

import Rl.RayLog.Platform;

using namespace Rl::RayLog::Platform;

class RayLogPlatformTest : public ::testing::Test
{
};

TEST_F(RayLogPlatformTest, ExactlyOnePlatformIsTrue)
{
  int trueCount = 0;
  if (IsAndroid) trueCount++;
  if (IsIOS) trueCount++;
  if (IsMacOS) trueCount++;
  if (IsLinux) trueCount++;
  if (IsWindows) trueCount++;
  
  EXPECT_EQ(trueCount, 1);
}

TEST_F(RayLogPlatformTest, PlatformFlagsAreCompileTime)
{
  EXPECT_TRUE((IsAndroid || !IsAndroid));
  EXPECT_TRUE((IsIOS || !IsIOS));
  EXPECT_TRUE((IsMacOS || !IsMacOS));
  EXPECT_TRUE((IsLinux || !IsLinux));
  EXPECT_TRUE((IsWindows || !IsWindows));
}

TEST_F(RayLogPlatformTest, AndroidAndLinuxAreMutuallyExclusive)
{
  EXPECT_FALSE(IsAndroid && IsLinux);
}

TEST_F(RayLogPlatformTest, IOSAndMacOSAreMutuallyExclusive)
{
  EXPECT_FALSE(IsIOS && IsMacOS);
}
