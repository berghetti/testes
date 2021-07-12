
#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"

typedef struct mydata
{
  int a;
  int b;
} Mydata;

int
main(int argc, char **argv)
{

  hashtable_t *ht = hashtable_new();

  Mydata *data = malloc(sizeof *data * 2);
  data[0].a = 35798;
  data[0].b = 10;

  data[1].a = 14692;
  data[1].b = 7;

  hashtable_set( ht, data[0].a, &data[0] );
  hashtable_set( ht, data[1].a, &data[1] );

  Mydata *s = hashtable_get( ht, data[0].a );

  if ( s )
    {
      printf( "%d %d\n", s->a, s->b );
    }

  hashtable_free( ht, NULL );
  free( data );

  return 0;
}
