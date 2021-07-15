
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memset
#include <time.h>
#include <unistd.h>

#include "hashtable.h"

#define TOT_DATA 20

typedef struct mydata
{
  int key;
  int value;
} Mydata;

struct my_array
{
  Mydata *data;
  size_t pos;
};

int
print( void *value, void *user_data )
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

int to_array( void *value, void *user_data )
{
  struct my_array *ar = user_data;
  ar->data[ar->pos] = *(Mydata *)value;
  ar->pos++;

  return 0;
}

Mydata *
copy_ht_to_array ( hashtable_t *ht )
{
  size_t size = ht->nentries + 1;
  Mydata *p = malloc( size * sizeof( *p ) );

  if (p)
    {
      struct my_array my_array = { .data = p, .pos = 0 };

      hashtable_foreach( ht, to_array, &my_array );

      memset(p + ht->nentries, 0, sizeof( *p ) );
    }

  return p;
}

int
main( void )
{

  Mydata *data;
  hashtable_t *ht = hashtable_new( free );





  int keys[TOT_DATA];
  srand(time(NULL));
  for (size_t i = 0; i < TOT_DATA; i++)
    keys[i] = rand() % 50000;

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

  for (size_t i = 0; i < TOT_DATA / 1.2; i++)
    {
      data = hashtable_remove( ht, keys[i] );
      if ( data )
       {
         // DEBUG("REMOVED\n");
         free(data);
       }
    }
  print_ht( ht );
  //
  // data = copy_ht_to_array( ht );
  // Mydata *p = data;
  // data = *pp;
  hashtable_destroy( ht );

  // while(p->key)
  //   {
  //     printf("data->key %d\ndata->value %d\n\n",
  //     p->key, p->value);
  //
  //     p++;
  //   }
  //
  // free(data);

  return 0;
}
