#ifndef INCLUDE_DEBUG
#define INCLUDE_DEBUG

#include <stdio.h>

#ifndef NDEBUG
#define DEBUG(...) fprintf( stderr, __VA_ARGS__ )
#else
#define DEBUG(...) do{ } while ( 0 )
#endif

#endif // INCLUDE_DEBUG
