
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main (int argc, char **argv)
{

  int opt;
  while( (opt = getopt(argc, argv, "abcd")) != -1)
    {
      switch (opt)
        {
          case 'a':
            puts("a");
            break;
          case 'b':
            puts("b");
            break;
          case 'c':
            puts("c");
            break;
          case 'd':
            puts("d");
            break;
        }
    }

  printf("argc - %d\noptind - %d\n", argc, optind);

  printf("argv[0] - %s\n", argv[0]);
  printf("argv[1] - %s\n", argv[1]);
  printf("argv[2] - %s\n", argv[2]);
  printf("argv[3] - %s\n", argv[3]);
  printf("argv[4] - %s\n", argv[4]);
  return 0;
}
