#include <cctype>
#include <exception>
#include <locale>
#include <compare>
#include <stdexcept>
#include <format>
#include <fstream>
#include <sstream>

#include <cxc/compiler/lexer.h>
#include <cxc/utility/character.h>
#include <cxc/utility/macros.h>
#include <cxc/diagnostic.h>

namespace stdfs = std::filesystem;

namespace cxc
{
    void Lexer::load(stdfs::path const& path)
    {
        if (!stdfs::exists(path)) {
            throw std::invalid_argument(std::format(
                "file path doesn't exist: {}", path.string()));
        }

        if (!stdfs::is_regular_file(path)) {
            throw std::invalid_argument(std::format(
                "file path doesn't lead to a regular file: {}", path.string()));
        }

        std::ifstream f{path};
        if (f.fail()) {
            throw std::runtime_error(std::format(
                "an unknown failure happend from trying to open the file: {}", path.string()));
        }

        m_file_path = path;

        std::stringstream ss;
        ss << f.rdbuf();
        auto const& str{ss.str()};
        m_iterator = new char[str.size() + 1];
        const_cast<char&>(m_iterator[str.size()]) = '\0';
        std::copy(str.begin(), str.end(), const_cast<char*>(m_iterator));
        return;
    }

    void Lexer::next(Token& t)
    {
        while (std::isspace(current())) [[likely]] {
            consume();
        }

        t = Token {
            .location = {
                .path = file_path(),
                .position = {
                    .row = position().row,
                    .column = position().column - 1
                }
            }
        };

        auto ctt = static_cast<TokenType>(current());
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
        switch (ctt) {
#pragma GCC diagnostic pop
        case TokenType::Quote:
            consume();
            t.type = TokenType::String;
            next_string(t);
            break;
        case TokenType::Period:
            if (std::isdigit(peek(1))) {
                t.type = TokenType::Floating;
                next_number(t);
                break;
            }
            [[fallthrough]];
        case TokenType::Plus:
        case TokenType::Minus:
        case TokenType::Colon:
        case TokenType::LeftParenthesis:
        case TokenType::RightParenthesis:
        case TokenType::LeftAngle:
        case TokenType::RightAngle:
        case TokenType::Comma:
        case TokenType::Semicolon:
            t.type = static_cast<TokenType>(ctt);
            break;
        default:
            if (std::isdigit(current())) {
                t.type = TokenType::UInteger;
                next_number(t);
                break;
            }

            if (std::isalpha(current()) || current() == '_') {
                t.type = TokenType::Identifier;

                std::vector<char> buf;
                do {
                    buf.push_back(current());
                    consume();
                } while (std::isdigit(current())
                      || std::isalpha(current())
                      || current() == '_');

                // We then relocate the buffer to a fitting area.
                auto size = buf.size() + 1;
                t.value.identifier = std::string_view{new char[size], size};
                const_cast<char&>(t.value.identifier[buf.size()]) = '\0';
                std::copy(buf.begin(), buf.end(), const_cast<char*>(t.value.identifier.begin()));
                break;
            }

            if (current() == '\0') {
                throw LexingError(LexingError::END_OF_FILE);
            }

            throw LexingError(LexingError::UNKNOWN_TOKEN);
        }
    }

    void Lexer::consume() noexcept
    {
        ++m_iterator;
        if (current() == '\n') {
            ++m_position.row;
            m_position.column = 0;
        }
        ++m_position.column;
    }

    char Lexer::peek(std::ptrdiff_t offset) const noexcept
    {
        return iterator()[offset];
    }

    void Lexer::next_string(Token& t)
    {
        std::stringstream buf;
        do {
            if (current() == '\\') {
                consume();
                match_escape_sequence(buf);
            } else {
                buf.put(current());
                consume();
            }

            if (current() == '\n') {
                throw LexingError(LexingError::NEWLINE_IN_STRING_TOKEN);
            }
        } while (current() != '"');
        consume();

        std::string_view view = buf.view();
        std::size_t size = view.size();
        t.value.string = std::string_view{
            new char[size], size
        }; 
        std::copy(view.begin(), view.end(), const_cast<char*>(t.value.string.data()));
    }

    void Lexer::match_escape_sequence(std::stringstream& buf)
    {
        std::stringstream buf2;
        std::uint64_t n;

        switch (current()) {
        case 'x':
            consume();
            if (std::isxdigit(current())) {
                buf2 << current();
                consume();
                if (std::isxdigit(current())) {
                    buf2 << current();
                    consume();
                }
            }

            buf2 >> std::hex >> n;
            if (buf2.fail()) {
                throw LexingError(LexingError::HEX_ESCAPE_SEQUENCE);
            }
            buf << n;
            return;
        case '\'':
        case '\"':
        case '\\':
            buf.put(current());
            consume();
            return;
        case 'a': buf.put('\a'); break;
        case 'b': buf.put('\b'); break;
        case 'f': buf.put('\f'); break;
        case 'n': buf.put('\n'); break;
        case 'r': buf.put('\r'); break;
        case 't': buf.put('\t'); break;
        case 'v': buf.put('\v'); break;
        default:
            if (isoctal(current())) {
                buf2.put(current());
                consume();
                if (isoctal(current())) {
                    buf2.put(current());
                    consume();
                    if (isoctal(current())) {
                        buf2.put(current());
                        consume();
                    }
                }

                buf2 >> n;
                if (buf2.fail()) {
                    throw LexingError(LexingError::DIGIT_ESCAPE_SEQUENCE);
                }
                buf << n;
                return;
            }
            throw LexingError(LexingError::UNKNOWN_ESCAPE_SEQUENCE);
        }
        consume();
    }

    void Lexer::next_number(Token& t)
    {
        std::stringstream buf;
        do {
            buf.put(current());
            consume();

            if (current() == '.') {
                if (t.type == TokenType::Floating) {
                    throw LexingError(LexingError::EXTRA_DOT_IN_FLOATING_TOKEN);
                }

                t.type = TokenType::Floating;
                buf.put(current());
                consume();
                continue;
            }
        } while (std::isdigit(current()));
        if (t.type == TokenType::UInteger) {
            buf >> t.value.uinteger;
            if (buf.fail()) {
                throw LexingError(LexingError::FLOATING);
            }
        } else {
            buf >> t.value.floating;
            if (buf.fail()) {
                throw LexingError(LexingError::UINTEGER);
            }
        }
    }

    char const* LexingError::what() const noexcept
    {
            char const* m;
            switch (type()) {
            case UNKNOWN_TOKEN:
                m = "unknown token";
                break;
            case EXTRA_DOT_IN_FLOATING_TOKEN:
                m = "extra dot in floating token";
                break;
            case UINTEGER:
                m = "failed to make a uinteger token";
                break;
            case FLOATING:
                m = "failed to make a floating token";
                break;
            case NEWLINE_IN_STRING_TOKEN:
                m = "new line in string token";
                break;
            case HEX_ESCAPE_SEQUENCE:
                m = "failed to parse a hexadecimal escape sequence in a string token";
                break;
            case DIGIT_ESCAPE_SEQUENCE:
                m = "failed to parse a digital escape sequence in a string token";
                break;
            case UNKNOWN_ESCAPE_SEQUENCE:
                m = "unknown escape sequence";
                break;
            }
            return m;
    }
}
