
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//strcat_ ( char *src, char *dst )
//{
//  size_t size = strlen( src ) + strlen ( dst ) + 1;
//  char *str = malloc( size );
//  char *ps = str;
//
//  while( *src )
//    *ps++ = *src++;
//
//  while( *dst )
//    *ps++ = *dst++;
//
//  return str;
//}

int main( int argc, char **argv )
{

    // char *str = strcat_( "ola", " mundo" );
  printf( "%s\n", strcat( argv[1], argv[2] ) );

  // printf( "%s\n", str );
  return 0;
}
