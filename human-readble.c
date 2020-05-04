// converte bits para formato legivel
// compile gcc -o human-readble human-readble.c

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>   // snprintf
#include <stdlib.h>  // atoi

// based in source code of program wget
// https://github.com/mirror/wget/blob/master/src/utils.c

// caso valor final obtido seja maior ou igual que accuracy
// não será impresso casas decimais
// 1023 é o melhor valor pois só não ira exibir casas decimais quando o fluxo
// for maior ou igual 1024 Tib ou TiB... :p
// caso o valor fosse 1000, poderiamos ter resultados como
// 1023 Kb/s, quando seria mais apropriado 1023.55 Kb/s
#define ACCURACY 1024

// numero de casas decimais que serão inseridas caso
// o valor seja menor que ACCURACY (maioria dos casos, creio que sempre)
#define DECIMAL_PLACES 2

#define BASE_IEC 1024
#define BASE_SI 1000

/// determina se um valor float é inteiro
// com base nas casas decimais (precision) fornecida
// @float n, numero a ter testado
// @int precision, casas decimais para testar
// @round, 1 arredonda, 0 não
int
is_integer ( float n, int precision, int round )
{
  // pow(10, precision)
  int mult = 1;
  for ( size_t i = 0; i < precision; i++ )
    mult *= 10;

  // example n == 1,005859 precision == 2
  // adiciona 0,005 em n, n == 1,010859 (caso arredondar)
  // multiplica n por 100, n == 101,0859
  // pega a parte inteira, n == 101
  // divide por 100, n == 1,01
  // return false, pois 1,01 != 1
  // caso não tivesse arredondado, retornaria true

  float temp;

  if ( round )
    temp = ( float ) ( ( int ) ( ( n + ( ( float ) 5 / ( mult * 10 ) ) ) *
                                 mult ) ) /
           mult;
  else
    temp = ( float ) ( ( int ) ( ( float ) n * mult ) ) / mult;

  return ( ( float ) temp == ( int ) n );
}

bool
human_readable ( char *buffer, const size_t len_buff, uint64_t bytes )
{
  const char *const sufix[] = {"b/s", "Kib/s", "Mib/s", "Gib/s", "Tib/s"};

  const size_t len_sufix = sizeof ( sufix ) / sizeof ( sufix[0] );

  // retorno da função snprintf
  ssize_t sn;

  // const double base = 976562E-4;
  const int base = BASE_IEC;

  // quantidade de bytes ou bits menor que 1024 ou 1000
  if ( bytes < ( uint64_t ) base )
    {
      sn = snprintf ( buffer, len_buff, "%d %s", ( int ) bytes, sufix[0] );
      return ( sn > 0 && ( size_t ) sn < len_buff );
    }

  int decimals;
  double val;
  bool round = true;
  bool integer = false;
  for ( size_t i = 1; i < len_sufix; i++ )
    {
      /* At each iteration N is greater than the *subsequent* power.
         That way N/1024.0 produces a decimal number in the units of
         *this* power.  */
      if ( ( val = ( double ) bytes / base ) < base || i == ( len_sufix - 1 ) )
        {
          integer = is_integer ( val, DECIMAL_PLACES, round );

          decimals = ( ( uint32_t ) val < ACCURACY )
                         ? !integer ? DECIMAL_PLACES : 0
                         : 0;

          /* Print values smaller than the accuracy level (acc) with (decimal)
           * decimal digits, and others without any decimals.  */
          sn =
              snprintf ( buffer, len_buff, "%.*f %s", decimals, val, sufix[i] );
          return ( sn > 0 && ( size_t ) sn < len_buff );
        }
      bytes /= base;
    }
  return false; /* unreached */
}

int
main ( int argc, const char **argv )
{
  if ( argc != 2 )
    {
      fprintf ( stderr,
                "Usage: %s bits\n"
                "Ex: %s 1024\n",
                argv[0],
                argv[0] );
      return EXIT_FAILURE;
    }

  char value[14];
  human_readable ( value, 14, atoi ( argv[1] ) );

  printf ( "%s\n", value );

  return EXIT_SUCCESS;
}
