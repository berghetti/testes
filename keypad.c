
#include <stdio.h>

int main( void )
{

  char buf[5];
  fgets(buf, 5, stdin);

  printf("buff - %x\n", buf);
  
  return 0;
}
