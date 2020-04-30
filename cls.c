//
// example to clear terminal screen, based on the linux clear command
// compile: gcc cls.c -o cls -ltinfo
//
#include <stdio.h>    // putchar
#include <unistd.h>   // STDOUT_FILENO
#include <term.h>     // setupterm, putp, tigetstr, tputs


int main(void)
{
  int err;
  setupterm(NULL, STDOUT_FILENO, &err);

  switch (err)
    {
      case -2:
        fprintf(stderr, "unreadable terminal descriptor");
        return 1;
      case -1:
        fprintf(stderr, "no terminfo database");
        return 1;
      case 0:
        fprintf(stderr, "unknown terminal");
        return 1;
    }

  // get code escape to clear scroll
  char *E3 = tigetstr("E3");

  // first method
  putp(clear_screen);
  if (E3)
    putp(E3);

  // second method, skip putp()
  tputs(clear_screen, lines > 0 ? lines : 1, putchar);
  if (E3)
    (void) tputs(E3, lines > 0 ? lines : 1, putchar);

  return 0;
}
