
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memset
#include <time.h>
#include <unistd.h>

#include "hashtable.h"

#define TOT_DATA 2

typedef struct mydata
{
  int key;
  int value;
} Mydata;

struct my_array
{
  Mydata **data;
  size_t pos;
};

int
print( hashtable_t *ht, void *value, void *user_data )
{
  Mydata *data = (Mydata *)value;
  int *count = (int *)user_data;

  printf("count - %d\nmydata->key %d\nmydata->value %d\n\n",
          ++(*count), data->key, data->value);
  return 0;
}

void print_ht ( hashtable_t *ht )
{
  int count = 0;
  hashtable_foreach( ht, print, &count );
}

int to_array( hashtable_t *ht, void *value, void *user_data )
{
  struct my_array *ar = user_data;
  ar->data[ar->pos] = value;
  ar->pos++;

  return 0;
}

Mydata **
copy_ht_to_array ( hashtable_t *ht )
{
  Mydata **pp = malloc( ( ht->nentries + 1 ) * sizeof( *pp ) );

  if ( pp )
    {
      struct my_array my_array = { .data = pp, .pos = 0 };

      hashtable_foreach( ht, to_array, &my_array );

      pp[ht->nentries] = NULL;  // last pointer
    }

  return pp;
}

int remove_inative( hashtable_t * ht, void *value, void *user_data )
{
  Mydata *data = value;

  fprintf(stderr, "value %d\n", data->value);
  if ( 0 == data->value )
    free ( hashtable_remove( ht, data->key ) );

  return 0;
}

int
main( void )
{
  hashtable_t *ht = hashtable_new( free );

  int keys[TOT_DATA];
  srand(time(NULL));
  for (size_t i = 0; i < TOT_DATA; i++)
    keys[i] = rand() % 50000;

  Mydata *data;
  for (size_t i = 0; i < TOT_DATA; i++)
    {
      data = malloc( sizeof *data );
      data->key = keys[i];
      data->value = i;

      hashtable_set( ht, data->key, data );
    }

  // for (size_t i = 0; i < TOT_DATA; i++)
  //   {
  //     data = hashtable_get( ht, keys[i] );
  //     if ( data )
  //       {
  //         printf("data->key - %d\ndata->value - %d\n\n",
  //                 data->key, data->value);
  //       }
  //   }

  // for (size_t i = 0; i < TOT_DATA / 1.2; i++)
  //   {
  //     data = hashtable_remove( ht, keys[i] );
  //     if ( data )
  //      {
  //        // DEBUG("REMOVED\n");
  //        free(data);
  //      }
  //   }

  // Mydata **pp_data = copy_ht_to_array( ht );
  //
  // Mydata **pp = pp_data;
  // while( *pp )
  //   {
  //     Mydata *p = *pp;
  //     // printf("data->key %d\ndata->value %d\n\n",
  //     // p->key, p->value);
  //
  //     // p->value += 100;
  //
  //     pp++;
  //   }
  //
  // free( pp_data );

  print_ht( ht );

  hashtable_foreach( ht, remove_inative, NULL );

  print_ht( ht );

  hashtable_destroy( ht );


  return 0;
}
