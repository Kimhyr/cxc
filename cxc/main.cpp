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

#include <cxc/compiler/lexer.h>

using namespace cxc;

int main(int argc, char* argv[])
{
  (void)argc, (void)argv;
 
  lexer l;
  token t;
  char const* root = "/home/k/Projects/cxc";
  l.load(std::format("{}/tests/main.cx", root).c_str());
  l.next(t);

  return 0;
}
