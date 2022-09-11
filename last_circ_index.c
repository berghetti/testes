#include <stdio.h>
#include <unistd.h>

struct idx
{
 unsigned int idx:3;
};

int main ( void )
{
  struct idx id = { 0 };

  while( 1 )
   {
     id.idx = id.idx << 1;
     printf("%d\n", id.idx );
     sleep(1);
   }
}

void g ( void )
 {
   unsigned int i = 0;

   while( 1 )
    {
      if ( i == 0 ) i = 5;

      printf("%d\n", i-- );

      sleep(1);
    }
 }
