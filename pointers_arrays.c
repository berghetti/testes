#include <stdio.h>

int main(){
  
  char *a[2] = { "abc", "def" };

  // p is a pointer to a array of/with two pointer of chars
  char * (*p)[2];

  p = &a;

  printf("%zu\n", sizeof(*p) );

  printf("%s\n", *p[0]);

  printf("%s\n", p[0][1]);

  printf("%s\n", * ( *p + 1 ) );

  return 0;
}
