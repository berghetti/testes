// gcc -o hash hash.c -DNDEBUG; ./hash
#include <stdio.h>
#include <stdlib.h> // rand
#include <time.h>

// keep power of two
#define T_SIZE 256

static int *hash_table[T_SIZE];
static int values[T_SIZE];

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
  return ( key * 2654435761 ) & (T_SIZE - 1);
}

unsigned int
hash2( unsigned int key )
{
  key = ( ( key >> 16) ^ key ) * 0x45d9f3b;
  key = ( ( key >> 16) ^ key ) * 0x45d9f3b;
  key = ( key>> 16 ) ^ key;
  return key & (T_SIZE - 1);
}

void
test ( unsigned int (*hash) (unsigned int ) )
{
  int i, hit = 0;
  unsigned int index;

  for ( i = 0; i < T_SIZE; i++ )
    {
      int *p = malloc( sizeof *p );
      *p = rand() % 4194304;
      values[i] = *p;                 // backup
      index = hash( *p );

      // not overwrite
      if ( ! hash_table[index] )
        hash_table[ index ] = p;
    }

  for ( i = 0; i < T_SIZE; i++ )
    {
      index = hash( values[i] );
      char *st = "(colision)";

      if ( values[i] == *hash_table[index] )
        {
          hit++;
          st = "";
        }

#ifndef NDEBUG
      printf( "%d - %d (index %u) %s\n",
              values[i], *hash_table[ index ], index, st );
#endif
    }

   printf("   Total values: %d\n"
          "   Total hits:   %d (%.1f%%)\n",
          T_SIZE, hit, ( (float) hit /  T_SIZE ) * 100 );
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

int
main( void )
{
  srand(time(NULL));

  unsigned int (*f[])( unsigned int ) = { hash0, hash1, hash2 };

  int i = 0;
  while ( i < sizeof f / sizeof (void *) )
    {
      printf("Test function hash%d\n", i);
      test( f[i] );
      free_hash_table();
      puts("\n");
      i++;
    }

  return 0;
}
