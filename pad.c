#include <ncurses.h>


int main ()
{

  initscr();

  chtype prt[1];
  ptr[0] = A | A_REVERSE;

  addch(ptr);

  printw("deu\n");
  refresh();
  getch();
  endwin();

}
