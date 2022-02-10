
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "hashtable.h"

#define TOT_VALUES 147

int cb_func( hashtable_t *ht, void *value, void *user_data)
{
  value = value;

  (*(int *)user_data)++;

  return 0;
}

int main ( void )
{
  hashtable_t *ht = hashtable_new( NULL );

  assert(ht);
  assert(ht->nentries == 0);
  assert(ht->nbuckets > 0);

  assert( hashtable_get( ht, 1 ) == NULL );

  srand(time(NULL));

  // int values[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 0 };
  // int *p = values;

  int *values = malloc( TOT_VALUES * sizeof ( *values ));

  for (int i = 0; i < TOT_VALUES; i++)
    {
      int v = rand();
      values[i] = v;
      hashtable_set( ht, v, (void *)  ( ( size_t) v ) );
    }

  // assert( ht->nentries == sizeof values  / sizeof values[0] - 1 );
  assert( ht->nbuckets > ht->nentries );

  for (int i = 0; i < TOT_VALUES; i++)
    {
      int *q = hashtable_get( ht,  values[i]);
      // assert(q);
      // assert(*q == values[i] );
    }

  // p = hashtable_remove( ht, values[0] );
  // assert(p);
  // assert(*p == values[0]);
  // assert(ht->nentries == sizeof values  / sizeof values[0] - 2);
  // assert(hashtable_remove( ht, values[0] ) == NULL);

  int count = 0;
  assert( hashtable_foreach(ht, cb_func, &count) == 0 );
  assert( (size_t) count == ht->nentries );

  hashtable_destroy( ht );
  return 0;
}
