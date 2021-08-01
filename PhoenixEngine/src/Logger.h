#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

class Logger
{
public:
  enum class LogLevel
  {
    None = 0,
    Error,
    Warning,
    Trace
  };

  Logger()
  {
    spdlog::set_pattern("%^[%T] %n: %v%$");
    s_SpdLogger = spdlog::stdout_color_mt("Phoenix Engine");
#ifdef _DEBUG
    SetLevel(LogLevel::Trace);
#else
    SetLevel(LogLevel::None);
#endif // DEBUG
  }
  ~Logger() = default;

  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;

  void SetLevel(LogLevel level)
  {
    switch (level)
    {
    case LogLevel::Error:
      s_SpdLogger->set_level(spdlog::level::err);
      break;
    case LogLevel::Warning:
      s_SpdLogger->set_level(spdlog::level::warn);
      break;
    case LogLevel::Trace:
      s_SpdLogger->set_level(spdlog::level::trace);
      break;
    case LogLevel::None:
    default:
      s_SpdLogger->set_level(spdlog::level::off);
      break;
    }
  }

  inline void Trace(const char* message) const noexcept
  {
    s_SpdLogger->trace(message);
  }

  inline void Trace(string message) const noexcept
  {
    s_SpdLogger->trace(message);
  }

  inline void Warn(const char* message) const noexcept
  {
    s_SpdLogger->warn(message);
  }

  inline void Warn(string message) const noexcept
  {
    s_SpdLogger->warn(message);
  }

  inline void Error(const char* message) const noexcept
  {
    s_SpdLogger->error(message);
  }

  inline void Error(string message) const noexcept
  {
    s_SpdLogger->error(message);
  }

private:
  shared_ptr<spdlog::logger> s_SpdLogger;
};

