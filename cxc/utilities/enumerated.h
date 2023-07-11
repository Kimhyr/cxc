#ifndef CXC_ENUMERATED_H
#define CXC_ENUMERATED_H

#include <compare>
#include <type_traits>

namespace cxc
{
  template<typename T>
    requires std::is_enum_v<T>
  class enumerated
  {
  public:
    using type = T;

    constexpr enumerated() noexcept = default;
    constexpr enumerated(type value) noexcept
      : m_value{value} {}

    constexpr enumerated operator=(type right) noexcept { m_value = right; }

    friend std::strong_ordering operator<=>(enumerated const&, enumerated const&) noexcept = default;
    friend std::strong_ordering operator<=>(enumerated const& left, type const& right) noexcept { left.m_value <=> right; }
   
  private:
    type m_value;
  };
}

#endif
