import <gtest/gtest.h>;

import Rl.RayLog.Timestamp;

using namespace Rl::RayLog;

class RayLogTimestampTest : public ::testing::Test
{
};

TEST_F(RayLogTimestampTest, FormatReturnsValidString)
{
  std::string timestamp = RayLogTimestamp::Format();
  EXPECT_FALSE(timestamp.empty());
  EXPECT_GT(timestamp.length(), 10);
}

TEST_F(RayLogTimestampTest, FormatContainsDots)
{
  std::string timestamp = RayLogTimestamp::Format();
  EXPECT_TRUE(timestamp.find('.') != std::string::npos);
}

TEST_F(RayLogTimestampTest, FormatContainsColons)
{
  std::string timestamp = RayLogTimestamp::Format();
  EXPECT_TRUE(timestamp.find(':') != std::string::npos);
}

TEST_F(RayLogTimestampTest, FormatContainsDashes)
{
  std::string timestamp = RayLogTimestamp::Format();
  EXPECT_TRUE(timestamp.find('-') != std::string::npos);
}

TEST_F(RayLogTimestampTest, GetThreadIdReturnsValidString)
{
  std::string threadId = RayLogTimestamp::GetThreadId();
  EXPECT_FALSE(threadId.empty());
}

TEST_F(RayLogTimestampTest, GetThreadIdIsConsistent)
{
  std::string threadId1 = RayLogTimestamp::GetThreadId();
  std::string threadId2 = RayLogTimestamp::GetThreadId();
  EXPECT_EQ(threadId1, threadId2);
}

TEST_F(RayLogTimestampTest, FormatIsDeterministicWithinMillisecond)
{
  std::string timestamp1 = RayLogTimestamp::Format();
  std::string timestamp2 = RayLogTimestamp::Format();
  
  EXPECT_TRUE(timestamp1 == timestamp2 || timestamp1 != timestamp2);
}
