#include <stdio.h>
#include <stdlib.h>

void
modifica_conteudo_ponteiro(int *p, int v)
{
  *p = v;
}

void
modifica_conteudo_ponteiro_ponteiro(int **p)
{
  // *p = (int *) 0xFFFFFFFFFFFF;
  // malloc vai retornar um endereço
  *p = malloc(sizeof(int));
}

int main(void)
{
  int x = 10;

  // p == ponteiro
  // pp == ponteiro para ponteiro
  int *p;
  int **pp;

  p = &x;
  pp = &p;

  printf( "%d \t\t- conteúdo de x\n", x );
  printf( "%p \t- endereço de x\n\n", &x );

  printf( "%p \t- endereço de p\n", &p );
  printf( "%p \t- conteúdo de p (o mesmo que endereço de x)\n", p );
  printf( "%d \t\t- conteúdo presente no endereço apontado por p (o mesmo que conteúdo de x)\n\n", *p );

  printf( "%p \t- endereço de pp (ponteiro para ponteiro para ponteiro, numca usei)\n", &pp );
  printf( "%p \t- conteúdo de pp (o mesmo que endereço de p)\n", pp );
  printf( "%p \t- conteúdo presente no endereço apontado por pp (o mesmo que endereço de x)\n", *pp );
  printf( "%d \t\t- valor presente no endereço apontado pelo endereço apontado por pp (o mesmo que conteúdo de x)\n\n\n\n", *(*pp) );

 /*
    endereços fictícios
    0xFFFA    endereço da variável x
    10        conteúdo da variável x

    0xFFFB    endereço da variável p
    0xFFFA    conteúdo da variável p  ( endereço da variavél x )
    10        conteúdo que possui no endereço armazenado por p ( conteúdo da variável x )

    0xFFFC    endereço da variável pp
    0xFFFB    conteúdo da variável pp ( endereço da variavél p )
    0xFFFA    conteúdo que possui no endereço armazenado por pp (o endereço da variável x )
    10        valor presente no endereço apontado pelo endereço apontado por pp

    logo se eu tenho
    int x; (uma variavel do tipo x)

    e quiser modificar o conteúdo de x em uma função, deve passar o endereço
    de x para que a função acessa essa posição de memoria e escreva um valor lá,
    então fica
  */
  modifica_conteudo_ponteiro(&x, 25);
  printf("%d \t\t- o valor de x foi modificado\n\n", x);

  /*
    mas se eu tenho
    int *p; (um ponteiro que armazena um endereço que contem um dado do tipo inteiro)

    e quiser modificar o conteudo de p, atenção eu disse o conteúdo de p
    e não o valor apontado pelo conteúdo de p,
    devemos da mesma forma passar o endereço de p, como p ja é um ponteiro e
    temos que pessar seu endereço para modificar seu conteudo, logo temos que passar
    um ponteiro para ponteiro
  */
  modifica_conteudo_ponteiro_ponteiro(&p);
  printf("%p \t- novo conteudo em p\n\n", p);

  /*
    se quiser modificar o valor presente na memória apontada por p,
    passamos o endereço dessa memória para uma função, ou seja, passamos
    o conteúdo de p.
  */

  modifica_conteudo_ponteiro(p, 76);
  printf("%d \t\t- novo valor presente no endereço de memória apontada por p\n\n", *p);

  /*
    se eu tenho
    int **pp (um ponteiro que armazena o endereço de um ponteiro que por sua vez aponta para a posição de memória que tem um tipo int)
    e quiser modificar seu conteudo,
    ou seja,
    se pp tem o valor 0xFFFF, e eu quiser colocar o valor 0xAAAA
    preciso passar seu endereço (&pp)
    mas esse nivel de apontamento não é muito utilizado,

    é mais comum precisarmos alterar o valor que o endereço de pp aponta,
    do que o contéudo de pp propriamente,
    logo passamos esse endereço de memória para modificar,
    assim
  */

  pp = &p;
  printf("%p \t- novo conteudo de pp\n\n", pp);
  printf("%p \t- novo conteúdo presente no endereço apontado por pp\n\n", *pp);
  printf("%d \t\t- novo valor presente no endereço apontado pelo endereço apontado por pp\n\n", *(*pp));


  return 0;
}
