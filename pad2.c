#include <unistd.h>
#include <curses.h>
#include <stdlib.h>

int main()
{
    WINDOW *pad_ptr;
    int x, y;
    int pad_lines;
    int pad_cols;
    char disp_char;

    initscr();

    pad_lines = LINES + 50;
    pad_cols = COLS + 50;

    pad_ptr = newpad(pad_lines, pad_cols);
    printw("pad_lines %d\npad_cols %d\n", pad_lines, pad_cols);	
    refresh();

    disp_char = 'a';
    for(x = 0; x < pad_lines; x++)
    {
            for(y = 0; y < pad_cols; y++)
            {
                    mvwaddch(pad_ptr, x, y, disp_char);
                    if(disp_char == 'z')
                            disp_char = 'a';
                    else
                            disp_char++;
            }

	//disp_char++;
	//if (disp_char == 'z') disp_char = 'a';
    }

/* We just filled the pad with letters from the alphabet. */

/* Now we will fill part of the main window with a 10x10 section
 * of the pad.
 * Notice that the text does not wrap around. (Where is 'k'?)
 */

   // prefresh(pad_ptr, 0, 0, 3, 3, 9+3, 9+3);
   // sleep(3000);
	
   // parametros
   // 1, avança eixo y
   // 2, avança eixo x
   // 3, quantidade de linhas para pular antes de exibir
   // 4, quantidade de colunas para pular antes de exibir
   // 5, quantidade de linhas para exibir
   // 6, quantidade de colunas para exibir

   // parametro 4 e 6, se correlacionam
    for (x=0; x<100; x++) {
    prefresh(pad_ptr, 0, x, 2, 5, 15, 7);
    wgetch(pad_ptr);
    }
//  prefresh(pad_ptr, LINES + 5, COLS + 7, 5, 5, 21, 19);
//  sleep(4);

    delwin(pad_ptr);

    endwin();

    exit(0);:
}
