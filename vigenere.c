
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN_ALPHABET 26

void chiper(char p, char k)
{
  char offset;
  if (p >= 'a')
    offset = 'a';
  else
    offset = 'A';

  p -= offset;
  k = (k >= 'a') ? k - 'a' : k - 'A';

  printf("%c", offset + ((p + k) % LEN_ALPHABET));
}

int main (int argc, char **arg)
{

  int i;
  size_t key_len;
  char *msg, *p;
  char *key;
  char crap;

  printf("Informe a mensagem:\n");
  scanf("%m[^\n]%c", &msg, &crap);

  printf("Informe a chave:\n");
  scanf("%m[^\n]", &key);

  key_len = strlen(key);

  i = 0;
  p = msg;
  while(*p)
  {
    chiper( *p++, *(key + i) );
    i = (i + 1) % key_len;
  }
  putchar('\n');

  free(msg);
  free(key);

  return 0;
}
