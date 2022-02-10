
#include <stdio.h>
#include <time.h>
#include <stdint.h>

uint64_t
get_time ( void )
{
  clockid_t ci;

#ifdef CLOCK_MONOTONIC_COARSE
  ci = CLOCK_MONOTONIC_COARSE;
#else
  ci = CLOCK_MONOTONIC;
#endif

  struct timespec ts;
  if ( -1 == clock_gettime ( ci, &ts ) )
    return 0;

  return ts.tv_sec * 1000U + ts.tv_nsec / 1E6;
}

#define ONE_SEC 1000

int
main( void )
{
  
  uint64_t cur_time = get_time();

  while(1)
    {
      uint64_t new_time = get_time();

      if (new_time < cur_time)
         continue;

      uint32_t diff_time = new_time - cur_time;

      if ( diff_time >= ONE_SEC )
        {
          diff_time -= ONE_SEC;

          cur_time = new_time + diff_time;

          static int tick_tack = 0;
          printf( "%s\n", (tick_tack) ? "tack" : "tick" );
          tick_tack = !tick_tack;
        }
    }
  
  return 0;
}
