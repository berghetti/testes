#include <ncurses.h>

int main(void)
{
    int i = 2, height, width;
    WINDOW *new;

    initscr();
    getmaxyx(stdscr, height, width);
    new = newwin(height - 2, width - 2, 1, 1);

    scrollok(stdscr,TRUE);

    while(1)
    {
        printw( "%d - lots and lots of lines flowing down the terminal\n", i);
        ++i;
        refresh();
    }

    endwin();
    return 0;
}
