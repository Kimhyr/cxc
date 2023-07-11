#ifndef CXC_DIAGNOSTIC_H
#define CXC_DIAGNOSTIC_H

#include <cstdint>

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
    span        span;
  }; 
}

#endif
