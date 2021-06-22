dis
#include <stdio.h>
#include <pthread.h>

_Atomic var = 0;

void * func(void *arg)
{
  for (int i = 0; i < 1000000; i++)
    var++;

  return NULL;
}


int main(int argc, char **argv)
{
  int i;
  pthread_t t[2];

  for (i = 0; i < 2; i++)
    pthread_create( &t[i], NULL, func, NULL );


  for (i = 0; i < 2; i++)
    pthread_join( t[i], NULL );

  printf( "var is %d\n", var );
  return 0;
}
