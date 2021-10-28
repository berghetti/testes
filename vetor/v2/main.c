
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

  int tot_data = 2;
  struct mydata data1 = { .a = 1, .b = 2 }, data2 = { .a = 5, .b = 10 };

  struct mydata *vt_data = vector_new ( 1, sizeof *vt_data );

  vector_push( &vt_data, &data1 );
  vector_push( &vt_data, &data2 );

  for ( int i = 0; i < tot_data; i++ )
    {
      printf("%d %d\n", vt_data[i].a, vt_data[i].b );
    }

  vector_free( vt_data );

  return 0;
}
