#include "parser.h"

namespace cxc
{

// value my_value: U64 = 32;
auto Parser::parse(Syntax& p) -> Syntax&
{
    auto& n{*new Syntax};
    Token t{};
    m_lexer.next(t);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
    switch (t.type) {
#pragma GCC diagnostic pop
    case TokenType::Value:
        n.m_type = SyntaxType::ValueDeclaration;
        n.m_value.value_declaration = &parse_value_declaration();
        break;
    case TokenType::Equals:
        break;
    default: throw ParsingError(ParsingError::INVALID_TOKEN);
    }
    p.m_next = &n;
    return n;
}

auto Parser::parse_value_declaration() -> ValueDeclaration&
{
    auto& o{*new ValueDeclaration};
    parse_identifier(o.identifier);
    Token t{};
    m_lexer.next(t);
    if (t.type != TokenType::Colon) {
        throw ParsingError(ParsingError::INVALID_TOKEN);
    }
    parse_type(o.type);
    return o;
}

auto Parser::parse_identifier(Identifier& o) -> void
{
    Token t{};
    m_lexer.next(t);
    if (t.type != TokenType::Identifier) {
        throw ParsingError(ParsingError::INVALID_TOKEN);
    }
    o.value = t.value.identifier;
}

auto Parser::parse_type(Type& o) -> void
{
    Token t{};
    m_lexer.next(t);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
    switch (t.type)
#pragma GCC diagnostic pop
    {
    case TokenType::U8:
    case TokenType::U16:
    case TokenType::U32:
    case TokenType::U64:
    case TokenType::I8:
    case TokenType::I16:
    case TokenType::I32:
    case TokenType::I64:
    case TokenType::F16:
    case TokenType::F32:
    case TokenType::F64:
        o.type = SyntaxType::Primitive;
        o.value.primitive = t.type;
        break;
    default: throw ParsingError(ParsingError::INVALID_TOKEN);
    }
}

[[nodiscard]]
auto ParsingError::what() const noexcept -> char const*
{
    return Base::what();
}

}
