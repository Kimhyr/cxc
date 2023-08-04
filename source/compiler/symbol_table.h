#pragma once

#include <string_view>
#include <cstdint>
#include <list>
#include <unordered_map>

#include <cxc/syntax.h>

namespace cxc
{

struct Symbol
{
    using This = Symbol;

    enum Type
        : std::uint8_t
    {
        VALUE,
    };

    std::string_view identifier;
    Type             type;

    friend auto operator==(This const&, This const&) noexcept -> bool = default;
};

}

namespace std
{
    
template<>
struct hash<cxc::Symbol>
{
    auto operator()(cxc::Symbol const& x) const noexcept -> std::size_t
    {
        struct Bits
        {
            std::size_t type: 2,
                        identifier;
        };
        
        Bits v{
            .type = x.type,
            .identifier = hash<std::string_view>{}(x.identifier)
        };
        return v.type;
    }
};

}

namespace cxc
{

using SymbolTable = std::unordered_map<Symbol, Syntax*>;

// class SymbolTable
// {
// public:
//     struct Entry
//     {
//         std::string_view identifier;
//         Syntax&          syntax;
//         bool             is_public;
//     };

// private:
//     std::list<Entry> m_entries;
// };

// std::list<SymbolTable> tables;    

}
