#include <stdio.h>
#include <unistd.h>

#define SEC 45667

int main (void)
{

  int sec = 0,
      min = 0,
      hora = 0;


  hora = SEC / 3600;
  min = (SEC % 3600) / 60;
  sec = (SEC % 3600) % 60;


  printf("%d : %d : %d\n", hora, min, sec);



}
