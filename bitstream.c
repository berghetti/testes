#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define ARRAY_SIZE(a) (sizeof (a) / sizeof (*a))

/* converte um valor para sua representação binaria
 * value: valor a ser convertido;
 * i: largura, em bits, que o valor deve ser representado;
 * buffer: onde a representação binária será armazenada, deve ter pelo menos
          i bytes + 1 */
static void
int_to_bits (int value, int i, char *buffer)
{
  while (i--)
    *buffer++ = (bool)(value & (1U << i)) + '0';
}

static void
bitstream (int *values, int *bit_sizes, size_t tot_values, uint8_t *buffer)
{
  uint8_t *byte = buffer;
  int cur_bit = 0;

  while (tot_values--)
    {
      cur_bit += *bit_sizes++;

      // se o bit atual passa a fronteira de um byte,
      // adiciona os bits inferiores (i.e., parcial do proximo byte) no byte
      // atual e pula para o proximo byte
      while (cur_bit > 8)
        {
          cur_bit -= 8;
          *byte |= *values >> cur_bit;
          byte++;
        }

      // adiciona os valores conforme aparecem nos bits superiores do byte
      // atual
      *byte |= (uint8_t)*values++ << (8 - cur_bit);
    }
}

static void
print_buffer (uint8_t *bytes, size_t len)
{

  char buffer[9];
  buffer[8] = 0;
  uint8_t *p = bytes;

  int i = len;
  while (i--)
    {
      int_to_bits (*bytes++, 8, buffer);
      printf ("%s ", buffer);
    }
  putchar ('\n');

  i = len;
  while (i--)
    printf ("%#2x\t ", *p++);
  putchar ('\n');
}

int
main (int argc, char **argv)
{
  // int bit_sizes[] = { 3, 9, 4, 6, 14 };     // tamanhos de campos.
  // int values[] = { 2, 0x3d, 11, 40, 1234 }; // valores a serem codificados

  int bit_sizes[] = { 1, 2, 2, 3, 3, 3, 3, 4, 4, 4 }; // tamanhos de campos.
  int values[]
      = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }; // valores a serem codificados

  // calcula buffer com total de bytes necessários para armazenar os valores
  int total_bits = 0;
  for (int i = 0; i < ARRAY_SIZE (bit_sizes); i++)
    total_bits += bit_sizes[i];

  while (total_bits & (8 - 1))
    total_bits++;

  // VLA :(
  uint8_t buffer[total_bits / 8];

  bitstream (values, bit_sizes, ARRAY_SIZE (values), buffer);

  print_buffer (buffer, ARRAY_SIZE (buffer));

  return 0;
}
