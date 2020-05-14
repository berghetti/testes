
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PROG_NAME "tline"
#define VERSION "0.0.1"

#define BUFF_STDIN 1024

void
version ( void )
{
  printf ( PROG_NAME " - " VERSION "\n" );
}

void
usage ( void )
{
  version ();
  printf ( "Usage: " PROG_NAME
           " <file> <options> -d <\"novo conteudo\" | stdin>\n"
           "\n"
           "<options>\n"
           "-l <line>           especifica a linha do arquivo a ser alterada, "
           "1 é default\n"
           "-s                  troca o conteudo da linha atual pelo novo "
           "conteudo\n"
           "-d \"novo conteudo\"  conteudo que sera adicionado\n"
           "-h                  exibe essa mensagem\n"
           "-v                  exibe a versão\n"
           "\n"
           "Ex: " PROG_NAME
           " file.c < licence\n"
           "    " PROG_NAME
           " file.c -s -l 3 -d \"novo conteudo da linha 3\"\n" );
}

int
main ( int argc, char **argv )
{
  if ( argc < 2 )
    {
      usage ();
      return EXIT_FAILURE;
    }

  char *new_value;            // conteudo novo a ser inserido
  size_t new_value_l;         // tamanho do novo conteudo
  int read_stdin = 1;         // leitura pela entrada padrão é default
  int swap_content_line = 0;  // adicionar conteudo é default
  int mod_linha = 1;          // linha de referencia é a 1 por default

  int opt;
  while ( ( opt = getopt ( argc, argv, "sl:d:hv" ) ) != -1 )
    {
      switch ( opt )
        {
          case 's':
            swap_content_line = 1;
            break;
          case 'l':
            mod_linha = atoi ( optarg );
            break;
          case 'd':
            new_value = optarg;
            new_value_l = strlen ( new_value );
            read_stdin = 0;
            break;
          case 'h':
            usage ();
            return EXIT_SUCCESS;
          case 'v':
            version ();
            return EXIT_SUCCESS;
        }
    }

  // pega novo valor da entrada padrão
  if ( read_stdin )
    {
      size_t len_buffer = BUFF_STDIN;  // tamanho inicial
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
          // se o tamanho do conteudo em stdin for maior que o ultimo buffer
          // aloca mais memoria
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

  // coloca quebra de linha caso não tenha
  if ( new_value[new_value_l - 1] != '\n' )
    {
      new_value[new_value_l] = '\n';
      new_value_l++;
    }

  if ( argc == optind )
    {
      fprintf ( stderr, "file not specified" );
      return EXIT_FAILURE;
    }

  int skip_line = mod_linha - 1;

  // main loop
  char *file;
  for ( ; optind < argc; optind++ )
    {
      file = argv[optind];
      printf ( "Alterando: %s\n", file );
      // abre o arquivo para leitura
      FILE *file_r = fopen ( file, "rb" );
      if ( !file_r )
        {
          fprintf ( stderr, "%s: %s\n", file, strerror ( errno ) );
          if ( read_stdin )
            free ( new_value );

          continue;
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
          printf ( "%s: %s\n", file, strerror ( errno ) );
          if ( read_stdin )
            free ( new_value );

          free ( buffer );
          fclose ( file_r );
          continue;
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
                    "%s possui apenas %d linha(s)\n",
                    file,
                    mod_linha - skip_line - 1 );
          if ( read_stdin )
            free ( new_value );

          free ( buffer );
          fclose ( file_r );
          continue;
        }

      // pega o tamanho da linha atual a ser modificada
      size_t len_cur_line = 0;
      while ( buffer[pos_line + len_cur_line++] != '\n' )
        ;

      // novo tamanho que o arquivo editado terá
      size_t write_len = 0;

      // adicionar conteudo, desloca linha atual para baixo
      if ( !swap_content_line )
        {
          // se não fizer o cast de pos_line para int, o i é tratado como
          // unsigned na comaração e assim numca sera menor que 0 (caso seja a
          // primeira linha), e o for numca termina.
          for ( int i = len_file_r; i >= ( int ) pos_line; i-- )
            buffer[i + new_value_l] = buffer[i];

          for ( size_t i = 0; i < new_value_l; i++ )
            buffer[i + pos_line] = new_value[i];

          write_len = len_file_r + new_value_l;
        }
      // remove linha atual, novo conteudo tem mesmo tamanho
      else if ( new_value_l == len_cur_line )
        {
          // copia os novos bytes por cima
          for ( size_t i = 0; i < new_value_l; i++ )
            buffer[i + pos_line] = new_value[i];

          write_len = len_file_r;
        }
      // remove linha atual, novo conteudo é maior que conteudo atual
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
      // remove linha atual, novo conteudo é menor que conteudo atual
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
          printf ( "%s: %s\n", file, strerror ( errno ) );
          if ( read_stdin )
            free ( new_value );

          free ( buffer );
          continue;
        }

      if ( ( fwrite ( buffer, 1, write_len, file_w ) ) < write_len )
        {
          printf ( "%s: %s\n", file, strerror ( errno ) );
          if ( read_stdin )
            free ( new_value );

          free ( buffer );
          fclose ( file_w );
          continue;
        }

      if ( read_stdin )
        free ( new_value );

      free ( buffer );
      fclose ( file_w );
    }

  return EXIT_SUCCESS;
}
