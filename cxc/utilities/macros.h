#ifndef CXC_MACROS_H
#define CXC_MACROS_H

#define STRING(E) #E

#define PRAGMA(P) _Pragma(#P)
#define PUSH_DIAGNOSTIC      PRAGMA(GCC diagnostic push)
#define POP_DIAGNOSTIC       PRAGMA(GCC diagnostic pop)
#define IGNORE_DIAGNOSTIC(W) PRAGMA(GCC diagnostic ignored #W)

#endif
