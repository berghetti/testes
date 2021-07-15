// based implementation python
// https://github.com/python/cpython/blob/main/Python/hashtable.c

#include <stdlib.h>
#include "hashtable.h"

#include "debug.h"

#define HASHTABLE_MIN_SIZE 16
#define HASHTABLE_HIGH 0.75
#define HASHTABLE_LOW 0.10
#define HASHTABLE_REHASH_FACTOR 2.0 / (HASHTABLE_LOW + HASHTABLE_HIGH)

#define TABLE_HEAD(ht, index) \
  ( ( hashtable_entry_t *) ((slist_t *)&(ht)->buckets[(index)])->head )

#define PP_TABLE_HEAD(ht, index)\
  ( ( hashtable_entry_t **) &((slist_t *)&(ht)->buckets[(index)])->head )

#define ENTRY_NEXT(entry) \
  ( (hashtable_entry_t *) ((slist_item_t *) (entry))->next )

#define PP_ENTRY_NEXT(entry) \
  ( (hashtable_entry_t **) &((slist_item_t *) (entry))->next )

/* makes sure the real size of the buckets array is a power of 2 */
static size_t
round_size( size_t s )
{
  if (s < HASHTABLE_MIN_SIZE)
    return HASHTABLE_MIN_SIZE;

  size_t i = 1;
  while ( i < s )
    i <<= 1;

  return i;
}

// https://github.com/shemminger/iproute2/blob/main/misc/ss.c
static hash_t
hash_func( size_t key )
{
  return (key >> 24) ^ (key >> 16) ^ (key >> 8) ^ key;
}

static inline void
hashtable_preprend( slist_t *list, slist_item_t *item )
{
  item->next = list->head;
  list->head = item;
}

static int
hashtable_rehash( hashtable_t *ht)
{
  size_t num_buckets = round_size((size_t)(ht->nentries * HASHTABLE_REHASH_FACTOR));
  if (num_buckets == ht->nbuckets)
    return 1;

  slist_t *new_buckets = calloc( num_buckets, sizeof( ht->buckets[0]) );
  if ( !new_buckets )
    return 0;

  for (size_t i = 0; i < ht->nbuckets; i++)
    {
      hashtable_entry_t *entry = TABLE_HEAD( ht, i );
      while ( entry )
        {
          hashtable_entry_t *next = ENTRY_NEXT( entry );

          size_t index = entry->key_hash & (num_buckets - 1);
          hashtable_preprend( &new_buckets[index], (slist_item_t *) entry );

          entry = next;
        }
    }

  free( ht->buckets );
  ht->nbuckets = num_buckets;
  ht->buckets = new_buckets;

  return 1;
}

static hashtable_entry_t *
hashtable_get_entry( hashtable_t *ht, const size_t key )
{
  hash_t hash = hash_func( key );
  size_t index = hash & ( ht->nbuckets - 1);

  hashtable_entry_t *entry = TABLE_HEAD( ht, index );
  while ( entry )
    {
      if ( (entry)->key == key )
        break;

      entry = ENTRY_NEXT( entry );
    }

  return entry;
}


hashtable_t *
hashtable_new( fclear clear )
{
    hashtable_t *ht = malloc ( sizeof *ht );

    if ( ! ht )
      return NULL;

    ht->nbuckets = HASHTABLE_MIN_SIZE;

    ht->buckets = calloc( ht->nbuckets, sizeof( ht->buckets[0] ) );
    if ( !ht->buckets )
      {
        free( ht );
        return NULL;
      }

    ht->nentries = 0;
    ht->clear = clear;

    return ht;
}

void *
hashtable_set ( hashtable_t * restrict ht, const size_t key, void *value )
{
  hashtable_entry_t *entry = malloc( sizeof *entry );
  if ( !entry )
    return NULL;

  entry->key_hash = hash_func( key );
  entry->key = key;
  entry->value = value;

  ht->nentries++;
  if ( (float)ht->nentries / (float)ht->nbuckets > HASHTABLE_HIGH )
    {
      if ( !hashtable_rehash( ht ) )
        {
          ht->nentries--;
          free( entry );
          return NULL;
        }
    }

  size_t index = entry->key_hash & ( ht->nbuckets - 1 );
  hashtable_preprend( &ht->buckets[index], (slist_item_t *) entry );

  DEBUG("seting new key %ld\n", entry->key);
  return value;
}

void *
hashtable_get( hashtable_t *ht, const size_t key )
{
  hashtable_entry_t *entry = hashtable_get_entry( ht, key );
  if ( entry )
    return entry->value;

  return entry;
}

int
hashtable_foreach( hashtable_t *ht,
                   hashtable_foreach_func func,
                   void *user_data )
{
  size_t j = 0;
  for ( size_t i = 0;
       i < ht->nbuckets && j < ht->nentries;
       i++ )
    {
      hashtable_entry_t *entry = TABLE_HEAD( ht, i );
      while( entry )
        {
          int ret = func( entry->value, user_data );
          if ( ret )
            return ret;

          j++;
          entry = ENTRY_NEXT( entry );
        }
    }

  return 0;
}

// https://github.com/mkirchner/linked-list-good-taste/
void *
hashtable_remove ( hashtable_t *ht, const size_t key )
{
  hash_t hash = hash_func( key );
  size_t index = hash & ( ht->nbuckets - 1);

  hashtable_entry_t **entry = PP_TABLE_HEAD( ht, index );
  while( *entry && (*entry)->key != key )
    entry = PP_ENTRY_NEXT( *entry );

  if ( !( *entry ) )
    return NULL;

  hashtable_entry_t *del = *entry;

  void *value = del->value;
  *entry = ENTRY_NEXT( del );
  free( del );
  ht->nentries--;

  if ( (float)ht->nentries / (float)ht->nbuckets < HASHTABLE_LOW )
    hashtable_rehash( ht );

  return value;
}

static inline void
hashtable_destroy_entry( hashtable_t *ht, hashtable_entry_t *entry )
{
  if ( ht->clear )
    ht->clear( entry->value );

  free( entry );
}

void
hashtable_destroy( hashtable_t *ht )
{
  for ( size_t i = 0; i < ht->nbuckets; i++ )
    {
      if ( !ht->nentries )
        break;

      DEBUG("%ld - |", i);

      hashtable_entry_t *entry = TABLE_HEAD( ht, i );
      int j = 0;  // debug
      while( entry )
        {
          DEBUG( " %d key %ld |", ++j, entry->key );
          hashtable_entry_t *entry_next = ENTRY_NEXT( entry );
          hashtable_destroy_entry( ht, entry );
          ht->nentries--;
          entry = entry_next;
        }
      DEBUG("\n");
    }

  free( ht->buckets );
  free( ht );
}