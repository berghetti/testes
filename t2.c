#include <stdio.h>
#include <stdlib.h>

int main(void){

    int listaCodigos [4], i;
    char candidatos[][20] = {"Joao", "Marta", "Voto Nulo", "Voto em Branco"};
    long int saida = 2; // alterado para variavel de 64 bits devido tamanho da chave de saida
    long int voto;      // idem
    int total_voto = 0;



    printf("Digite 1 para votar em Joao, 2 para votar em Marta\n3 para voto nulo e 4 para votar em branco\n\n");


    // poderia ter inicializado o array assim
    // int listaCodigos [4] = { 0 }; é mais rapido
    for(i = 0; i < 4; i++){
        listaCodigos[i] = 0;
    }

    // alterado para checar apenas pela senha de saida
    while( saida != 17000569999 ){
        printf("Digite seu voto. Para encerrar, digite o codigo da urna: ");
        scanf("%ld", &voto);  // alterado o especificador de formato
                              // aqui é necessario uma verificação no retorno de scanf
                              // experimente digitar uma letra para ver oque acontece...

        // inserido { chaves } no if, do jeito que estava apenas a primeira
        // linha dependia da condição para ser executada
        if(voto >= 1 && voto <= 4)
          {
            listaCodigos[voto - 1]++;
            total_voto++; // voto estava sendo incrementado de forma errada
          }

        saida = voto;
    }


    // alterado para exibir porcentagem tambem
    for(i = 0; i < 4; i++){
        printf("\nCandidato %s recebeu %d votos %.1f%%\n",
         candidatos[i],
         listaCodigos[i],
         ( (float) listaCodigos[i] / total_voto ) * 100 );
    }

    printf("\ntotal: %d\n\n", total_voto);

    return 0;
}
