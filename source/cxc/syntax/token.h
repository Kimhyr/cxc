#pragma once

#include <cstdint>
#include <limits>
#include <format>
#include <type_traits>
#include <utility>

#include <cxc/utility/macros.h>
#include <cxc/utility/string.h>
#include <cxc/utility.h>
#include <cxc/diagnostic.h>

namespace cxc
{

enum class TokenType
    : std::int8_t
{
    Identifier = std::numeric_limits<std::int8_t>::min(),
    End,

    // Literals
    ByteLiteral,
    IntegerLiteral,
    FloatingLiteral,
    StringLiteral,
    CharacterLiteral,

    //
    // Words
    //

    // Determiners
    Module,   // 'module'
    Trait,    // 'trait'
    Type,     // 'type'
    Extend,   // 'extend'
    Function, // 'function'
    Value,    // 'value'

    // Actions
    Use,    // 'use'
    Return, // 'return'

    //
    // Types
    //

    // Bit Types
    Byte,   // 'byte'
    Byte1,  // 'byte1'
    Byte2,  // 'byte2'
    Byte4,  // 'byte4'
    Byte8,  // 'byte8'
    Byte16, // 'byte16'

    // Integer Types
    Int,   // 'int'
    Int1,  // 'int1'
    Int2,  // 'int2'
    Int4,  // 'int4'
    Int8,  // 'int8'
    Int16, // 'int16'

    // Floating-point Types
    Float,   // 'float'
    Float2,  // 'float2'
    Float4,  // 'float4'
    Float8,  // 'float8'
    Float16, // 'float16'

    // Regular Punctuators
    RightwardsArrow,
    Apostrophe      = '\'',
    QutationMark    = '"',
    FullStop        = '.',
    Comma           = ',',
    Colon           = ':',
    Semicolon       = ';',
    EqualsSign      = '=',
    PlusSign        = '+',
    MinusSign       = '-',
    Asterisk        = '*',
    Solidus         = '/',
    ReverseSolidus  = '\\',
    VerticalLine    = '|',
    ExclamationMark = '!',
    QuestionMark    = '?',
    ComercialAt     = '@',
    NumberSign      = '#',

    // Delimiterized Punctuators
    LeftCurlyBracket   = '{',
    RightCurlyBracket  = '}',
    LeftParenthesis    = '(',
    RightParenthesis   = ')',
    LeftAngleBracket   = '<',
    RightAngleBracket  = '>',
    LeftSquareBracket  = '[',
    RightSquareBracket = ']',
};

struct Token
{
    union Value
    {
        std::string_view identifier;
        unsigned long    byte;
        long             integer;
        double           floating;
        std::string_view string;
        char             character;
    };

    Value     value{};
    Location  location;
    TokenType type;
};

}

namespace std
{

template<>
struct std::formatter<cxc::TokenType>
    : formatter<string>
{
    auto format(cxc::TokenType i, format_context& ctx) const
    {
        using namespace cxc;

        std::string_view str;
        switch (i) {
        case TokenType::Identifier:         str = "Identifier"; break;
        case TokenType::End:                str = "End"; break;
        case TokenType::ByteLiteral:        str = "Byte"; break;
        case TokenType::IntegerLiteral:     str = "Integer"; break;
        case TokenType::FloatingLiteral:    str = "Floating"; break;
        case TokenType::StringLiteral:      str = "String"; break;
        case TokenType::CharacterLiteral:   str = "Character"; break;
        case TokenType::Module:             str = "Module"; break;
        case TokenType::Trait:              str = "Trait"; break;
        case TokenType::Type:               str = "Type"; break;
        case TokenType::Extend:             str = "Extend"; break;
        case TokenType::Function:           str = "Function"; break;
        case TokenType::Value:              str = "Value"; break;
        case TokenType::Use:                str = "Use"; break;
        case TokenType::Return:             str = "Return"; break;
        case TokenType::Byte:               str = "Byte"; break;
        case TokenType::Byte1:              str = "Byte1"; break;
        case TokenType::Byte2:              str = "Byte2"; break;
        case TokenType::Byte4:              str = "Byte4"; break;
        case TokenType::Byte8:              str = "Byte8"; break;
        case TokenType::Byte16:             str = "Byte16"; break;
        case TokenType::Int:                str = "Int"; break;
        case TokenType::Int1:               str = "Int1"; break;
        case TokenType::Int2:               str = "Int2"; break;
        case TokenType::Int4:               str = "Int4"; break;
        case TokenType::Int8:               str = "Int8"; break;
        case TokenType::Int16:              str = "Int16"; break;
        case TokenType::Float:              str = "Float"; break;
        case TokenType::Float2:             str = "Float2"; break;
        case TokenType::Float4:             str = "Float4"; break;
        case TokenType::Float8:             str = "Float8"; break;
        case TokenType::Float16:            str = "Float16"; break;
        case TokenType::RightwardsArrow:    str = "RightwardsArrow"; break;
        case TokenType::Apostrophe:         str = "Apostrophe"; break;
        case TokenType::QutationMark:       str = "QutationMark"; break;
        case TokenType::FullStop:           str = "FullStop"; break;
        case TokenType::Comma:              str = "Comma"; break;
        case TokenType::Colon:              str = "Colon"; break;
        case TokenType::Semicolon:          str = "Semicolon"; break;
        case TokenType::EqualsSign:         str = "EqualsSign"; break;
        case TokenType::PlusSign:           str = "PlusSign"; break;
        case TokenType::MinusSign:          str = "MinuxSign"; break;
        case TokenType::Asterisk:           str = "Asterisk"; break;
        case TokenType::Solidus:            str = "Solidus"; break;
        case TokenType::ReverseSolidus:     str = "ReverseSolidus"; break;
        case TokenType::VerticalLine:       str = "VerticalLine"; break;
        case TokenType::ExclamationMark:    str = "ExclamationMark"; break;
        case TokenType::QuestionMark:       str = "QuestionMark"; break;
        case TokenType::ComercialAt:        str = "ComercialAt"; break;
        case TokenType::NumberSign:         str = "NumberSign"; break;
        case TokenType::LeftCurlyBracket:   str = "LeftCurlyBracket"; break;
        case TokenType::RightCurlyBracket:  str = "RightCurlyBracket"; break;
        case TokenType::LeftParenthesis:    str = "LeftParenthesis"; break;
        case TokenType::RightParenthesis:   str = "RightParenthesis"; break;
        case TokenType::LeftAngleBracket:   str = "LeftAngleBracket"; break;
        case TokenType::RightAngleBracket:  str = "RightAngleBracket"; break;
        case TokenType::LeftSquareBracket:  str = "LeftSquareBracket"; break;
        case TokenType::RightSquareBracket: str = "RightSquareBracket"; break;
        }
        return std::formatter<string>::format(std::format("{}", str), ctx);
    }
};

template<>
struct std::formatter<cxc::Token>
    : formatter<string>
{
    template<typename Ctx>
    auto format(cxc::Token const& t, Ctx& ctx) const
    {
        using namespace cxc;

        string value;
        switch (t.type) {
        case TokenType::Identifier:       value = std::format("{}", t.value.identifier); break;
        case TokenType::ByteLiteral:      value = std::format("{}", t.value.byte); break;
        case TokenType::IntegerLiteral:   value = std::format("{}", t.value.integer); break;
        case TokenType::FloatingLiteral:  value = std::format("{}", t.value.floating); break;
        case TokenType::StringLiteral:    value = std::format("\"{}\"", t.value.string); break;
        case TokenType::CharacterLiteral: value = std::format("'{}'", t.value.character); break;
        case TokenType::RightwardsArrow:  value = "->"; break;
        default:
            if (to_underlying(t.type) > 0) {
                value = to_underlying(t.type);
            } else {
                value = std::format("{}", t.type);
            }
            break;
        }
        auto format = std::format("Token {{ type = {}, value = {}, location = {} }}",
                                  t.type, value, t.location);
        return formatter<string>::format(format, ctx);
    }
};

}
