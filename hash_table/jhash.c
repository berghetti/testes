
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "hashtable.h"
#include "jhash.h"
#include "../cycle_counting.h"

#define TOT_VALUES 10

static hash_t
cb_hash_int( const void *data )
{
  uint32_t value = (uint32_t) FROM_PTR( data );
  return jhash8( &value, 4, value );
  // return jhash32( &value, 1, value );
  // return 0;
}

static int
cb_compare( const void *key1, const void *key2)
{
  return key1 == key2;
}

static int
ht_print_debug( hashtable_t *ht, void *value, void *user_data)
{
  printf("value - %d\n", (int) FROM_PTR(value) );

  return 0;
}

void
test_int( void )
{
  hashtable_t *ht = hashtable_new( cb_hash_int, cb_compare, NULL );

  srand(time(NULL));

  int *values = malloc( TOT_VALUES * sizeof ( *values ));

  for (int i = 0; i < TOT_VALUES; i++)
    {
      // int v = rand();
      int v = i;
      values[i] = v;
      hashtable_set( ht, TO_PTR (v), TO_PTR( v ) );
    }

  for (int i = 0; i < TOT_VALUES; i++)
    {
      int *p = hashtable_get( ht,  TO_PTR( values[i] ) );
      assert((int) FROM_PTR(p) == values[i] );
    }

  hashtable_foreach( ht, ht_print_debug, NULL );

  free(values);
  hashtable_destroy( ht );
}

struct data
{
  int x;
  int y;
};

static hash_t
cb_hash_data( const void *key )
{
  return jhash8( key, 8, 0 );
  // return jhash32( key, 2, 0);
  // return key;
}

static int
cb_compare_data( const void *k1, const  void *k2 )
{
  struct data *d1, *d2;

  d1 = ( struct data *) k1;
  d2 = ( struct data *) k2;

  return d1->x == d2->x &&
         d1->y == d2->y;
}

void
test_data( void )
{
  hashtable_t *ht = hashtable_new( cb_hash_data, cb_compare_data, NULL);

  srand(time(NULL));

  struct data *values = malloc( TOT_VALUES * sizeof ( *values ));

  for (int i = 0; i < TOT_VALUES; i++ )
    {
      values[i].x = i;
      values[i].y = i * 2;
      hashtable_set( ht, values + i, values + i );
    }

  for (int i = 0; i < TOT_VALUES; i++)
    {
      // int key = hash_data(values + i );
      struct data *p = hashtable_get( ht, values + i );
      assert( p->y == values[i].y );
    }

  hashtable_destroy( ht );
}

int main( void )
{
  // test_int();
  test_data();

  int a[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  counter_T cnt;

  cnt = BEGIN_TSC();
  uint32_t hash = jhash8( a, sizeof(a), 1);
  cnt = END_TSC(cnt);

  printf("hash8: hash - %d - cycles - %ld\n", hash, cnt);

  cnt = BEGIN_TSC();
  hash = jhash32( (uint32_t *) a, sizeof(a) / sizeof(uint32_t), 1);
  cnt = END_TSC(cnt);

  printf("hash32: hash - %d - cycles - %ld\n", hash, cnt);


  return 0;
}
