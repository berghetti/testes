
#include <stdio.h>
#include <stdlib.h>

int f(void)
{
  puts("f");
  return 1;
}

int main(int argc, char **argv)
{
  char ar[ atoi(argv[1]) ];

  char ar2[2] = {0};

  printf("%d\n", sizeof(ar));

  printf("%d\n", sizeof ( ar2[f()] ));

  return 0;
}
