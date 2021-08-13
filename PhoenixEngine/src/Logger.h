#pragma once

// Disable warnings for spdlog
#pragma warning(push)
#pragma warning(disable : 26812)
#pragma warning(disable : 6285)
#pragma warning(disable : 26437)
#pragma warning(disable : 26451)
#pragma warning(disable : 26498)
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#pragma warning(pop)

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
        m_SpdLogger->set_level(spdlog::level::err);
        break;
      case LogLevel::Warning:
        m_SpdLogger->set_level(spdlog::level::warn);
        break;
      case LogLevel::Trace:
        m_SpdLogger->set_level(spdlog::level::trace);
        break;
      case LogLevel::None:
      default:
        m_SpdLogger->set_level(spdlog::level::off);
        break;
      }
    }

    inline void Trace(const char* message) const noexcept
    {
      m_SpdLogger->trace(message);
    }

    inline void Trace(std::string message) const noexcept
    {
      m_SpdLogger->trace(message);
    }

    inline void Warn(const char* message) const noexcept
    {
      m_SpdLogger->warn(message);
    }

    inline void Warn(std::string message) const noexcept
    {
      m_SpdLogger->warn(message);
    }

    inline void Error(const char* message) const noexcept
    {
      m_SpdLogger->error(message);
    }

    inline void Error(std::string message) const noexcept
    {
      m_SpdLogger->error(message);
    }

  private:
    Logger() :
      m_SpdLogger(spdlog::stdout_color_mt("Phoenix"))
    {
      spdlog::set_pattern("%^[%L] (%T): %v%$");
      SetLevel(LogLevel::Trace);
      Trace("Start logging");
    }

    std::shared_ptr<spdlog::logger> m_SpdLogger;
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