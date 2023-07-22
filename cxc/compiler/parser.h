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

    ~Parser();

    [[nodiscard]] auto lexer() const noexcept -> Lexer const& { return m_lexer; }

    auto load(stdfs::path const& file) -> void;
    auto next() -> SyntaxTree const&;

private:
    Lexer m_lexer;

    auto parse(Syntax&) -> Syntax&;
    auto parse_value_declaration() -> ValueDeclaration&;
    auto parse_assignment() -> Assignment&;
    auto parse_identifier(Identifier&) -> void;
    auto parse_type(Type&) -> void;
};

class ParsingError
    : public std::runtime_error
{
public:
    using This = ParsingError;
    using Base = std::runtime_error;

    static constexpr auto INVALID_TOKEN         = "invalid token";
    static constexpr auto INCOMPLETE_ASSIGNMENT = "incomplete assignment";

    ParsingError(char const* s)
        : Base{s} {}

    [[nodiscard]] auto what() const noexcept -> char const* override;
};

}
