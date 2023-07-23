#include "parser.h"

#include <cxc/syntax.h>

namespace cxc
{

auto Parser::load(stdfs::path const& file) -> void
{
    m_lexer.load(file);
}

// value my_value: u64 {21}
auto Parser::next(Syntax& o) -> void
{
    consume();
    switch (token().type) {
    case TokenType::Value:
        o.m_type = SyntaxType::Value;
        next_value(*o.m_value.value);
        break;
    default: throw ParsingError{};
    }
}

auto Parser::next_value(Value& o) -> void
{
    next_identifier(o.identifier);
    consume();
    if (token().type != TokenType::Colon) {
        throw ParsingError{};
    }
    next_type(o.type);
    consume();
    switch (token().type) {
    case TokenType::LeftCurl:
        next_initializer(o.initializer);
        break;
    default: break;
    }
}

auto Parser::next_expression(Syntax& o) -> void
{

}

auto Parser::next_initializer(Initializer& o) -> void
{
    next_expression(*o.expression);
    consume();
    if (token().type != TokenType::RightCurl) {
        throw ParsingError{};
    }
}

auto Parser::next_type(Type& o) -> void
{
    consume();
    switch (token().type) {
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
        o.value.primitive = token().type;
        break;
    default:
        throw ParsingError{};
    }
}

auto Parser::next_identifier(Identifier& o) -> void
{
    consume();
    if (token().type != TokenType::Identifier) {
        throw ParsingError{};
    }
    o.value = token().value.identifier;
};

[[nodiscard]]
auto ParsingError::what() const noexcept -> char const*
{
    return Base::what();
}

}
