
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

  struct mydata *vt_data = vector_new ( 0, sizeof *vt_data );

  vector_push( vt_data, &data1 );
  vector_push( vt_data, &data2 );

  for ( size_t i = 0; i < vector_size( vt_data ); i++ )
    {
      printf("%d %d\n", vt_data[i].a, vt_data[i].b );
    }

  vector_free( vt_data );

  int *vi = vector_new ( 0, sizeof ( int ) );

  for (size_t i = 0; i < 20; ++i)
    {
      vector_push( vi, &i );
    }

  for ( size_t i = 0; i < vector_size( vi ); i++ )
    {
      printf("%d\n", vi[i] );
    }

  vector_free( vi );

  return 0;
}
