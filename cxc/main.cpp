#ifndef __clang__
#  error use Clang as the compiler
#endif

#if defined(_WIN32) || defined(_WIN64)
# error use a UNIX-based operating system
#endif

int main(int argc, char* argv[])
{
  (void)argc, (void)argv; 
  return 0;
}
