
#include <stdio.h>
#include <stdlib.h>

#include "sum.h"

int
main( int argc, char **argv )
{
  printf( "sum - %d\n", sum( atoi(argv[1] ) ) );

  return 0;
}
