
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define DUP( code ) code, code 

int
main ( int argc, char **argv )
{

  int dummy;

  DUP(dummy);

}
