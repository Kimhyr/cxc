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
    Syntax* syntax;
};

using SymbolTable = std::unordered_map<std::string_view, Symbol>;

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
