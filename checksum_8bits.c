
#include <stdio.h>

int main(void)
{
  char msg[50];
  char soma = 0;
  char *m;

  printf("msg: ");
  scanf("%49s", msg);

  m = msg;
  while(*m)
    soma += *m++;

  printf("%#x\n", ~(soma) + 1);

  return 0;
}

