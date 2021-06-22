
#include <stdio.h>


int my_strcmp(const char *str1, const char *str2)
{
  while ( *str1 && (*str1++ == *str2++) );

  return *str1 - *str2;
  
}

int main(int argc, char **argv)
{ 
  int r = my_strcmp ( argv[1], argv[2] );

  if (r > 0)
    puts("str1 maior");
  else if (r < 0)
    puts("str2 maior");
  else
    puts("str1 e str2 iguais");

  return 0;  
}
