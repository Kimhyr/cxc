#pragma once

#include <cstdint>
#include <stdexcept>
#include <string_view>

#include <cxc/syntax/token.h>

namespace cxc
{

struct Value;
struct      Assignment;

enum class SyntaxType
    : std::uint8_t
{
    None,
    Value,
    Primitive,
    Literal,
    Assignment,
    Identifier,
    Type,
};

class Syntax
{
    friend class Parser;

public:
    using This  = Syntax;

    union Value {
        cxc::Value* value;
        Assignment* assignment;
    };

    Syntax() = default;

    Syntax(This& prior)
        : m_prior{&prior} {}

    Syntax(SyntaxType type)
        : m_type{type} {}

    [[nodiscard]] auto prior() const noexcept -> This const& { return *m_prior; }
    [[nodiscard]] auto next() const noexcept -> This const& { return *m_next; }
    [[nodiscard]] auto type() const noexcept -> SyntaxType { return m_type; }
    [[nodiscard]] auto value() const noexcept -> Value { return m_value; } 

    [[nodiscard]]
    auto is_expression() const noexcept -> bool
    {
        switch (type()) {
        case SyntaxType::Assignment:
        case SyntaxType::Literal:
            return true;
        default: return false;
        }
    }
    
private:
    This*      m_prior{nullptr};
    This*      m_next{nullptr};
    Value      m_value{};
    SyntaxType m_type{SyntaxType::None};
};

struct Identifier
{
    std::string_view value;
};

using Primitive = TokenType;

struct Expression
{
    union {
        
    }         value;
    SyntaxType type;
};

struct Type
{
    SyntaxType type;
    union {
        Primitive primitive;
    } value;
};

struct Value
{     
    Identifier identifier; 
    Type       type;
};

struct Assignment
{
};

}
