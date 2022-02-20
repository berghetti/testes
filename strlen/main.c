
// gcc -o main_gcc main.c -Wall -Wextra -O2 -msse4.2 -msse4.1

#include <stdio.h>
#include <string.h>
#include <stddef.h>  // size_t
#include "../cycle_counting.h"

#define NOINLINE __attribute__ ( ( noinline ) )

NOINLINE size_t
str1 ( char *s )
{
  char *p = s;
  while ( *p ) p++;
  return p - s;
}

// https://elixir.bootlin.com/glibc/latest/source/sysdeps/i386/strlen.c#L22
NOINLINE size_t
str2 ( char *s )
{
  int cnt;

  asm( "cld\n"   /* Search forward.  */
       "repnz\n" /* Look for a zero byte.  */
       "scasb"   /* %0, %1, %3 */
       : "=c"( cnt )
       : "D"( s ), "0"( -1 ), "a"( 0 ) );

  return -2 - cnt;
}

NOINLINE size_t
str3 ( char *s )
{
  size_t r = 0;

  // need align?
  while ( !( ( uintptr_t ) s & 4 ) ) { s++; r++; }

  uint32_t *p = ( uint32_t * ) s;

  // assume little endian
  while ( 1 )
    {
      uint64_t mask = 0xff;
      #pragma GCC unroll 4 /* very bad whitout this */
      for ( unsigned int i = 0; i < sizeof ( uint32_t ); i++ )
        {
          if ( 0 == ( *p & mask ) )
            goto END;

          r++;
          mask <<= 8;
        }

      p++;
    }

END:
  return r;
}

NOINLINE size_t
str4 ( char *s )
{
  size_t r = 0;

  // need align?
  while ( !( ( uintptr_t ) s & 8 ) ) { s++; r++; }

  uint64_t *p = ( uint64_t * ) s;

  // assume little endian
  while ( 1 )
    {
      uint64_t mask = 0xff;
      #pragma GCC unroll 8 /* very bad whitout this */
      for ( unsigned int i = 0; i < sizeof ( uint64_t ); i++ )
        {
          if ( 0 == ( *p & mask ) )
            goto END;

          r++;
          mask <<= 8;
        }

      p++;
    }

END:
  return r;
}

#include <x86intrin.h>

// https://github.com/WojciechMula/simd-string/blob/master/strlen.cpp#L1
NOINLINE size_t
sse4_1 ( const char *s )
{
  size_t result = 0;

  const __m128i zeros = _mm_setzero_si128 ();
  __m128i *mem = ( __m128i * ) s;

  for ( /**/; /**/; mem++, result += 16 )
    {
      const __m128i data = _mm_loadu_si128 ( mem );
      const __m128i cmp = _mm_cmpeq_epi8 ( data, zeros );

      if ( !_mm_testc_si128 ( zeros, cmp ) )
        {
          const int mask = _mm_movemask_epi8 ( cmp );

          return result + ( ( mask ) ? __builtin_ctz ( mask ) : 0 );
          // return result + bits::get_first_bit_set(mask);
        }
    }

  return 0;
}

// https://github.com/WojciechMula/simd-string/blob/master/strlen.cpp#L26
NOINLINE size_t
sse4_2 ( const char *s )
{
  size_t result = 0;

  // __m128i * mem = reinterpret_cast<__m128i *>(const_cast<char*>(s));
  __m128i *mem = ( __m128i * ) s;
  const __m128i zeros = _mm_setzero_si128 ();

  for ( /**/; /**/; mem++, result += 16 )
    {
      const __m128i data = _mm_loadu_si128 ( mem );

      uint8_t mode =
              _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH | _SIDD_LEAST_SIGNIFICANT;

      // Note: pcmpstri return mask/index and set ALU flags. Intrinsics
      //       functions can return just single value (mask, particular
      //       flag), so multiple intrinsics functions have to be called.
      //
      //       The good news: GCC and MSVC merges multiple _mm_cmpXstrX
      //       invocations with the same arguments to the single pcmpstri
      //       instruction.
      //
      if ( _mm_cmpistrc ( data, zeros, mode ) )
        {
          const int idx = _mm_cmpistri ( data, zeros, mode );

          return result + idx;
        }
    }
}

#define STR( s ) #s
#define F( f ) { STR ( f ), ( size_t ( * ) ( char * ) ) f }

int
main ( int argc, char **argv )
{
  static struct functions
  {
    char *f_name;
    size_t ( *f ) ( char * );
  } funcs[] = {
          F ( strlen ),
          F ( str1 ),
          F ( str2 ),
          F ( str3 ),
          F ( str4 ),
          // F ( sse4_1 ),
          // F ( sse4_2 ),
  };

  // preload
  // the 'if' avoid code elimintation
  if ( !str1 ( "A" ) )
    return 1;

  // whitout this in gcc and option -O2 get segfault
  // unroll up to 20 times
  #pragma GCC unroll 20
  for ( unsigned int i = 0; i < sizeof funcs / sizeof funcs[0]; i++ )
    {
      counter_T cnt = BEGIN_TSC ();
      size_t r = funcs[i].f ( argv[1] );
      cnt = END_TSC ( cnt );

      printf ( "%s\t cycles - %ld \t result - %ld\n", funcs[i].f_name, cnt, r );
    }

  return 0;
}
