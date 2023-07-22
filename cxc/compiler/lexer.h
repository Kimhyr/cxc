#pragma once

#include <cstdint>
#include <iterator>
#include <filesystem>
#include <stdexcept>
#include <vector>
#include <fstream>

#include <cxc/diagnostic.h>
#include <cxc/syntax/token.h>

namespace stdfs = std::filesystem;

namespace cxc
{ 

class Lexer
{
public:
    using This = Lexer;
    
    Lexer() noexcept = default;
    Lexer(This const&) noexcept = delete;
    Lexer(This&&) noexcept = delete;

    auto operator=(This const&) noexcept -> This& = delete;
    auto operator=(This&&) noexcept -> This& = delete;

    ~Lexer() noexcept = default;

    [[nodiscard]] auto location() const noexcept -> FileLocation const& { return m_location; }
    [[nodiscard]] auto file() const noexcept -> File const& { return const_cast<This*>(this)->file(); }
    [[nodiscard]] auto file_path() const noexcept -> stdfs::path const& { return const_cast<This*>(this)->file_path(); }
    [[nodiscard]] auto position() const noexcept -> Position const& { return const_cast<This*>(this)->position(); }
    [[nodiscard]] auto current() const noexcept -> char { return static_cast<char>(m_current); }

    auto load(stdfs::path const& file) -> void;
    auto next(Token& out) -> void;

private:
    FileLocation m_location = {
        .position = {
            .row = 1,
            .column = 2
        }
    };
    int          m_current;
    int          m_padding;

    [[nodiscard]] auto file() noexcept -> File& { return m_location.file; }
    [[nodiscard]] auto file_path() noexcept -> stdfs::path& { return file().path; }
    [[nodiscard]] auto position() noexcept -> Position& { return m_location.position; }
    
    auto consume() -> void;
    auto peek() const noexcept -> int;
    auto next_string(Token&) -> void;
    auto next_character(Token&) -> void;
    auto next_number(Token&) -> void;
    auto match_escape_sequence(std::stringstream&) -> void;
};

class LexingError
    : public std::runtime_error
{
public:
    using Base = std::runtime_error;

    static constexpr auto END_OF_FILE                   = "end of file";
    static constexpr auto UNKNOWN_TOKEN                 = "unknown token";
    static constexpr auto EXTRA_DOT_IN_FLOATING_TOKEN   = "extra dot in floating token";
    static constexpr auto UINTEGER                      = "failed to make a uinteger token";
    static constexpr auto FLOATING                      = "failed to make a floating token";
    static constexpr auto NEWLINE_IN_STRING_TOKEN       = "new line in string token";
    static constexpr auto HEX_ESCAPE_SEQUENCE           = "failed to parse a hexadecimal escape sequence in a string token";
    static constexpr auto DIGIT_ESCAPE_SEQUENCE         = "failed to parse a digital escape sequence in a string token";
    static constexpr auto UNKNOWN_ESCAPE_SEQUENCE       = "unknown escape sequence";
    static constexpr auto DELIMITERLESS_CHARACTER_TOKEN = "character token doesn't have delimiter";

    LexingError(char const* m) noexcept
        : Base{m} {}

    [[nodiscard]] auto what() const noexcept -> char const* override;
};

}

namespace std
{

template<>
struct iterator_traits<cxc::Lexer>
{
    using iterator_category = forward_iterator_tag;
    using value_type        = char;
    using pointer           = value_type*;
    using reference         = value_type&;
    using difference        = ptrdiff_t;
}; 

}
