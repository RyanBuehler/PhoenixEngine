#pragma once

// Disable warnings for spdlog
#pragma warning(push)
#pragma warning(disable : 26812)
#pragma warning(disable : 6285)
#pragma warning(disable : 26437)
#pragma warning(disable : 26451)
#pragma warning(disable : 26498)
#pragma warning(disable : 26800)
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
      NONE = 0,
      ERR,
      WARN,
      TRACE
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

    void SetLevel(const LogLevel Level) const
    {
      switch (Level)
      {
      case LogLevel::ERR:
        m_SpdLogger->set_level(spdlog::level::err);
        break;
      case LogLevel::WARN:
        m_SpdLogger->set_level(spdlog::level::warn);
        break;
      case LogLevel::TRACE:
        m_SpdLogger->set_level(spdlog::level::trace);
        break;
      case LogLevel::NONE:
        m_SpdLogger->set_level(spdlog::level::off);
        break;
      }
    }

    void Trace(const char* Message) const noexcept
    {
      m_SpdLogger->trace(Message);
    }

    void Trace(const std::string& Message) const noexcept
    {
      m_SpdLogger->trace(Message);
    }

    void Warn(const char* Message) const noexcept
    {
      m_SpdLogger->warn(Message);
    }

    void Warn(const std::string& Message) const noexcept
    {
      m_SpdLogger->warn(Message);
    }

    void Error(const char* Message) const noexcept
    {
      m_SpdLogger->error(Message);
    }

    void Error(const std::string& Message) const noexcept
    {
      m_SpdLogger->error(Message);
    }

  private:
    Logger() :
      m_SpdLogger(spdlog::stdout_color_mt("Phoenix"))
    {
      spdlog::set_pattern("%^[%l] (%T): %v%$");
      SetLevel(LogLevel::TRACE);
      Trace("Start logging");
    }

    std::shared_ptr<spdlog::logger> m_SpdLogger;
  };

  inline void Trace(const char* Message) noexcept
  {
    Logger::Instance().Trace(Message);
  }

  inline void Trace(const std::string& Message) noexcept
  {
    Logger::Instance().Trace(Message);
  }

  inline void Warn(const char* Message) noexcept
  {
    Logger::Instance().Warn(Message);
  }

  inline void Warn(const std::string& Message) noexcept
  {
    Logger::Instance().Warn(Message);
  }

  inline void Error(const char* Message) noexcept
  {
    Logger::Instance().Error(Message);
  }

  inline void Error(const std::string& Message) noexcept
  {
    Logger::Instance().Error(Message);
  }
}