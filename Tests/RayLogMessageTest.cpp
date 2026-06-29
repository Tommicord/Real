import Rl.RayLog.Message;
import Rl.RayLog.LevelPrinter;

import <gtest/gtest.h>;
import <chrono>;

using namespace Rl::RayLog;

class RayLogMessageTest : public ::testing::Test
{
  protected:
  void SetUp() override
  {
    message = new RayLogMessage(RayLogLevel::Info, "Test message", "TestTag");
  }

  void TearDown() override
  {
    delete message;
  }

  RayLogMessage* message;
};

TEST_F(RayLogMessageTest, ConstructorInitializesFields)
{
  EXPECT_EQ(message->level, RayLogLevel::Info);
  EXPECT_EQ(message->formattedMessage, "Test message");
  EXPECT_EQ(message->tag, "TestTag");
}

TEST_F(RayLogMessageTest, DefaultConstructor)
{
  RayLogMessage defaultMessage;
  EXPECT_EQ(defaultMessage.formattedMessage, "");
  EXPECT_EQ(defaultMessage.tag, "");
}

TEST_F(RayLogMessageTest, MoveConstructor)
{
  RayLogMessage original(RayLogLevel::Error, "Error message", "ErrorTag");
  RayLogMessage moved(std::move(original));
  
  EXPECT_EQ(moved.level, RayLogLevel::Error);
  EXPECT_EQ(moved.formattedMessage, "Error message");
  EXPECT_EQ(moved.tag, "ErrorTag");
}

TEST_F(RayLogMessageTest, DifferentLogLevels)
{
  RayLogMessage traceMsg(RayLogLevel::Trace, "Trace", "Tag");
  RayLogMessage debugMsg(RayLogLevel::Debug, "Debug", "Tag");
  RayLogMessage infoMsg(RayLogLevel::Info, "Info", "Tag");
  RayLogMessage warnMsg(RayLogLevel::Warning, "Warning", "Tag");
  RayLogMessage errorMsg(RayLogLevel::Error, "Error", "Tag");
  RayLogMessage fatalMsg(RayLogLevel::Fatal, "Fatal", "Tag");
  
  EXPECT_EQ(traceMsg.level, RayLogLevel::Trace);
  EXPECT_EQ(debugMsg.level, RayLogLevel::Debug);
  EXPECT_EQ(infoMsg.level, RayLogLevel::Info);
  EXPECT_EQ(warnMsg.level, RayLogLevel::Warning);
  EXPECT_EQ(errorMsg.level, RayLogLevel::Error);
  EXPECT_EQ(fatalMsg.level, RayLogLevel::Fatal);
}

TEST_F(RayLogMessageTest, TimestampIsSet)
{
  auto now = std::chrono::system_clock::now();
  RayLogMessage msg(RayLogLevel::Info, "Test", "Tag");
  
  auto diff = std::chrono::duration_cast<std::chrono::seconds>(
    msg.timestamp - now).count();
  
  EXPECT_LE(std::abs(diff), 1);
}

TEST_F(RayLogMessageTest, ThreadIdIsSet)
{
  RayLogMessage msg(RayLogLevel::Info, "Test", "Tag");
  EXPECT_EQ(msg.tid, std::this_thread::get_id());
}
