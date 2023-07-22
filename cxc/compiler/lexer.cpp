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

auto Lexer::load(stdfs::path const& path) -> void
{
    if (!stdfs::exists(path)) {
        throw std::invalid_argument(std::format(
            "file path doesn't exist: {}", path.string()));
    }

    if (!stdfs::is_regular_file(path)) {
        throw std::invalid_argument(std::format(
            "file path doesn't lead to a regular file: {}", path.string()));
    }

    file().stream.open(path);
    if (file().stream.fail()) {
        throw std::runtime_error(std::format(
            "an unknown failure happend from trying to open the file: {}", path.string()));
    }

    if (file().stream.eof()) {
        throw LexingError(LexingError::END_OF_FILE);
    }

    m_current = file().stream.get();

    file_path() = path;
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
    switch (ctt) {
    case TokenType::Quote:
        consume();
        t.type = TokenType::String;
        next_string(t);
        break;
    case TokenType::Period:
        if (std::isdigit(peek())) {
            t.type = TokenType::Floating;
            next_number(t);
            break;
        }
        [[fallthrough]];
    case TokenType::Apostrpohe:
        consume();
        t.type = TokenType::Character;
        next_character(t);
        break;
    case TokenType::Plus:
    case TokenType::Minus:
    case TokenType::Colon:
    case TokenType::LeftParenthesis:
    case TokenType::RightParenthesis:
    case TokenType::LeftAngle:
    case TokenType::RightAngle:
    case TokenType::Comma:
    case TokenType::Semicolon:
        consume();
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
            } while (std::isalpha(current()) || current() == '_' || std::isdigit(current()));

            std::string_view view{buf.data(), buf.size()};
            if (std::string_view{"value"} == view) {
                t.type = TokenType::Value;
                return;
            } else if (std::string_view{"function"} == view) {
                t.type = TokenType::Function;
                return;
            } else if (std::string_view{"u8"} == view) {
                t.type = TokenType::U8;
               return;
            } else if (std::string_view{"u16"} == view) {
                t.type = TokenType::U16;
               return;
            } else if (std::string_view{"u32"} == view) {
                t.type = TokenType::U32;
               return;
            } else if (std::string_view{"u64"} == view) {
                t.type = TokenType::U64;
               return;
            } else if (std::string_view{"i8"} == view) {
                t.type = TokenType::I8;
               return;
            } else if (std::string_view{"i16"} == view) {
                t.type = TokenType::I16;
               return;
            } else if (std::string_view{"i32"} == view) {
                t.type = TokenType::I32;
               return;
            } else if (std::string_view{"i64"} == view) {
                t.type = TokenType::I64;
                return;
            } else if (std::string_view{"f16"} == view) {
                t.type = TokenType::F16;
                return;
            } else if (std::string_view{"f32"} == view) {
                t.type = TokenType::F32;
                return;
            } else if (std::string_view{"f64"} == view) {
                t.type = TokenType::F64;
                return;
            }

            auto size = buf.size() + 1;
            t.value.identifier = std::string_view{new char[size], size};
            const_cast<char&>(t.value.identifier[buf.size()]) = '\0';
            std::copy(buf.begin(), buf.end(), const_cast<char*>(t.value.identifier.begin()));
            break;
        }

        if (file().stream.eof()) {
            throw LexingError(LexingError::END_OF_FILE);
        }

        throw LexingError(LexingError::UNKNOWN_TOKEN);
    }
}

auto Lexer::consume() -> void
{
    if (file().stream.eof()) {
        throw LexingError(LexingError::END_OF_FILE);
    }

    m_current = file().stream.get();
    if (current() == '\n') {
        ++position().row;
        position().column = 0;
    }
    ++position().column;
}

auto Lexer::peek() const noexcept -> int
{
    return const_cast<This*>(this)->file().stream.peek();
}

auto Lexer::next_string(Token& t) -> void
{
    std::stringstream buf;
    do {
        //if (current() == '\\') {
        //    consume();
        //    match_escape_sequence(buf);
        //} else {
            buf.put(current());
            consume();
        //}

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

auto Lexer::next_character(Token& t) -> void
{
    if (peek() != '\'') {
        throw LexingError(LexingError::DELIMITERLESS_CHARACTER_TOKEN);
    }

    t.value.character = current();
    consume(); // skip the character
    consume(); // skip the delimiter
}

[[deprecated("Moving this functionality into the code generator.")]]
auto Lexer::match_escape_sequence(std::stringstream& buf) -> void
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

auto Lexer::next_number(Token& t) -> void
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

[[nodiscard]]
auto LexingError::what() const noexcept -> char const*
{
    return Base::what();
}

}
