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

auto main(int argc, char* argv[]) -> int
{
    (void)argc, (void)argv;

    static constexpr char root[] = "/home/k/projects/cxc";
    std::string path = std::format("{}/tests/main.cx", root);
    
    cxc::Lexer l;
    cxc::Token t {};
    l.load(path.c_str());

    l.next(t);
    std::cout << std::format("{}", t) << std::endl;
    l.next(t);
    std::cout << std::format("{}", t) << std::endl;
    l.next(t);
    std::cout << std::format("{}", t) << std::endl;
    l.next(t);
    std::cout << std::format("{}", t) << std::endl;
    l.next(t);
    std::cout << std::format("{}", t) << std::endl;
    try {
        l.next(t);
        std::cout << std::format("{}", t) << std::endl;
    } catch (cxc::LexingError const& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
