import <gtest/gtest.h>;

import Rl.RayLog.RingBuffer;

using namespace Rl::RayLog;

class RayLogRingBufferTest : public ::testing::Test
{
  protected:
  void SetUp() override
  {
    buffer = new RayLogRingBuffer<int>(10);
  }

  void TearDown() override
  {
    delete buffer;
  }

  RayLogRingBuffer<int>* buffer;
};

TEST_F(RayLogRingBufferTest, PushAndPopSingleItem)
{
  EXPECT_TRUE(buffer->Push(42));
  
  int value = 0;
  EXPECT_TRUE(buffer->Pop(value));
  EXPECT_EQ(value, 42);
}

TEST_F(RayLogRingBufferTest, PushMultipleItems)
{
  for (int i = 0; i < 5; ++i)
  {
    EXPECT_TRUE(buffer->Push(i));
  }

  for (int i = 0; i < 5; ++i)
  {
    int value = 0;
    EXPECT_TRUE(buffer->Pop(value));
    EXPECT_EQ(value, i);
  }
}

TEST_F(RayLogRingBufferTest, EmptyInitially)
{
  EXPECT_TRUE(buffer->IsEmpty());
  EXPECT_EQ(buffer->Size(), 0);
}

TEST_F(RayLogRingBufferTest, NotEmptyAfterPush)
{
  buffer->Push(1);
  EXPECT_FALSE(buffer->IsEmpty());
  EXPECT_EQ(buffer->Size(), 1);
}

TEST_F(RayLogRingBufferTest, PopFromEmpty)
{
  int value = 0;
  EXPECT_FALSE(buffer->Pop(value));
}

TEST_F(RayLogRingBufferTest, FullBuffer)
{
  RayLogRingBuffer<int> smallBuffer(3);
  
  EXPECT_TRUE(smallBuffer.Push(1));
  EXPECT_TRUE(smallBuffer.Push(2));
  EXPECT_TRUE(smallBuffer.IsFull());
  
  EXPECT_FALSE(smallBuffer.Push(3));
}

TEST_F(RayLogRingBufferTest, WrapAround)
{
  RayLogRingBuffer<int> smallBuffer(3);
  
  smallBuffer.Push(1);
  smallBuffer.Push(2);
  
  int value = 0;
  smallBuffer.Pop(value);
  EXPECT_EQ(value, 1);
  
  EXPECT_TRUE(smallBuffer.Push(3));
  
  smallBuffer.Pop(value);
  EXPECT_EQ(value, 2);
  smallBuffer.Pop(value);
  EXPECT_EQ(value, 3);
}

TEST_F(RayLogRingBufferTest, SizeAfterOperations)
{
  EXPECT_EQ(buffer->Size(), 0);
  
  buffer->Push(1);
  EXPECT_EQ(buffer->Size(), 1);
  
  buffer->Push(2);
  buffer->Push(3);
  EXPECT_EQ(buffer->Size(), 3);
  
  int value = 0;
  buffer->Pop(value);
  EXPECT_EQ(buffer->Size(), 2);
}

TEST(RayLogRingBufferEdgeCases, MinimumCapacity)
{
  RayLogRingBuffer<int> minBuffer(2);
  EXPECT_TRUE(minBuffer.Push(42));
  EXPECT_TRUE(minBuffer.IsFull());
  
  int value = 0;
  EXPECT_TRUE(minBuffer.Pop(value));
  EXPECT_EQ(value, 42);
  EXPECT_TRUE(minBuffer.IsEmpty());
}
