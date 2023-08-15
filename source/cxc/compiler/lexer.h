#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <vector>
#include <unordered_map>

#include <cxc/diagnostic.h>
#include <cxc/syntax/token.h>
#include <cxc/utility/type_traits.h>

namespace stdfs = std::filesystem;

namespace cxc
{ 

class Lexer
{
public:
    static const std::unordered_map<std::string_view, TokenType> WORDS;

    Lexer() noexcept = default;
    Lexer(Lexer const&) noexcept = delete;
    Lexer(Lexer&&) noexcept = delete;

    auto operator=(Lexer const&) noexcept -> Lexer& = delete;
    auto operator=(Lexer&&) noexcept -> Lexer& = delete;

    ~Lexer() noexcept = default;

    [[nodiscard]] auto location() const noexcept -> FileLocation const& { return m_location; }
    [[nodiscard]] auto file() const noexcept -> File const& { return const_cast<Lexer&>(*this).file(); }
    [[nodiscard]] auto file_path() const noexcept -> stdfs::path const& { return const_cast<Lexer&>(*this).file_path(); }
    [[nodiscard]] auto position() const noexcept -> Position const& { return const_cast<Lexer&>(*this).position(); }
    [[nodiscard]] auto current() const noexcept -> char { return static_cast<char>(m_current); }

    auto load(stdfs::path const& file) -> void;
    auto next() -> Token;

private:
    FileLocation m_location = {
        .position = {
            .row = 1,
            .column = 2
        }
    };
    char m_current;
    std::byte m_padding[4];

    [[nodiscard]] auto file() noexcept -> File& { return m_location.file; }
    [[nodiscard]] auto file_path() noexcept -> stdfs::path& { return file().path; }
    [[nodiscard]] auto position() noexcept -> Position& { return m_location.position; }
    
    auto consume() -> void;
    auto unconsume() -> void;

    [[nodiscard]] auto peek() const noexcept -> char { return const_cast<Lexer*>(this)->file().stream.peek(); }

    auto next_string(Token&) -> void;
    auto next_character() -> bool;
    auto next_number(Token&) -> void;
};

}
