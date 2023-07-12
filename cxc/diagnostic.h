#ifndef CXC_DIAGNOSTIC_H
#define CXC_DIAGNOSTIC_H

#include <cstdint>
#include <format>

namespace cxc
{
  struct position
  {
    std::size_t row;
    std::size_t column;
  };

  struct span
  {
    position start;
    position end;
  };

  struct location
  {
    char const* file_path;
    position    position;
  };
}

namespace std
{
  template<>
  struct formatter<cxc::position>
    : formatter<string>
  {
    template<typename Ctx>
    auto format(cxc::position const& t, Ctx& ctx) const
    {
      return formatter<string>::format(std::format("{},{}", t.row, t.column), ctx);
    }
  };

  template<>
  struct formatter<cxc::span>
    : formatter<string>
  {
    template<typename Ctx>
    auto format(cxc::span const& t, Ctx& ctx) const
    {
      return formatter<string>::format(std::format("{}:{}", t.start, t.end), ctx);
    }
  };
 
  template<>
  struct formatter<cxc::location>
    : formatter<string>
  {
    template<typename Ctx>
    auto format(cxc::location const& t, Ctx& ctx) const
    {
      return formatter<string>::format(std::format("{}:{}", t.file_path, t.position), ctx);
    }
  };
}

#endif
