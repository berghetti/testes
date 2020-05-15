#include <curses.has#include <term.h>
#include <stdio.h>
#include <unistd.h>

int main(void){
  int cor;

  setupterm(NULL, STDOUT_FILENO, NULL);
  putp(exit_attribute_mode);
  for(cor=0; cor<max_colors; cor++){
    putp(tparm(set_a_foreground, cor));
    printf("Cor %d normal ", cor);
    putp(enter_bold_mode);
    printf("e \"bold\".\n");
    putp(exit_attribute_mode);
  }
  printf("Cor reset normal ");
  putp(enter_bold_mode);
  printf("e \"bold\".\n");
  putp(exit_attribute_mode);

  return 0;
} 
