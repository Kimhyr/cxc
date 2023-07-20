#pragma once

#include <cstdint>
#include <iterator>
#include <filesystem>
#include <stdexcept>
#include <vector>

#include <cxc/diagnostic.h>
#include <cxc/syntax/token.h>

namespace stdfs = std::filesystem;

namespace cxc
{ 
    class Lexer
    {
    public:
        Lexer() noexcept = default;
        Lexer(Lexer const&) noexcept = default;
        Lexer(Lexer&&) noexcept = default;

        Lexer& operator=(Lexer const&) noexcept = default;
        Lexer& operator=(Lexer&&) noexcept = default;

        ~Lexer() noexcept = default;

        [[nodiscard]] auto file_path() const noexcept -> stdfs::path const& { return m_file_path; }
        [[nodiscard]] auto iterator() const noexcept -> char const* { return m_iterator; }
        [[nodiscard]] auto position() const noexcept -> Position const& { return m_position; }
        [[nodiscard]] auto current() const noexcept -> char { return *iterator(); }
 
        void load(stdfs::path const& file);
        void next(Token& out);

    private:
        stdfs::path     m_file_path;
        char const*     m_iterator{nullptr};
        struct Position m_position{
            .row = 1,
            .column = 2, // this is 2 for a reason I can't explain
        };

        void consume() noexcept;
        char peek(std::ptrdiff_t) const noexcept;
        void next_string(Token&);
        void next_number(Token&);
        void match_escape_sequence(std::stringstream&);
    };

    class LexingError
        : public std::runtime_error
    {
    public:
        using base = std::runtime_error;

        enum Type : std::uintptr_t
        {
            END_OF_FILE,
            UNKNOWN_TOKEN,
            EXTRA_DOT_IN_FLOATING_TOKEN,
            FLOATING,
            UINTEGER,
            HEX_ESCAPE_SEQUENCE,
            DIGIT_ESCAPE_SEQUENCE,
            NEWLINE_IN_STRING_TOKEN,
            UNKNOWN_ESCAPE_SEQUENCE,
        };

        LexingError(Type type) noexcept
            : base{reinterpret_cast<char*>(type)} {}

        [[nodiscard]]
        auto type() const noexcept -> Type
        {
            auto* w{base::what()};
            return *reinterpret_cast<Type*>(&w);
        }

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
