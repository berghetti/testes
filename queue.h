#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stdint.h>

#include "compiler.h"
#include "afp_internal.h"

#define QUEUE_SIZE 128

/*
 * Ring queue FIFO.
 * Usable size is QUEUE_SIZE - 1.
 */

#define IS_POWER_OF_TWO( x ) ( ( x ) && ( x & ( ( x ) -1 ) ) == 0 )

static_assert ( IS_POWER_OF_TWO ( QUEUE_SIZE ), "Queue is not power of two" );

#define MASK ( QUEUE_SIZE - 1 )

struct queue
{
  uint32_t tail;
  uint32_t head;
  void *data[QUEUE_SIZE];
} __aligned ( CACHE_LINE_SIZE );

static inline struct queue *
queue_alloc ( void )
{
  return rte_malloc ( "queue", sizeof ( struct queue ), CACHE_LINE_SIZE );
};

static inline void
queue_free ( struct queue *q )
{
  rte_free ( q );
}

static inline void
queue_init ( struct queue *q )
{
  q->tail = q->head = 0;
}

static inline int
queue_is_empty ( struct queue *q )
{
  return q->tail == q->head;
}

static inline int
queue_is_full ( struct queue *q )
{
  return ( ( q->tail + 1 ) & MASK ) == q->head;
}

// count elements used
static inline unsigned
queue_count ( struct queue *q )
{
  return ( QUEUE_SIZE + q->tail - q->head ) & MASK;
}

static inline unsigned
queue_count_free ( struct queue *q )
{
  return QUEUE_SIZE - 1 - queue_count ( q );
}

static inline void *
queue_enqueue ( struct queue *restrict q, void *data )
{
  assert ( !queue_is_full ( q ) );

  q->data[q->tail++ & MASK] = data;
  return data;
}

static inline uint32_t
queue_enqueue_bulk ( struct queue *restrict q, void **buff, uint32_t size )
{
  assert ( queue_count_free ( q ) >= size );

  for ( uint32_t i = 0; i < size; i++ )
    q->data[q->tail++ & MASK] = buff[i];

  return size;
}

static inline void *
queue_dequeue ( struct queue *q )
{
  assert ( !queue_is_empty ( q ) );

  return q->data[q->head++ & MASK];
}

/* TODO: unrolling this loops */
static inline void
_merge ( struct queue *restrict dst, struct queue *restrict src, uint32_t size )
{
  for ( uint32_t i = 0; i < size; i++ )
    {
      /* get interleaved objs from src queue to dst queue */
      dst->data[dst->tail++ & MASK] = src->data[( src->head + i * 2 ) & MASK];

      /* fix holes in src queue */
      src->data[( src->head + i ) & MASK] =
              src->data[( src->head + ( i * 2 + 1 ) ) & MASK];
    }

  /* update src queue tail */
  src->tail -= size;
}

static inline void
_head ( struct queue *restrict dst, struct queue *restrict src, uint32_t size )
{
  while ( size-- )
    dst->data[dst->tail++ & MASK] = src->data[src->head++ & MASK];
}

static inline void
_tail ( struct queue *restrict dst, struct queue *restrict src, uint32_t size )
{
  while ( size-- )
    dst->data[dst->tail++ & MASK] = src->data[--src->tail & MASK];
}

static inline void
queue_stealing ( struct queue *restrict dst,
                 struct queue *restrict src,
                 uint32_t size )
{
  _head ( dst, src, size );
  //_tail ( dst, src, size );
  //_merge ( dst, src, size );
}

#endif
