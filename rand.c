
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main (void)
{
  uintptr_t s = ( uintptr_t ) main;
  srand( ( uint32_t ) s );
  
  int times = 10;

  while( times-- )
    {
      int r = rand();
      printf("%d\n", r);
      //srand( r );
    }
  
}
