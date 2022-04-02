#pragma once

#include "JEngine3D/Core/Base.hpp"// for DEBUGBREAK
#include "JEngine3D/Core/MemoryController.hpp"

#include <spdlog/spdlog.h>// IWYU pragma: export
#include <sstream>// IWYU pragma: export
#include <fmt/core.h>// IWYU pragma: export

#include <string>// for string


namespace spdlog {
class logger;
}

namespace JE {

// NOLINTNEXTLINE(hicpp-special-member-functions, cppcoreguidelines-special-member-functions)
class LoggerController
{
  friend class Logger;

public:
  LoggerController();

  ~LoggerController() { s_LoggerControllerInstance = nullptr; }

  [[nodiscard]] static inline auto Get() -> LoggerController &
  {
    if (s_LoggerControllerInstance == nullptr) {
      spdlog::error("LoggerController instance is null");
      DEBUGBREAK();
    }
    return *s_LoggerControllerInstance;
  }

private:
  std::stringstream m_TestLoggerOutput;
  std::shared_ptr<spdlog::logger> m_CoreLogger;
  std::shared_ptr<spdlog::logger> m_ClientLogger;
  std::shared_ptr<spdlog::logger> m_TestLogger;

  static LoggerController *s_LoggerControllerInstance;// NOLINT
};


class Logger
{
public:
  static inline auto CoreLogger() -> Logger { return Logger{ LoggerController::Get().m_CoreLogger }; }
  static inline auto ClientLogger() -> Logger { return Logger{ LoggerController::Get().m_ClientLogger }; }
  static inline auto TestLogger() -> Logger { return Logger{ LoggerController::Get().m_TestLogger }; }
  static inline auto TestLoggerOutput() -> std::string { return LoggerController::Get().m_TestLoggerOutput.str(); }

  template<typename FormatString, typename... Args> inline void trace(const FormatString &fmt, Args &&...args)
  {
    m_LoggerRef->trace(fmt::runtime(fmt), std::forward<Args>(args)...);
  }

  template<typename FormatString, typename... Args> inline void debug(const FormatString &fmt, Args &&...args)
  {
    m_LoggerRef->debug(fmt::runtime(fmt), std::forward<Args>(args)...);
  }

  template<typename FormatString, typename... Args> inline void info(const FormatString &fmt, Args &&...args)
  {
    m_LoggerRef->info(fmt::runtime(fmt), std::forward<Args>(args)...);
  }

  template<typename FormatString, typename... Args> inline void warn(const FormatString &fmt, Args &&...args)
  {
    m_LoggerRef->warn(fmt::runtime(fmt), std::forward<Args>(args)...);
  }

  template<typename FormatString, typename... Args> inline void error(const FormatString &fmt, Args &&...args)
  {
    m_LoggerRef->error(fmt::runtime(fmt), std::forward<Args>(args)...);
  }

  template<typename FormatString, typename... Args> inline void critical(const FormatString &fmt, Args &&...args)
  {
    m_LoggerRef->critical(fmt::runtime(fmt), std::forward<Args>(args)...);
  }

  template<typename T> inline void trace(const T &msg) { m_LoggerRef->trace(msg); }

  template<typename T> inline void debug(const T &msg) { m_LoggerRef->debug(msg); }

  template<typename T> inline void info(const T &msg) { m_LoggerRef->info(msg); }

  template<typename T> inline void warn(const T &msg) { m_LoggerRef->warn(msg); }

  template<typename T> inline void error(const T &msg) { m_LoggerRef->error(msg); }

  template<typename T> inline void critical(const T &msg) { m_LoggerRef->critical(msg); }

private:
  explicit Logger(std::shared_ptr<spdlog::logger> &logger) : m_LoggerRef(logger) {}
  std::shared_ptr<spdlog::logger> &m_LoggerRef;
};

}// namespace JE