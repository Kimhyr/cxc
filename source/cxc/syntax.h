#pragma once

#include <cstdint>
#include <stdexcept>
#include <string_view>

#include <cxc/syntax/token.h>

namespace cxc
{

enum class SyntaxType
{
    None,

    TypeCast,

    // Declarations
    Value,

    // Types
    Primitive,

    // Expressions
    Literal,
    Multiplication,
    Addition,
};

struct Syntax
{
    SyntaxType type{SyntaxType::None};
};

namespace syntax
{

struct Declaration;

struct TypeCast
    : public Syntax
{
    struct Flag
    {
        std::uint8_t is_mutable : 1 = false;
    };

    Declaration& type;
    Flag         flag;        
};

struct Declaration
    : public Syntax
{
    Symbol&   symbol;
    TypeCast  type;
    Syntax*   definition;
};

struct Literal
    : public Syntax
{
    union Value
    {
        char             character;
        std::string_view string;
        std::int64_t     integer;
        std::uint64_t    uinteger;
        double           floating;
    };

    Value value{};
};

struct Binary
    : public Syntax
{
    Syntax* first;
    Syntax& second;
};

}
