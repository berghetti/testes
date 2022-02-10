
#include <ncurses.h>

int main( void )
{

  initscr();
	
 // long long int a = acs_map[ (unsigned char) 't' ];

  
  //printf("%lld\n", a);
  addch( 4194420 );
  //printw("Hello World !!!");	/* Print Hello World		  */
	refresh();			/* Print it on to the real screen */
	getch();			/* Wait for user input */
	endwin();			/* End curses mode		  */

  return 0;
}
