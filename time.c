
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

static volatile sig_atomic_t stop = 0;

void
sig_handler( int sig )
{
  stop = 1;
}

int
main( void )
{
  
  time_t t = time( NULL );
  if ( t < 0 )
    return 1;

  signal( SIGINT, sig_handler );

  while ( !stop )
    {
      int s = 1;
      while( s )
        s = sleep( s );  
    }

  t = time ( NULL ) - t;

  printf("\nSeconds %ld\n", t);

  return 0;
}
