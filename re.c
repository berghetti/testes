#include <stdio.h>
#include <ctype.h>
#include <stddef.h>

size_t duplicate_count( char* text) {
  int freq[128] = {0}, dups = 0;
  while (*text) dups += ++freq[tolower(*text++)] == 2;
  return dups;
}

size_t
duplicate_count2( char *text )
{
  int freq[128] = {0}, dups = 0;
  
  while( *text )
  {
    freq[tolower(*text)]++; // freq[0]++
    if ( freq[tolower(*text)] == 2 )
      dups++;

    text++;
  }

  return dups;
}

int
main(void)
{
  char *text = "abbbc";

  printf("duplicate_count  - %ld\n"
         "duplicate_count2 - %ld\n",
          duplicate_count(text),
          duplicate_count2(text));

  return 0;
}
