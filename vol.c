// ENUNCIADO: .Em um ano de eleições municipais, dois candidatos disputam
// a prefeitura de Bicas: Número: 1 - Candidato João e Número 2 - Candidato Marta.
// Elabore um programa que permita ao eleitor votar nos candidatos, uma espécie
// de urna eletrônica. O eleitor poderá votar também em branco e, até mesmo,
// anular seu voto digitando um número que não seja de um dos candidatos.
// Ao final do dia, o presidente da seção deverá digitar o código 017000569999
// para encerrar a votação. O programa deve então, emitir o boletim de urna
// contendo: i) Total de Votos; ii) Total de Votos Brancos; iii) Total de Votos
// Nulos; iv) Total de Votos do Candidato Número 1; Percentagem de Votos
// do Candidato 1; Total de Votos do Candidato 2; Percentagem de Votos do Candidato

#include <stdio.h>

#define COD_STOP 17000569999

struct candidato
{
  char *name;
  int votos;
};

static void
show_resume ( struct candidato *candidatos, int tot_candidatos,
              unsigned int vt_total, unsigned int vt_branco, unsigned vt_nulo )
{
  printf( "\nTotal de votos: %u\n"
          "Votos em branco: %d - %.1f%%\n"
          "Votos nulos:     %d - %.1f%%\n\n",
           vt_total,
           vt_branco, ( ( float ) vt_branco / vt_total * 100 ),
           vt_nulo, ( ( float ) vt_nulo / vt_total * 100) );

  int i = 0;
  struct candidato *candidato = candidatos;

  while( i++ < tot_candidatos )
    {
      printf("Candidado %s - %d votos - %.1f%%\n",
              candidato->name,
              candidato->votos,
              ( (float) candidato->votos / vt_total ) * 100 );

      candidato++;
    }
}

static inline void
show_candidates ( void )
{
  printf("\n"
         "1 - Joao\n"
         "2 - Marta\n"
         "3 - Branco\n"
         "> ");
}

int main( void )
{

  struct candidato candidatos[] =
    {
      { .name = "Joao", .votos = 0 },
      { .name = "Marta", .votos = 0 }
    };

  unsigned int votos_total = 0, votos_nulos = 0, votos_branco = 0;

  // variavel de 64 bits
  // tamanho necessario para armazenar a "senha" de parada,
  // poderiamos trabalhar com string, porem gastaria mais memoria
  unsigned long long int voto;
  do
    {
      show_candidates();

      if ( 1 != scanf( "%llu", &voto) )
        {
          scanf( "%*[^\n]%*c" ); // clear stdin
          voto = 0; // voto nulo, qualquer valor diferente de 1 ou 2
        }

      if ( voto > 0 && voto < 3 )
        candidatos[ voto - 1 ].votos++;
      else if ( voto == 3 )
        votos_branco++;
      else
        votos_nulos++;

      votos_total++;
    }
  while( voto != COD_STOP );

  // desconsidera o voto computado pela "senha" de parada
  votos_total--, votos_nulos--;

  // calcula os dados e exibe
  show_resume ( candidatos,
                sizeof candidatos / sizeof candidatos[0],
                votos_total,
                votos_branco,
                votos_nulos );

  return 0;
}
