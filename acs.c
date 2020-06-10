
#include <ncursesw/curses.h>

int main()
{
        initscr();
        //

        // printw("Upper left corner           "); addch(ACS_BSSB); printw("\n\n");
        // printw("Upper left corner           "); addch(ACS_LLCORNER); printw("\n\n");
        // printw("Upper left corner           "); addch(ACS_NEQUAL); printw("\n\n");
        // printw("Upper left corner           "); addch(ACS_BSSB); printw("\n\n");
        // printw("Upper left corner           "); addch(ACS_BSSB); printw("\n\n");
        printw("Upper left corner           "); addch(ACS_ULCORNER); printw("\n\n");
        printw("Lower left corner           "); addch(ACS_LLCORNER); printw("\n\n");
        printw("Lower right corner          "); addch(ACS_LRCORNER); printw("\n\n");
        printw("Tee pointing right          "); addch(ACS_LTEE); printw("\n\n");
        printw("Tee pointing left           "); addch(ACS_RTEE); printw("\n\n");
        printw("Tee pointing up             "); addch(ACS_BTEE); printw("\n\n");
        printw("Tee pointing down           "); addch(ACS_TTEE); printw("\n\n");
        printw("Horizontal line             "); addch(ACS_HLINE); printw("\n\n");
        printw("Vertical line               "); addch(ACS_VLINE); printw("\n\n");
        printw("Large Plus or cross over    "); addch(ACS_PLUS); printw("\n\n");
        printw("Scan Line 1                 "); addch(ACS_S1); printw("\n\n");
        printw("Scan Line 3                 "); addch(ACS_S3); printw("\n\n");
        printw("Scan Line 7                 "); addch(ACS_S7); printw("\n\n");
        printw("Scan Line 9                 "); addch(ACS_S9); printw("\n\n");
        printw("Diamond                     "); addch(ACS_DIAMOND); printw("\n\n");
        printw("Checker board (stipple)     "); addch(ACS_CKBOARD); printw("\n\n");
        printw("Degree Symbol               "); addch(ACS_DEGREE); printw("\n\n");
        printw("Plus/Minus Symbol           "); addch(ACS_PLMINUS); printw("\n\n");
        printw("Bullet                      "); addch(ACS_BULLET); printw("\n\n");
        printw("Arrow Pointing Left         "); addch(ACS_LARROW); printw("\n\n");
        printw("Arrow Pointing Right        "); addch(ACS_RARROW); printw("\n\n");
        printw("Arrow Pointing Down         "); addch(ACS_DARROW); printw("\n\n");
        printw("Arrow Pointing Up           "); addch(ACS_UARROW); printw("\n\n");
        printw("Board of squares            "); addch(ACS_BOARD); printw("\n\n");
        printw("Lantern Symbol              "); addch(ACS_LANTERN); printw("\n\n");
        printw("Solid Square Block          "); addch(ACS_BLOCK); printw("\n\n");
        printw("Less/Equal sign             "); addch(ACS_LEQUAL); printw("\n\n");
        printw("Greater/Equal sign          "); addch(ACS_GEQUAL); printw("\n\n");
        printw("Pi                          "); addch(ACS_PI); printw("\n\n");
        printw("Not equal                   "); addch(ACS_NEQUAL); printw("\n\n");
        printw("UK pound sign               "); addch(ACS_STERLING); printw("\n\n");

        refresh();
        getch();
        endwin();

	return 0;
}
