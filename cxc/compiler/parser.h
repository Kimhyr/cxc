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

    auto consume() -> void { m_lexer.next(m_token); }
    auto next_value(Value&) -> void;
    auto next_identifier(Identifier&) -> void;
    auto next_type(Type&) -> void;
    auto next_initializer(Initializer&) -> void;
    auto next_expression(Syntax&) -> void;
};

class ParsingError
    : public std::runtime_error
{
public:
    using This = ParsingError;
    using Base = std::runtime_error;

    ParsingError()
        : Base{"parsing error"} {}

    [[nodiscard]] auto what() const noexcept -> char const* override;
};

struct Failure {};

}
