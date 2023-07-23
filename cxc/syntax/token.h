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
    String,
    Character,
    UInteger,
    Floating,
    Function, 
    Value,
    U8,
    U16,
    U32,
    U64,
    I8,
    I16,
    I32,
    I64,
    F16,
    F32,
    F64,
    ForwardArrow,
    Period           = '.',
    Quote            = '"',
    Apostrpohe       = '\'',
    Minus            = '-',
    Plus             = '+',
    Colon            = ':',
    LeftParenthesis  = '(',
    RightParenthesis = ')',
    LeftCurl         = '{',
    RightCurl        = '}',
    LeftAngle        = '<',
    RightAngle       = '>',
    Comma            = ',',
    Semicolon        = ';',
    Equals           = '=',
};

struct Token
{
    union {
        std::string_view identifier;
        char             character;
        std::string_view string;
        std::uint64_t    uinteger;
        double           floating;
    }         value {};
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
        std::string_view str;
        switch (i) {
        case cxc::TokenType::Identifier:       str = "identifier"; break;
        case cxc::TokenType::Character:        str = "character"; break;
        case cxc::TokenType::String:           str = "string"; break;
        case cxc::TokenType::UInteger:         str = "uinteger"; break;
        case cxc::TokenType::Floating:         str = "floating"; break;
        case cxc::TokenType::ForwardArrow:     str = "forward_arrow"; break;
        case cxc::TokenType::Function:         str = "function"; break;
        case cxc::TokenType::Value:            str = "value"; break;
        case cxc::TokenType::U8:               str = "u8"; break;
        case cxc::TokenType::U16:              str = "u16"; break;
        case cxc::TokenType::U32:              str = "u32"; break;
        case cxc::TokenType::U64:              str = "u64"; break;
        case cxc::TokenType::I8:               str = "i8"; break;
        case cxc::TokenType::I16:              str = "i16"; break;
        case cxc::TokenType::I32:              str = "i32"; break;
        case cxc::TokenType::I64:              str = "i64"; break;
        case cxc::TokenType::F16:              str = "f16"; break;
        case cxc::TokenType::F32:              str = "f32"; break;
        case cxc::TokenType::F64:              str = "f64"; break;
        case cxc::TokenType::Period:           str = "period"; break;
        case cxc::TokenType::Quote:            str = "quote"; break;
        case cxc::TokenType::Minus:            str = "minus"; break;
        case cxc::TokenType::Plus:             str = "plus"; break;
        case cxc::TokenType::Colon:            str = "colon"; break;
        case cxc::TokenType::LeftParenthesis:  str = "left_parenthesis"; break;
        case cxc::TokenType::RightParenthesis: str = "right_parenthesis"; break;
        case cxc::TokenType::LeftAngle:        str = "left_angle"; break;
        case cxc::TokenType::RightAngle:       str = "right_angle"; break;
        case cxc::TokenType::Comma:            str = "comma"; break;
        case cxc::TokenType::Semicolon:        str = "semicolon"; break;
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
        string value;
        switch (t.type) {
        case cxc::TokenType::Identifier:   value = std::format("{}"    , t.value.identifier); break;
        case cxc::TokenType::String:       value = std::format("\"{}\"", t.value.string); break;
        case cxc::TokenType::Character:    value = std::format("\'{}\'", t.value.character); break;
        case cxc::TokenType::UInteger:     value = std::format("{}"    , t.value.uinteger); break;
        case cxc::TokenType::Floating:     value = std::format("{}"    , t.value.floating); break;
        case cxc::TokenType::ForwardArrow: value = "->"; break;
        default:
            if (to_underlying(t.type) > 0) {
                value = to_underlying(t.type);
            } else {
                value = std::format("{}", t.type);
            }
        }
        auto format = std::format(
            "token {{ type = {}, value = {}, location = {} }}", t.type, value, t.location);
        return formatter<string>::format(format, ctx);
    }
};

}
