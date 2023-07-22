#pragma once

#include <filesystem>
#include <stdexcept>

#include <cxc/compiler/lexer.h>
#include <cxc/syntax.h>
#include <cxc/syntax/syntax_tree.h>

namespace stdfs = std::filesystem;

namespace cxc
{

class Parser
{
public:
    using This = Parser;

    Parser() = default;
    Parser(This const&) = delete;
    Parser(This&&) = delete;

    auto operator=(This const&) -> This& = delete;
    auto operator=(This&&) -> This& = delete;

    ~Parser() = default;

    [[nodiscard]] auto lexer() const noexcept -> Lexer const& { return m_lexer; }
    [[nodiscard]] auto token() const noexcept -> Token const& { return m_token; }

    auto load(stdfs::path const& file) -> void;
    auto next(Syntax&) -> void;

private:
    Lexer m_lexer;
    Token m_token;

    auto lex() -> void { m_lexer.next(m_token); }
};

class ParsingError
    : public std::runtime_error
{
public:
    using This = ParsingError;
    using Base = std::runtime_error;

    static constexpr auto INVALID_TOKEN         = "invalid token";
    static constexpr auto INCOMPLETE_ASSIGNMENT = "incomplete assignment";
    static constexpr auto ILLFORMED             = "illformed";

    ParsingError(char const* s)
        : Base{s} {}

    [[nodiscard]] auto what() const noexcept -> char const* override;
};

}
