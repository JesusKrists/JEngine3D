#include "LoggerController.hpp"

#if defined(JE_PLATFORM_UNIX)
#include <spdlog/sinks/ansicolor_sink.h>
#elif defined(JE_PLATFORM_WINDOWS)
#include <spdlog/sinks/stdout_color_sinks.h>
#endif

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/ostream_sink.h>

#include <spdlog/common.h>// for sink_ptr, trace
#include <spdlog/logger.h>// for logger
#include <spdlog/sinks/sink.h>// for sink
#include <iterator>// for begin, end

// IWYU pragma: no_include <ext/alloc_traits.h>

namespace JE {

LoggerController *LoggerController::s_LoggerControllerInstance = nullptr;// NOLINT

LoggerController::LoggerController()
{
  if (s_LoggerControllerInstance != nullptr) {
    spdlog::error("LoggerController instance already created");
    DEBUGBREAK();
  }
  s_LoggerControllerInstance = this;

  spdlog::set_level(spdlog::level::trace);
  spdlog::set_pattern("[%T] [%l] %n: %v");

  auto logSinks = std::vector<spdlog::sink_ptr>
  {
#if defined(JE_PLATFORM_UNIX)
    std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>(),
#elif defined(JE_PLATFORM_WINDOWS)
    std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
#endif
      std::make_shared<spdlog::sinks::basic_file_sink_mt>("JEngine3D.log", true)
  };

  logSinks[0]->set_pattern("%^[%T] [%l] %n: %v%$");
  logSinks[1]->set_pattern("[%T] [%l] %n: %v");

  m_CoreLogger = std::make_shared<spdlog::logger>(std::string{ "JEngine3D" }, std::begin(logSinks), std::end(logSinks));
  spdlog::register_logger(m_CoreLogger);
  m_CoreLogger->set_level(spdlog::level::trace);
  m_CoreLogger->flush_on(spdlog::level::trace);

  m_ClientLogger = std::make_shared<spdlog::logger>(std::string{ "App" }, std::begin(logSinks), std::end(logSinks));
  spdlog::register_logger(m_ClientLogger);
  m_ClientLogger->set_level(spdlog::level::trace);
  m_ClientLogger->flush_on(spdlog::level::trace);

  auto testSink = std::make_shared<spdlog::sinks::ostream_sink_mt>(m_TestLoggerOutput);
  testSink->set_pattern("[%T] [%l] %n: %v");
  m_TestLogger = std::make_shared<spdlog::logger>(std::string{ "TEST" }, testSink);
  spdlog::register_logger(m_TestLogger);
  m_TestLogger->set_level(spdlog::level::trace);
  m_TestLogger->flush_on(spdlog::level::trace);
}

}// namespace JE