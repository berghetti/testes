// gcc -o hash hash.c -DNDEBUG; ./hash
#include <stdio.h>
#include <stdlib.h> // rand
#include <time.h>

// keep power of two
#define T_SIZE 2048

#define TOT_VALUES 1501

static int *hash_table[T_SIZE];
static int values[TOT_VALUES];

// https://github.com/shemminger/iproute2/blob/main/misc/ss.c
unsigned int
hash0( unsigned int key )
{
  key = (key >> 24) ^ (key >> 16) ^ (key >> 8) ^ key;

  return key & (T_SIZE - 1);
}

// https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
unsigned int
hash1( unsigned int key )
{
  return key & (T_SIZE - 1);
}

unsigned int
hash2( unsigned int key )
{
  key = ( ( key >> 16) ^ key ) * 0x45d9f3b;
  key = ( ( key >> 16) ^ key ) * 0x45d9f3b;
  key = ( key >> 16 ) ^ key;
  return key & (T_SIZE - 1);
}

unsigned int
hash3( unsigned int key )
{
  return ( (key * 5) + 1) & (T_SIZE - 1);
}

unsigned int
hash4( unsigned int key )
{
  return (  ( (5381 << 5 ) + 5381) + key ) & (T_SIZE - 1);
}

unsigned int
hash5( unsigned int key )
{
  char *s = (char *) &key;

  unsigned long int h = 0;
  unsigned long int high;
  while ( *s )
    {
      h = (h << 4) + *s++;

      if ( ( high = ( h & 0xF0000000 ) ) )
        h ^= high >> 24;

      h &= ~high;
    }

  return h & (T_SIZE - 1);
}

void
test ( unsigned int (*hash) (unsigned int ) )
{
  int i, hit = 0;
  unsigned int index;

  for ( i = 0; i < TOT_VALUES; i++ )
    {
      int *p = malloc( sizeof *p );
      *p = values[i];
      index = hash( *p );

#ifndef NDEBUG
      char *st = "";
#endif

      // not overwrite
      if ( ! hash_table[index] )
        {
          hash_table[ index ] = p;
          hit++;
        }
      else
        {
          free(p);
#ifndef NDEBUG
          st = "(collision)";
#endif
        }

#ifndef NDEBUG
      printf( "%d - %d (index %u) %s\n",
              values[i], *hash_table[ index ], index, st );
#endif

    }

   printf("   Total values: %d\n"
          "   Table size:   %d\n"
          "   Total hits:   %d (%.1f%%)\n",
          TOT_VALUES, T_SIZE, hit, ( (float) hit /  TOT_VALUES ) * 100 );
}

void
free_hash_table( void )
{
  for ( int i = 0; i < T_SIZE; i++ )
    {
      free ( hash_table[ i ] );
      hash_table[i] = NULL;
    }
}

void
rand_values( void )
{
  for ( int i = 0; i < TOT_VALUES; i++ )
      values[i] = rand() % 50000;
}

int
main( void )
{
  srand(time(NULL));

  unsigned int (*f[])( unsigned int ) =
    {
     hash0, hash1, hash2, hash3, hash4, hash5
    };

  rand_values();

  unsigned long int i = 0;
  while ( i < sizeof f / sizeof (void *) )
    {
      printf("Test function hash%ld\n", i);
      test( f[i] );
      free_hash_table();
      puts("\n");
      i++;
    }

  return 0;
}
