#pragma once

#include <cstdint>
#include <string_view>

#include <cxc/syntax/token.h>

namespace cxc
{

struct ValueDeclaration;
struct Assignment;

enum class SyntaxType
    : std::uint8_t
{
    None,
    ValueDeclaration,
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

    Syntax() = default;

    Syntax(SyntaxType type)
        : m_type{type} {}
    
private:
    This*      m_prior{nullptr};
    This*      m_next{nullptr};
    union {
        ValueDeclaration* value_declaration;
        Assignment*       assignment;
    }          m_value;
    SyntaxType m_type;
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

struct ValueDeclaration
{
    Identifier identifier; 
    Type       type;
};

struct Assignment
{
    Expression* prior;
    Expression* next;
};

}
