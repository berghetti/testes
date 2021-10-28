
#include <stdlib.h> // realloc, free
#include <string.h> // memcpy
#include <stddef.h> // offsetof

struct vector
{
  size_t elements_allocated;
  size_t elements_used;
  size_t element_size;
};

#define CHUNCK_TO_MEM( pos ) ( ( char *) (pos)  + sizeof( struct vector ) )
#define MEM_TO_CHUNK( pos ) \
  ( ( struct vector *) ( (char *) (pos) - sizeof( struct vector ) ) )

static void *
vector_alloc( struct vector *v, size_t size )
{
  return realloc( v, size + sizeof( struct vector ) );
}

static void
vector_copy( struct vector *restrict v, void *restrict data )
{
  char *ptr = CHUNCK_TO_MEM( v ) + ( v->elements_used * v->element_size );

  memcpy( ptr, data, v->element_size );
}

void *
vector_new( size_t nmemb, size_t size )
{
  struct vector *vt = vector_alloc( NULL,  nmemb * size );

  vt->elements_allocated = nmemb;
  vt->elements_used = 0;
  vt->element_size = size;

  return CHUNCK_TO_MEM( vt );
}

int
vector_push( void ** restrict v, void *restrict data )
{
  struct vector *vt = MEM_TO_CHUNK( *v );

  if ( vt->elements_used == vt->elements_allocated )
    {
      vt->elements_allocated <<= 1;
      size_t new_size = vt->elements_allocated * vt->element_size;
      void *temp = vector_alloc( vt, new_size );

      if ( !temp )
        return 0;

      if ( vt != temp )
        {
          vt = temp;
          *v = CHUNCK_TO_MEM( vt );
        }
    }

  vector_copy( vt, data );

  vt->elements_used++;

  return 1;
}

void
vector_free( void *v )
{
  struct vector *vt = MEM_TO_CHUNK( v );

  free( vt );
}
