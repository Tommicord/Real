import <gtest/gtest.h>;

import Rl.RayLog.Config;

using namespace Rl::RayLog;

class RayLogConfigTest : public ::testing::Test
{
};

TEST_F(RayLogConfigTest, MaxQueueSizeIsSet)
{
  EXPECT_GT(MaxQueueSize, 0);
  EXPECT_EQ(MaxQueueSize, 1024);
}

TEST_F(RayLogConfigTest, MaxFileSizeIsSet)
{
  EXPECT_GT(MaxFileSize, 0);
  EXPECT_EQ(MaxFileSize, 256 * 1024);
}

TEST_F(RayLogConfigTest, WorkerThreadsIsSet)
{
  EXPECT_GT(WorkerThreads, 0);
  EXPECT_EQ(WorkerThreads, 4);
}

TEST_F(RayLogConfigTest, ImmediateFlushIsSet)
{
  EXPECT_EQ(ImmediateFlush, true);
}
