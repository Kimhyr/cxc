#pragma once

namespace cxc
{
    constexpr bool isoctal(char c) noexcept
    {
        return c >= '0' && c <= '7';
    }
}
