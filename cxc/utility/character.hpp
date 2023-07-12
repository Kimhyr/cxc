#ifndef CXC_CHARACTER_HPP
#define CXC_CHARACTER_HPP

namespace cxc
{
  constexpr bool isoctal(char c) noexcept
  {
    return c >= '0' && c <= '7';
  }
}

#endif
