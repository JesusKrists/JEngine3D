#include <catch2/catch_test_macros.hpp>// for StringRef, oper...

#include <JEngine3D/Core/MemoryController.hpp>
#include <JEngine3D/Core/LoggerController.hpp>

#include <exception>// for exception
#include <string>// for string


class LoggerTestsFixture
{
protected:
  JE::MemoryController m_MemoryController;
  JE::LoggerController m_WindowController;
};

TEST_CASE_METHOD(LoggerTestsFixture,
  "JE::LoggerController creates Loggers and TestLogger trace outputs something",
  "[JE::LoggerController]")
{
  JE::Logger::TestLogger().trace("This is a test message");
  REQUIRE(!JE::Logger::TestLoggerOutput().empty());
}

TEST_CASE_METHOD(LoggerTestsFixture,
  "JE::LoggerController creates Loggers and TestLogger debug outputs something",
  "[JE::LoggerController]")
{
  JE::Logger::TestLogger().debug("This is a test message");
  REQUIRE(!JE::Logger::TestLoggerOutput().empty());
}

TEST_CASE_METHOD(LoggerTestsFixture,
  "JE::LoggerController creates Loggers and TestLogger info outputs something",
  "[JE::LoggerController]")
{
  JE::Logger::TestLogger().info("This is a test message");
  REQUIRE(!JE::Logger::TestLoggerOutput().empty());
}

TEST_CASE_METHOD(LoggerTestsFixture,
  "JE::LoggerController creates Loggers and TestLogger warn outputs something",
  "[JE::LoggerController]")
{
  JE::Logger::TestLogger().warn("This is a test message");
  REQUIRE(!JE::Logger::TestLoggerOutput().empty());
}

TEST_CASE_METHOD(LoggerTestsFixture,
  "JE::LoggerController creates Loggers and TestLogger error outputs something",
  "[JE::LoggerController]")
{
  JE::Logger::TestLogger().error("This is a test message");
  REQUIRE(!JE::Logger::TestLoggerOutput().empty());
}

TEST_CASE_METHOD(LoggerTestsFixture,
  "JE::LoggerController creates Loggers and TestLogger critical outputs something",
  "[JE::LoggerController]")
{
  JE::Logger::TestLogger().critical("This is a test message");
  REQUIRE(!JE::Logger::TestLoggerOutput().empty());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE_METHOD(LoggerTestsFixture,
  "JE::LoggerController creates Loggers and TestLogger trace parameterized outputs something",
  "[JE::LoggerController]")
{
  JE::Logger::TestLogger().trace("This is a test message {0}", 0);
  REQUIRE(!JE::Logger::TestLoggerOutput().empty());
}

TEST_CASE_METHOD(LoggerTestsFixture,
  "JE::LoggerController creates Loggers and TestLogger debug parameterized outputs something",
  "[JE::LoggerController]")
{
  JE::Logger::TestLogger().debug("This is a test message {0}", 0);
  REQUIRE(!JE::Logger::TestLoggerOutput().empty());
}

TEST_CASE_METHOD(LoggerTestsFixture,
  "JE::LoggerController creates Loggers and TestLogger info parameterized outputs something",
  "[JE::LoggerController]")
{
  JE::Logger::TestLogger().info("This is a test message {0}", 0);
  REQUIRE(!JE::Logger::TestLoggerOutput().empty());
}

TEST_CASE_METHOD(LoggerTestsFixture,
  "JE::LoggerController creates Loggers and TestLogger warn parameterized outputs something",
  "[JE::LoggerController]")
{
  JE::Logger::TestLogger().warn("This is a test message {0}", 0);
  REQUIRE(!JE::Logger::TestLoggerOutput().empty());
}

TEST_CASE_METHOD(LoggerTestsFixture,
  "JE::LoggerController creates Loggers and TestLogger error parameterized outputs something",
  "[JE::LoggerController]")
{
  JE::Logger::TestLogger().error("This is a test message {0}", 0);
  REQUIRE(!JE::Logger::TestLoggerOutput().empty());
}

TEST_CASE_METHOD(LoggerTestsFixture,
  "JE::LoggerController creates Loggers and TestLogger critical parameterized outputs something",
  "[JE::LoggerController]")
{
  JE::Logger::TestLogger().critical("This is a test message {0}", 0);
  REQUIRE(!JE::Logger::TestLoggerOutput().empty());
}