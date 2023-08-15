// Copyright 2023 King E. Lanchester
#include "lexer.h"

#include <cctype>

#include <exception>
#include <locale>
#include <compare>
#include <stdexcept>
#include <format>
#include <fstream>
#include <sstream>

#include <cxc/utility/character.h>
#include <cxc/utility/macros.h>
#include <cxc/diagnostic.h>

namespace stdfs = std::filesystem;

namespace cxc
{

const std::unordered_map<std::string_view, TokenType> Lexer::WORDS = {
    { "module", TokenType::Module },
    { "trait", TokenType::Trait },
    { "type", TokenType::Type },
    { "extend", TokenType::Extend },
    { "function", TokenType::Function },
    { "value", TokenType::Value },

    { "use", TokenType::Use },
    { "return", TokenType::Return },

    { "byte", TokenType::Byte },
    { "byte1", TokenType::Byte1 },
    { "byte2", TokenType::Byte2 },
    { "byte4", TokenType::Byte4 },
    { "byte8", TokenType::Byte8 },
    { "byte16", TokenType::Byte16 },

    { "int", TokenType::Int },
    { "int1", TokenType::Int1 },
    { "int2", TokenType::Int2 },
    { "int4", TokenType::Int4 },
    { "int8", TokenType::Int8 },
    { "int16", TokenType::Int16 },

    { "float", TokenType::Float },
    { "float2", TokenType::Float2 },
    { "float4", TokenType::Float4 },
    { "float8", TokenType::Float8 },
    { "float16", TokenType::Float16 },
};

auto Lexer::load(stdfs::path const& path) -> void
{
    if (!stdfs::exists(path)) {
        throw std::invalid_argument(std::format(
            "file path doesn't exist: {}",
            path.string()));
    }

    if (!stdfs::is_regular_file(path)) {
        throw std::invalid_argument(std::format(
            "file path doesn't lead to a regular file: {}",
            path.string()));
    }

    file().stream.open(path);
    if (file().stream.fail()) {
        throw std::runtime_error(std::format(
            "an unknown failure happend from trying to open the file: {}",
             path.string()));
    }

    if (file().stream.eof()) {
        throw std::invalid_argument(std::format(
            "file is empty: {}",
            path.string()));
    }

    m_current = file().stream.get();
    file_path() = path;
    return;
}

auto Lexer::next() -> Token
{
    while (std::isspace(current())) [[likely]] {
        consume();
    }

    Token token = {
        .location = {
            .path = file_path(),
            .position = {
                .row = position().row,
                .column = position().column - 1
            }
        }
    };

    auto token_type = static_cast<TokenType>(current());
    bool is_escaped;
    switch (token_type) {
    case TokenType::QuestionMark:
        token.type = TokenType::StringLiteral;
        next_string(token);
        return token;
    case TokenType::FullStop:
        if (std::isdigit(peek())) {
            token.type = TokenType::FloatingLiteral;
            next_number(token);
            break;
        }
        [[fallthrough]];
    case TokenType::Apostrophe:
        is_escaped = next_character(); 
        if (current() == '\'' && !is_escaped) {
            token.value.character = 0;
            consume();
            return token;
        }
        token.value.character = current();
        consume();
        if (current() != '\'') {
            throw std::invalid_argument{"incomplete character"};
        }
        token.type = TokenType::CharacterLiteral;
        return token;
    case TokenType::PlusSign:
    case TokenType::MinusSign:
        // TODO: integer types
    case TokenType::Colon:
    case TokenType::LeftParenthesis:
    case TokenType::RightParenthesis:
    case TokenType::LeftCurlyBracket:
    case TokenType::RightCurlyBracket:
    case TokenType::LeftAngleBracket:
    case TokenType::RightAngleBracket:
    case TokenType::Comma:
    case TokenType::Semicolon:
        consume();
        token.type = static_cast<TokenType>(token_type);
        break;
    default:
        if (std::isdigit(current())) {
            token.type = TokenType::ByteLiteral;
            next_number(token);
            return token;
        }

        if (std::isalpha(current()) || current() == '_') {
            token.type = TokenType::Identifier;
            std::vector<char> buf;
            do {
                buf.push_back(current());
                consume();
            } while (std::isalpha(current())
                  || current() == '_'
                  || std::isdigit(current()));

            std::string_view view{buf.data(), buf.size()};
            try {
                token.type = WORDS.at(view);
                return token;
            } catch (std::out_of_range const&) {}

            auto size = buf.size() + 1;
            token.value.identifier = std::string_view{new char[size], size};
            const_cast<char&>(token.value.identifier[buf.size()]) = '\0';
            std::copy(buf.begin(), buf.end(), const_cast<char*>(
                      token.value.identifier.begin()));
            return token;
        }

        if (file().stream.eof()) {
            token.type = TokenType::End;
            return token;
        }
    }

    throw std::invalid_argument{"unknown token"};
}

auto Lexer::consume() -> void
{
    if (file().stream.eof()) {
        throw std::out_of_range{"end of file"};
    }

    m_current = file().stream.get();
    if (current() == '\n') {
        ++position().row;
        position().column = 0;
    }
    ++position().column;
}

auto Lexer::next_string(Token& token) -> void
{
    std::stringstream buffer;

    bool is_escaped;
    for (;;) {
        is_escaped = next_character();
        if (current() == '"' && !is_escaped) {
            break;
        }
        buffer << current();
    }
    consume();

    std::string_view view = buffer.view();
    std::size_t size = view.size();
    token.value.string = std::string_view{new char[size], size}; 
    std::copy(view.begin(), view.end(), const_cast<char*>(
              token.value.string.begin()));
}

auto Lexer::next_character() -> bool
{
    consume();
    if (current() != '\\') {
        return false;
    }

    consume();
    switch (current()) {
    case '\\':
    case '\'':
    case '\"':
        return true;
    default:
        throw std::invalid_argument{"unknown escape sequence"};
    }
}

auto Lexer::next_number(Token& token) -> void
{
    std::stringstream buffer;
    do {
        buffer << current();
        consume();

        if (current() == '.') {
            if (token.type == TokenType::FloatingLiteral) {
                throw std::invalid_argument{"multiple decimal points in floating-point literal"};
            }

            token.type = TokenType::FloatingLiteral;
            buffer << current();
            consume();
            continue;
        }
    } while (std::isdigit(current()));
    if (token.type == TokenType::ByteLiteral) {
        buffer >> token.value.byte;
        if (buffer.fail()) {
            throw std::runtime_error{
                "failed to parse byte literal from buffer"};
        }
    } else {
        buffer >> token.value.floating;
        if (buffer.fail()) {
            throw std::runtime_error{
                "failed to parse floating-point literal from buffer"};
        }
    }
}

}
