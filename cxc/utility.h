#ifndef CXC_UTILITY_H
#define CXC_UTILITY_H

#include <type_traits>

namespace cxc
{
  template<typename T>
  concept enumerated = std::is_enum_v<T>;

  template<enumerated T>
  constexpr std::underlying_type_t<T> to_underlying(T v)
  {
    return static_cast<std::underlying_type_t<T>>(v);
  }
}

#endif
