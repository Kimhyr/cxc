#include "parser.h"

namespace cxc
{

auto Parser::load(stdfs::path const& file) -> void
{
    m_lexer.load(file);
}

// value my_value: U64 = 21;
auto Parser::next(Syntax& o) -> void
{
    lex();
    switch (token().type) {
    case TokenType::Value:
        o.m_type = SyntaxType::Value;
        o.m_value.value = new Value;
        lex();
        if (token().type != TokenType::Identifier) {
            throw ParsingError("expected identifier");
        }
        o.m_value.value->identifier.value = token().value.identifier; 

        lex();
        if (token().type != TokenType::Colon) {
            throw ParsingError("expected colon");
        }
        lex();
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
            o.m_value.value->type.type = SyntaxType::Primitive;
            o.m_value.value->type.value.primitive = token().type;
            break;
        default: throw ParsingError("expected primitive");
        }
        break;
    case TokenType::Function:
        throw ParsingError("unexpected token function");
    default: throw ParsingError("expected primary token");
    }
}


[[nodiscard]]
auto ParsingError::what() const noexcept -> char const*
{
    return Base::what();
}

}
