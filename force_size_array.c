#include <stdio.h>

#define LEN_ARRAY 3

void f( int(*a)[LEN_ARRAY] )
{
  int i = LEN_ARRAY;
  while(i--)
    printf("%d\n", (*a)[i]  );
}

int main()
{
  
  int ar[] = {1, 2, 3}; 

  f(&ar);
  return 0;
}
