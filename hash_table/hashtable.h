
#ifndef HASHTABLE
#define HASHTABLE

#include <stdint.h>

typedef size_t hash_t;

typedef struct slist_item
{
  struct slist_item *next;
} slist_item_t;

typedef struct slist
{
  slist_item_t *head;
} slist_t;

typedef struct hashtable
{
  size_t nentries;  // Total number of entries in the table
  size_t nbuckets;
  slist_t *buckets;
} hashtable_t;

typedef struct hashtable_entry
{
  // used by hashtable_t.buckets to link entries
  slist_item_t slist_item;

  hash_t key_hash;
  uint64_t key;
  void *value;
} hashtable_entry_t;

hashtable_t *
hashtable_new ( void );

void *
hashtable_set ( hashtable_t *ht, const uint64_t key, void *value );

void *
hashtable_get ( hashtable_t *ht, const uint64_t key );

void
hashtable_free( hashtable_t *ht, void (*clear)(void *) );

#endif // HASHTABLE
