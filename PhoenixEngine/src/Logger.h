#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Log
{
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

    static Logger& Instance()
    {
      static Logger instance;
      return instance;
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

    inline void Trace(std::string message) const noexcept
    {
      s_SpdLogger->trace(message);
    }

    inline void Warn(const char* message) const noexcept
    {
      s_SpdLogger->warn(message);
    }

    inline void Warn(std::string message) const noexcept
    {
      s_SpdLogger->warn(message);
    }

    inline void Error(const char* message) const noexcept
    {
      s_SpdLogger->error(message);
    }

    inline void Error(std::string message) const noexcept
    {
      s_SpdLogger->error(message);
    }

  private:
    Logger() :
      s_SpdLogger(spdlog::stdout_color_mt("Phoenix Engine"))
    {
      spdlog::set_pattern("%^[%T] %n: %v%$");
      SetLevel(LogLevel::Trace);
    }

    std::shared_ptr<spdlog::logger> s_SpdLogger;
  };

  inline void Trace(const char* message) noexcept
  {
    Logger::Instance().Trace(message);
  }

  inline void Trace(std::string message) noexcept
  {
    Logger::Instance().Trace(message);
  }

  inline void Warn(const char* message) noexcept
  {
    Logger::Instance().Warn(message);
  }

  inline void Warn(std::string message) noexcept
  {
    Logger::Instance().Warn(message);
  }

  inline void Error(const char* message) noexcept
  {
    Logger::Instance().Error(message);
  }

  inline void Error(std::string message) noexcept
  {
    Logger::Instance().Error(message);
  }
}