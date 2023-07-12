#ifndef CXC_TOKEN_HPP
#define CXC_TOKEN_HPP

#include <cstdint>
#include <limits>
#include <format>
#include <type_traits>
#include <utility>

#include <cxc/utility/macros.hpp>
#include <cxc/utility/string.hpp>
#include <cxc/utility.hpp>
#include <cxc/diagnostic.hpp>

namespace cxc
{
  enum class token_type
    : std::int8_t
  {
    identifier = std::numeric_limits<std::int8_t>::min(),
    string,
    character,
    uinteger,
    floating,
    function, 
    value,
    u8,
    u16,
    u32,
    u64,
    i8,
    i16,
    i32,
    i64,
    f16,
    f32,
    f64,
    forward_arrow,
    end               = '\0',
    period            = '.',
    quote             = '"',
    minus             = '-',
    plus              = '+',
    colon             = ':',
    left_parenthesis  = '(',
    right_parenthesis = ')',
    left_angle        = '<',
    right_angle       = '>',
    comma             = ',',
    semicolon         = ';',
  };

  struct token
  {
    union {
      std::string_view identifier;
      char             character;
      std::string_view string;
      std::uint64_t    uinteger;
      double           floating;
    } value;
    location    location;
    token_type  type;
  };
}

namespace std
{
  template<>
  struct std::formatter<cxc::token_type>
    : formatter<string>
  {
    auto format(cxc::token_type i, format_context& ctx) const
    {
      std::string_view str;
      switch (i) {
      case cxc::token_type::identifier:        str = "identifier"; break;
      case cxc::token_type::character:         str = "character"; break;
      case cxc::token_type::string:            str = "string"; break;
      case cxc::token_type::uinteger:          str = "uinteger"; break;
      case cxc::token_type::floating:          str = "floating"; break;
      case cxc::token_type::forward_arrow:     str = "forward_arrow"; break;
      case cxc::token_type::function:          str = "function"; break;
      case cxc::token_type::value:             str = "value"; break;
      case cxc::token_type::u8:                str = "u8"; break;
      case cxc::token_type::u16:               str = "u16"; break;
      case cxc::token_type::u32:               str = "u32"; break;
      case cxc::token_type::u64:               str = "u64"; break;
      case cxc::token_type::i8:                str = "i8"; break;
      case cxc::token_type::i16:               str = "i16"; break;
      case cxc::token_type::i32:               str = "i32"; break;
      case cxc::token_type::i64:               str = "i64"; break;
      case cxc::token_type::f16:               str = "f16"; break;
      case cxc::token_type::f32:               str = "f32"; break;
      case cxc::token_type::f64:               str = "f64"; break;
      case cxc::token_type::end:               str = "end"; break;
      case cxc::token_type::period:            str = "period"; break;
      case cxc::token_type::quote:             str = "quote"; break;
      case cxc::token_type::minus:             str = "minus"; break;
      case cxc::token_type::plus:              str = "plus"; break;
      case cxc::token_type::colon:             str = "colon"; break;
      case cxc::token_type::left_parenthesis:  str = "left_parenthesis"; break;
      case cxc::token_type::right_parenthesis: str = "right_parenthesis"; break;
      case cxc::token_type::left_angle:        str = "left_angle"; break;
      case cxc::token_type::right_angle:       str = "right_angle"; break;
      case cxc::token_type::comma:             str = "comma"; break;
      case cxc::token_type::semicolon:         str = "semicolon"; break;
      }
      return std::formatter<string>::format(std::format("{}", str), ctx);
    }
  };

  template<>
  struct std::formatter<cxc::token>
    : formatter<string>
  {
    template<typename Ctx>
    auto format(cxc::token const& t, Ctx& ctx) const
    {
      string value;
PUSH_DIAGNOSTIC
IGNORE_DIAGNOSTIC(-Wswitch-enum)
      switch (t.type) {
POP_DIAGNOSTIC
      case cxc::token_type::identifier:    value = std::format("{}"    , t.value.identifier); break;
      case cxc::token_type::string:        value = std::format("\"{}\"", t.value.string); break;
      case cxc::token_type::character:     value = std::format("\'{}\'", t.value.character); break;
      case cxc::token_type::uinteger:      value = std::format("{}"    , t.value.uinteger); break;
      case cxc::token_type::floating:      value = std::format("{}"    , t.value.floating); break;
      case cxc::token_type::forward_arrow: value = "->"; break;
      default:
          if (to_underlying(t.type) > 0) {
            value = to_underlying(t.type);
          } else {
            value = std::format("{}", t.type);
          }
      }
      string format = std::format("token {{ type = {}, value = {}, location = {} }}", t.type, value, t.location);
      return formatter<string>::format(format, ctx);
    }
  };
}

#endif
