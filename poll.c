
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include <poll.h>

#define MAXBUFF 1024

int main()
{
    struct pollfd fds;
    fds.fd = 0;
    fds.events = POLLIN | POLLPRI;
    fds.revents = 0;

    char buff[MAXBUFF];
    int pr;
    printf("iniciando...");
    while (1)
    {
      pr = poll (&fds, 1, -1);
      if (pr > 0)
      {
        read(fileno(stdin), buff, MAXBUFF);
        printf("fd ativo\n");
      }

      if (pr == 0)
        printf("timeout\n");
      if (pr == -1)
        printf("error\n");
    }



    return EXIT_SUCCESS;
}
