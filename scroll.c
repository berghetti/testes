
#include <ncurses.h>

int main(int argc, char **argv)
{
  initscr();
  WINDOW *j = newwin(LINES * 2, COLS, 0, 0);
  raw();        /* Line buffering disabled  */
  keypad(j, TRUE);   /* We get F1, F2 etc..    */
  noecho();     /* Don't echo() while we do getch */
  scrollok(j, FALSE);

  char str[3];

  int ch;
  while ('q' != (ch = wgetch(j)))
  {
    switch (ch)
    {
      case KEY_DOWN:
        scrollok(j, TRUE);
        wscrl(j, 1);
        scrollok(j, FALSE);
        break;

      case KEY_UP:
        scrollok(j, TRUE);
        wscrl(j, -1);
        scrollok(j, FALSE);
        break;

      default:
        //printw("The pressed key is ");
        sprintf(str, "%c\n", ch);


        return 0;
    }


