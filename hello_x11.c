
// apt install libx11-dev 
// cc -O2 -o hello hello_x11.c -lX11

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main ( void )
{
  Display *d;
  Window w;
  XEvent e;
  static const char msg[] = "Hello, World!";
  int s;

  // Pega o display default.
  d = XOpenDisplay ( NULL );

  if ( ! d )
  {
    fputs ( "Cannot open display\n", stderr );
    return EXIT_FAILURE;
  }

  s = DefaultScreen ( d );

  // Cria uma janela simples com base na janela raiz (desktop?).
  w = XCreateSimpleWindow ( d, 
                            RootWindow ( d, s ), 
                            10, 10,     // x,y
                            320, 240,   // width,height
                            1,          // borda de 1 px?
                            BlackPixel ( d, s ), 
                            WhitePixel ( d, s ) );

  // Seleciona os eventos para a janela.
  XSelectInput ( d, w, ExposureMask | KeyPressMask );

  // Associa a janela ao display.
  XMapWindow ( d, w );

  // loop de coleta de eventos.
  do
  {
    XNextEvent ( d, &e );   // Espera pelo próximo evento.

    // Exemplo de tratamento de evento (mensagens)!
    if ( e.type == Expose )
    {
                                               // x,  y,  w,  h
      XFillRectangle ( d, w, DefaultGC ( d, s ), 20, 20, 10, 10 );
                                            // x,  y
      XDrawString ( d, w, DefaultGC ( d, s ), 10, 50, msg, sizeof msg - 1 );
    }
  } while ( e.type != KeyPress );

  // Poderia ter:
  //   XUnmapWindow( d, w );
  //   XDestroyWindow( d, w );
  // Mas, XCloseDisplay() já toma conta disso.

  XCloseDisplay ( d );

  return EXIT_SUCCESS;
}
