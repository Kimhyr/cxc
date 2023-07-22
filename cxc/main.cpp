#ifndef __clang__
#    error use Clang as the compiler
#endif

#if defined(_WIN32) || defined(_WIN64)
# error use a UNIX-based operating system
#endif

#include <filesystem>
#include <stdexcept>
#include <iostream>
#include <format>
#include <type_traits>

#include <cxc/compiler/lexer.h>
#include <cxc/compiler/parser.h>

auto main(int argc, char* argv[]) -> int
{
    using namespace cxc;
    (void)argc, (void)argv;

    static constexpr char root[]{"/home/k/projects/cxc"};
    stdfs::path path{std::format("{}/tests/value.cx", root)};

    Parser parser{};
    parser.load(path);
    Syntax o;
    parser.next(o);
    std::cout << o.value().value->identifier.value << std::endl;
    return 0;
}
