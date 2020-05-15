
/*
 *  Copyright (C) 2020 Mayco S. Berghetti
 *
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/*
 *  Copyright (C) 2020 Mayco S. Berghetti
 *
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


// editor de arquivo simples
// ex: ./prog file 2 "novo valor"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_STDIN 1024

int
main ( int argc, const char **argv )
{
  if ( argc < 2 )
    {
      fprintf ( stderr,
                "Usage %s <file> <line | default 1> <\"new string\" | stdin>\n"
                "Ex: %s file.c < licence\n",
                *argv,
                *argv );
      return EXIT_FAILURE;
    }

  const char *file = *++argv;

  // linha a ser modificada, 1 é o padrão
  const int mod_linha =
          ( *++argv ) ? isdigit ( **argv ) ? atoi ( *argv++ ) : 1 : 1;

  char *new_value;     // conteudo novo a ser inserido
  size_t new_value_l;  // tamanho do novo conteudo

  // pega novo valor por argumento
  if ( *argv )
    {
      new_value = ( char * ) *argv;
      new_value_l = strlen ( new_value );
    }
  // pega novo valor da entrada padrão
  else
    {
      size_t len_buffer = BUFF_STDIN;
      char *temp = calloc ( 1, len_buffer );
      if ( !temp )
        {
          perror ( "Falha alocação de memoria" );
          return EXIT_FAILURE;
        }

      char *line = NULL;
      size_t line_l = 0;
      size_t bytes_read = 0;
      size_t bytes_read_tmp;

      while ( ( bytes_read_tmp = getline ( &line, &line_l, stdin ) ) != -1 )
        {
          if ( bytes_read + bytes_read_tmp >= len_buffer )
            {
              len_buffer = ( bytes_read + bytes_read_tmp ) * 2;
              if ( !( temp = realloc ( temp, len_buffer ) ) )
                {
                  perror ( "Falha alocação de memoria" );
                  return EXIT_FAILURE;
                }
            }

          memcpy ( temp + bytes_read, line, bytes_read_tmp );
          bytes_read += bytes_read_tmp;
        }

      free ( line );
      new_value = temp;
      new_value_l = bytes_read;
    }

  int skip_line = mod_linha - 1;

  // coloca quebra de linha caso não tenha
  if ( new_value[new_value_l - 1] != '\n' )
    {
      new_value[new_value_l] = '\n';
      new_value_l++;
    }

  // abre o arquivo para leitura
  FILE *file_r = fopen ( file, "rb" );
  if ( !file_r )
    {
      perror ( "falha abrir arquivo" );
      return EXIT_FAILURE;
    }

  // vai para final do arquivo e pega quantos bytes arquivo possui
  fseek ( file_r, 0L, SEEK_END );
  long len_file_r = ftell ( file_r );

  char *buffer = calloc ( 1, len_file_r + new_value_l );

  // retornar para inicio do arquivo
  // e le os dados para o buffer
  rewind ( file_r );
  if ( ( fread ( buffer, 1, len_file_r, file_r ) ) < len_file_r )
    {
      perror ( "falha ao ler arquivo" );
      free ( buffer );
      fclose ( file_r );
      return EXIT_FAILURE;
    }

  fclose ( file_r );

  // pega quantidade de bytes até começar linha a ser modificada
  size_t pos_line;
  for ( pos_line = 0; pos_line < len_file_r && skip_line; pos_line++ )
    {
      if ( buffer[pos_line] == '\n' )
        skip_line--;
    }

  if ( skip_line )
    {
      fprintf ( stderr,
                "arquivo possui apenas %d linha(s)\n",
                mod_linha - skip_line - 1 );
      if ( *argv )
        free ( new_value );
      free ( buffer );
      fclose ( file_r );
      return EXIT_FAILURE;
    }

  // pega o tamanho da linha atual a ser modificada
  size_t len_cur_line = 0;
  while ( buffer[pos_line + len_cur_line++] != '\n' )
    ;

  // novo tamanho que o arquivo editado terá
  size_t write_len;
  if ( new_value_l == len_cur_line )
    {
      // copia os novos bytes por cima
      for ( size_t i = 0; i < new_value_l; i++ )
        buffer[i + pos_line] = new_value[i];

      write_len = len_file_r;
    }
  else if ( new_value_l > len_cur_line )
    {
      // desloca os dados para direita para "abrir espaço" para novo dado
      for ( size_t i = len_file_r; i > pos_line + len_cur_line - 1; i-- )
        buffer[i + ( new_value_l - len_cur_line )] = buffer[i];

      // copia os novos bytes
      for ( size_t i = 0; i < new_value_l; i++ )
        buffer[i + pos_line] = new_value[i];

      write_len = len_file_r + ( new_value_l - len_cur_line );
    }
  else if ( new_value_l < len_cur_line )
    {
      // desloca os dados para esquerda
      for ( size_t i = pos_line + len_cur_line; i < len_file_r; i++ )
        buffer[i - ( len_cur_line - new_value_l )] = buffer[i];

      // copia os novos bytes
      for ( size_t i = 0; i < new_value_l; i++ )
        buffer[i + pos_line] = new_value[i];

      write_len = len_file_r - ( len_cur_line - new_value_l );
    }

  // abre para escrita
  FILE *file_w = fopen ( file, "wb" );
  if ( !file_w )
    {
      perror ( "falha abrir arquivo" );
      free ( buffer );
      return EXIT_FAILURE;
    }

  if ( ( fwrite ( buffer, 1, write_len, file_w ) ) < write_len )
    {
      perror ( "falha ao salvar arquivo" );
      if ( !*argv )
        free ( new_value );

      free ( buffer );
      fclose ( file_w );
      return EXIT_FAILURE;
    }

  if ( !*argv )
    free ( new_value );

  free ( buffer );
  fclose ( file_w );

  return EXIT_SUCCESS;
}
