// example usage list

#include <stdio.h>
#include "list.h"

struct list_node *
search ( struct list_node *node, int value )
{
  while( node )
    {
      if ( value == *(int *)node->data )
        return node;

      node = node->next;
    }

  return NULL;
}

void
print( struct list_node *node )
{
  while( node )
    {
      int *v = node->data;
      printf( "%d ", *v );
      node = node->next;
    }

  putchar('\n');
}

int
main( void )
{
  struct list list = { 0 };
  struct list_node *pn;

  int values[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

  // insert values
  for (int i = 0; i < 10; i++)
    list_push( &list, &values[i] );

  print( list.head );

  // example search / modify
  pn = search( list.head, 1);
  if ( pn )
    {
      int *v = pn->data;
      *v = 20;
    }
  print( list.head );

  // example search / delete
  pn = search( list.head, 7);
  if ( pn )
    {
      list_delete( &list, pn );
    }
  print( list.head );

  list_delete( &list, list.tail );
  print( list.head );


  return 0;
}
