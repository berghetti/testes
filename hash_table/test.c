
#include <stddef.h>
#include <assert.h>

#include "hashtable.h"

int cb_func( void *value, void *user_data)
{
  value = value; //ugly

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

  int values[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 0 };
  int *p = values;

  while(*p)
    {
      hashtable_set( ht, *p, p );
      p++;
    }

  assert( ht->nentries == sizeof values  / sizeof values[0] - 1 );
  assert( ht->nbuckets > ht->nentries );

  p = values;
  while(*p)
    {
      int *q = hashtable_get( ht, *p );
      assert(*q == *p);
      p++;
    }

  p = hashtable_remove( ht, values[0] );
  assert(*p == values[0]);
  assert(ht->nentries == sizeof values  / sizeof values[0] - 2);
  assert(hashtable_remove( ht, values[0] ) == NULL);

  int count = 0;
  assert( hashtable_foreach(ht, cb_func, &count) == 0 );
  assert( (size_t) count == ht->nentries );

  hashtable_destroy( ht );
  return 0;
}
