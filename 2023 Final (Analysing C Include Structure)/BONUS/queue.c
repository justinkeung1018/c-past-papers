// queue.[ch] module:
//	Provides a simple queue of strings (which are duplicated).
//	Elements are enqued to the end, and dequeued from the front.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "queue.h"


struct queue
{
	int cap;		// the current allocated capacity of el
	int nel;		// the current number of elements (nel<=cap)
	char **el;		// the malloc()ed block of cap elements
};



// queue q = make_queue( firstcap );
//	Create a new queue, with an initial capacity of firstcap.
//	Abort if any memory allocation fails.
//
queue make_queue( int firstcap )
{
	queue q = malloc( sizeof(struct queue) );
	assert( q != NULL );
	q->cap = firstcap;
	q->nel = 0;
	q->el  = malloc( firstcap * sizeof(char *) );
	assert( q->el != NULL );
	return q;
}


// bool isempty = isempty_queue( q );
//	Return true iff q is empty.
int isempty_queue( queue q )
{
	return q->nel == 0;
}


// int nel = length_queue( q );
//	Return the length of (number of elements currently in) the q.
int length_queue( queue q )
{
	return q->nel;
}


// en_queue( q, element );
//	Enqueue the given element to the back of q, expanding q's chunk
//	if necessary.  The element is stdup()d.
//	Special case: don't let an element appear twice.
//	Abort if any memory allocation fails.
void en_queue( queue q, char *element )
{
	if( q->cap == q->nel )
	{
		q->cap += 20;
		q->el = realloc( q->el, q->cap * sizeof(char *) );
		assert( q->el != NULL );
	}
	for( int i=0; i<q->nel; i++ )
	{
		if( strcmp( q->el[i], element ) == 0 ) return;
	}
	char *p = strdup(element);
	assert( p != NULL );
	q->el[ q->nel++ ] = p;
}


// char *element = de_queue( q );
//	Dequeue the first element from q (and shuffle up the rest)
//	The caller is responsible for free()ing this element.
//	Abort if q was empty when called.
char *de_queue( queue q )
{
	assert( q->nel > 0 );
	char *result = q->el[0];
	for( int i=1; i<q->nel; i++ )
	{
		q->el[i-1] = q->el[i];
	}
	q->nel--;
	q->el[q->nel] = NULL;
	return result;
}


// foreach_queue( q, &elementfunc );
//	Iterate over every element of q, invoking the
//	given per-element callback function for each one
//	(with the queue element as parameter)
void foreach_queue( queue q, queue_el_func cb )
{
	for( int i=0; i<q->nel; i++ )
	{
		(*cb)( q->el[i] );
	}
}


// print_queue( q, out );
//	Printout the queue q to out, in a standard array like
//	format (CSV), invoking the built in print function for each element
void print_queue( queue q, FILE *out )
{
	fprintf( out, "[ " );
	for( int i=0; i<q->nel; i++ )
	{
		if( i>0 ) fputc( ',', out );
		fprintf( out, "%s", q->el[i] );
	}
	fprintf( out, " ]" );
}


// free_queue( q );
//	Free the given queue q.
void free_queue( queue q )
{
	for( int i=0; i<q->nel; i++ )
	{
		free( q->el[i] );
	}
	free( q->el );
	free( q );
}
