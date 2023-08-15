#pragma once

#include <array>
#include <limits>
#include <type_traits>
#include <algorithm>
#include <iostream>

#include <cxc/utility/macros.h>

namespace cxc
{

namespace detail
{

template<std::size_t N, std::size_t I, typename... Xs>
static constexpr auto stringify(const char x[N], Xs... xs) -> std::array<char, N - 1>
{
    if constexpr(I == N - 2) {
        return {xs..., '\0'};
    } else {
        return stringify<N, I + 1>(x, xs..., x[I]);
    }
}

}

template<auto _, std::size_t Start = 0>
constexpr auto stringify() -> std::string_view
{
    constexpr auto p = sizeof("std::string_view cxc::stringify() [_ = ") + Start;
    // FIXME: I'm pretty sure this is a bug
    static constexpr auto r = detail::stringify<sizeof(FUNCTION), p - 1>(FUNCTION);
    return {r.begin(), r.size()};
}

}
