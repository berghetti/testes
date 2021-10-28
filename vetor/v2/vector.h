
#ifndef VECTOR_H
#define VECTOR_H

#include <sys/types.h>

void *
vector_new( size_t nmeb, size_t size );

int
vector_push( void *v, void *data );

void
vector_free( void *v );

#endif // VECTOR_H
