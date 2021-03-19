
#include <stdio.h>

int main(void)
{
  char name[60], address[60], city[50];
  int age;

  // lê 59 caracteres ou até encontrar '\n'
  // se tiver mais de 59 caracteres na entrada (stdin)
  // consume os caracteres restantes sem armazena-los
  printf("Nome: ");
  scanf("%59[^\n]%*[^\n]", name);

 // pula todos espaços em branco no bufffer,
 // (i.e. '\n' deixado na linha anterior),
 // depois faz a mesma leitura que a ultima chamada scanf
  printf("Endereco: ");
  scanf(" %59[^\n]%*[^\n]", address);

  // idem
  printf("Cidade: ");
  scanf(" %59[^\n]%*[^\n]", city);

  // o format '%d' pula por padrão os espaços em branco
  // no buffer (stdin) até encontrar o numero
  printf("Idade: ");
  scanf("%d", &age);

  printf("\n");
  printf("Nome: %s\n", name);
  printf("Endereco: %s\n", address);
  printf("Cidade: %s\n", city);
  printf("Idade: %d\n", age);

  return 0;
}
