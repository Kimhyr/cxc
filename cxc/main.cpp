#ifndef __clang__
#  error use Clang as the compiler
#endif

#if defined(_WIN32) || defined(_WIN64)
# error use a UNIX-based operating system
#endif

#include <filesystem>
#include <stdexcept>
#include <iostream>
#include <format>
#include <type_traits>

#include <cxc/compiler/lexer.hpp>

int main(int argc, char* argv[])
{
  (void)argc, (void)argv;
  static constexpr char root[] = "/home/k/Projects/cxc";
  std::string path = std::format("{}/tests/main.cx", root);
  
  cxc::lexer l;
  cxc::token t {};
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
  std::cout << t.value.string << std::endl;
  l.next(t);
  std::cout << std::format("{}", t) << std::endl;

  return 0;
}
