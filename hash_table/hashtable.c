#include <stdio.h> // provisorio
#include <stdlib.h>
#include "hashtable.h"

#define HASHTABLE_MIN_SIZE 16

#define TABLE_HEAD(ht, index) \
  ( ( hashtable_entry_t *) ( (slist_t *) &(ht)->buckets[(index)] )->head )

#define ENTRY_NEXT(entry) \
  ( (hashtable_entry_t *) ( (slist_item_t *) (entry))->next )

// https://github.com/shemminger/iproute2/blob/main/misc/ss.c
static hash_t
hash_func( uint64_t key )
{
  return (key >> 24) ^ (key >> 16) ^ (key >> 8) ^ key;
}

static void
hashtable_preprend( slist_t *list, slist_item_t *item )
{
  item->next = list->head;
  list->head = item;
}

static hashtable_entry_t *
hashtable_get_entry( hashtable_t *ht, const uint64_t key )
{
  hash_t hash = hash_func( key );
  size_t index = hash & ( ht->nbuckets - 1);

  hashtable_entry_t *entry = TABLE_HEAD(ht, index);
  while ( entry )
    {
      if ( entry->key == key )
        break;

      entry = ENTRY_NEXT(entry);
    }

  return entry;
}

hashtable_t *
hashtable_new( void )
{
    hashtable_t *ht = malloc ( sizeof *ht );

    if ( ! ht )
      return NULL;

    ht->nbuckets = HASHTABLE_MIN_SIZE;
    ht->nentries = 0;

    ht->buckets = calloc( ht->nbuckets, sizeof *(ht->buckets) );
    if ( !ht->buckets )
      {
        free( ht );
        return NULL;
      }

    return ht;
}

void *
hashtable_set ( hashtable_t *ht, const uint64_t key, void *value )
{
  hashtable_entry_t *entry = malloc( sizeof *entry );
  if ( !entry )
    return NULL;

  entry->key_hash = hash_func( key );
  entry->key = key;
  entry->value = value;

  ht->nentries++;
  // resize hashtable is needed here

  size_t index = entry->key_hash & ( ht->nbuckets - 1 );
  hashtable_preprend( &ht->buckets[index], (slist_item_t *) entry );

  return value;
}

void *
hashtable_get( hashtable_t *ht, const uint64_t key )
{
  hashtable_entry_t *entry = hashtable_get_entry( ht, key );
  if ( entry )
    return entry->value;

  return entry;
}

void
hashtable_free( hashtable_t *ht, void (*clear)(void *) )
{
  size_t tot_entries = ht->nentries;
  for ( size_t i = 0; i < ht->nbuckets && tot_entries; i++)
    {
      if ( !tot_entries )
        break;

      hashtable_entry_t *entry = TABLE_HEAD(ht, i);
      while( entry )
        {
          hashtable_entry_t *next = ENTRY_NEXT(entry);
          if (clear)
            clear( entry );

          free( entry );
          tot_entries--;
          entry = next;
        }
    }

  free( ht->buckets );
  free( ht );

}
