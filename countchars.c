
#include <stdio.h>

#define SIZE_TBL_ASCII 127

int main(int argc, char **argv)
{
  char *txt = "Esse e um texto muito longo~";

  int counter[SIZE_TBL_ASCII] = { 0 };

  while(*txt)
     counter[ (int) *txt++ ]++;

  for ( int i = 0; i < SIZE_TBL_ASCII; i++ )
    {
      if ( !counter[i] || i == ' ' || i == '\n' || i == '\t' )
        continue;

      printf("%c - %d\n", (char) i, counter[i] );
    }

  return 0;
}
