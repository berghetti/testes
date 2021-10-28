
#ifndef vector_H
#define vector_H

#include <sys/types.h>

struct vector
{
  void *buffer;
  size_t elements_allocated;
  size_t elements_used;
  size_t element_size;
};

typedef struct vector vector;

#define VECTOR_INIT( size )  \
  { .element_size = (size),  \
    .elements_allocated = 0, \
    .elements_used = 0,      \
    .buffer = NULL }

#define VECTOR_SIZE( v ) \
  ( (v).elements_used )

vector *
vector_new ( size_t element_size );

int
vector_push( vector *v, void *data );

size_t
vector_size( vector *v );

void *
vector_buffer( vector *v );

#endif // vector_H
