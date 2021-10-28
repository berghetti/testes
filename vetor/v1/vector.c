
#include <stdlib.h>
#include <string.h>

#include "vector.h"

#define SIZE_CHUNK 16

// struct vector
// {
//   void *buffer;
//   size_t elements_allocated;
//   size_t elements_used;
//   size_t element_size;
// };

static void *
vector_alloc( vector *v, size_t size )
{
  return realloc( v->buffer, size * v->element_size );
}

static void
vector_copy( vector *v, void *data )
{
  memcpy( v->buffer + ( v->elements_used * v->element_size ),
          data,
          v->element_size );
}

vector *
vector_new( size_t element_size )
{
  vector *v = malloc( sizeof *v );

  if (v)
    {
      v->element_size = element_size;
      v->elements_used = 0;
      v->elements_allocated = 0;
      v->buffer = NULL;
    }

  return v;
}

int
vector_push( vector *v, void *data )
{
  if ( v->elements_used == v->elements_allocated )
    {
      v->elements_allocated += SIZE_CHUNK;

      void *tmp = vector_alloc(v, v->elements_allocated );

      if ( !tmp )
        return 0;

      v->buffer = tmp;
    }

  vector_copy( v, data );

  v->elements_used++;

  return 1;
}

size_t
vector_size( vector *v )
{
  return v->elements_used;
}

void *
vector_buffer( vector *v )
{
  return v->buffer;
}
