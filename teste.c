#include <ncurses.h>

int
main ( void )
{
  initscr ();

  // imprime o caractere corretamente, sem atributos
  addch ( ACS_LTEE );
  refresh ();

  getch ();

  // buffer que vai receber o conteudo da linha, detalhe para o tipo do dado
  // que armazena no mesmo byte o caracter e os atributos
  chtype line[COLS];

  // copia toda a linha da janela para line,
  // ou melhor copia até COLS - 1 caracteres na linha e coloca NULL no fim.
  mvinchnstr ( 0, 0, line, COLS );

  // volta o cursor para posição que desejamos alterar
  move ( 0, 0 );

  int i = 0;
  // esse loop só termina no fim da linha (NULL), e não até o fim do caracter,
  // caso a inteção for alterar apenas do caracter precisa limitar o loop de
  // outra forma...
  while ( line[i] )
    {
      // altera o byte e deixa ligado apenas os bits corresponde ao caracter,
      // o pulo do gato aqui é a macro A_ALTCHARSET, que corresponde aos bits
      // de caracteres extendidos referenciados pelas macros ACS_*
      line[i] &= A_CHARTEXT | A_ALTCHARSET;

      // depois do byte "limpo", adicionamos o atributo que queremos
      line[i] |= A_REVERSE;

      addch ( line[i++] );
    }

  refresh ();

  getch ();
  endwin ();

  return 0;
}
