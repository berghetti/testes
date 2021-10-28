
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

struct mydata
{
  int a;
  int b;
};

int main( void )
{

  struct mydata data1 = { .a = 1, .b = 2 }, data2 = { .a = 5, .b = 10 };

  // vector *v = vector_new( sizeof (struct mydata) );
  vector v = VECTOR_INIT( sizeof (struct mydata) );

  vector_push( &v, &data1 );
  vector_push( &v, &data2 );

  size_t len = VECTOR_SIZE(v);
  struct mydata *d = vector_buffer(&v);

  for ( size_t i = 0; i < len; i++ )
    {
      printf("%d %d\n", d[i].a, d[i].b );
    }


  return 0;
}
