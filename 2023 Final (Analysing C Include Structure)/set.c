// set.c:	a simple set of strings, to which items are only ever added

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "set.h"


struct set
{
	int cap;	// current capacity, i.e. allocated size of data
	int n;		// current number of entries in data (n<=cap)
	char **data;	// actual data, growable.  pointers into buf
	int bufcap;	// the allocated size of buf
	int buflen;	// the current amount of data stored in buf
	char *buf;	// the buffer of underlying data
};


// A set is a growable dynarray of string pointers into a common
// string buffer (storage for which is also maintained inside the set).


// set s = make_set()
//	Create an empty set.
//
set make_set( void )
{
	set s = malloc( sizeof(struct set) );
	assert( s != NULL );
	s->cap  = 10;
	s->n    = 0;
	s->data = malloc( s->cap * sizeof(char *) );
	assert( s->data != NULL );
	s->bufcap = 10;
	s->buflen = 0;
	s->buf = malloc( s->bufcap * sizeof(char) );
	assert( s->buf != NULL );
	return s;
}


static int cmpstringp( const void *p1, const void *p2 )
{
	// The actual arguments to this function are "pointers to
	// pointers to char", but strcmp(3) arguments are "pointers
	// to char", hence the following:
	char **a = (char **)p1;
	char **b = (char **)p2;
	//printf( "debug: cmpstringp: a=%s, b=%s\n", *a, *b );
	return strcmp( *a, *b );
}


// void add_set( s, item );
//	Add a given item to set s, growing it if necessary
//
void add_set( set s, char *item )
{
	bool isin = in_set( s, item );
	if( isin ) return;		// already there?

	//printf( "debug: add set.  adding item %s to set\n", item );

	int len = strlen(item)+1;
	if( s->buflen + len > s->bufcap )
	{
		s->bufcap += 100 + len;
		char *old = s->buf;
		char *new = realloc( old, s->bufcap * sizeof(char) );
		assert( new != NULL );
		if( old != new )
		{
			s->buf = new;
			// the buffer has moved, so we'll need
			// to alter all the data pointers
			for( int i=0; i<s->n; i++ )
			{
				s->data[i] = new+(s->data[i]-old);
			}
		}
	}
	char *sp = s->buf + s->buflen;
	strcpy( sp, item );
	s->buflen += len;

	if( s->n >= s->cap )
	{
		s->cap += 100;
		assert( s->n <= s->cap );
		s->data = realloc( s->data, s->cap * sizeof(char *) );
		assert( s->data != NULL );
	}
	s->data[s->n++] = sp;

	#if 0
	printf( "debug: BEFORE qsort, set ptrs are: " );
	for( int i=0; i<s->n; i++ )
	{
		printf( "%s,", s->data[i] );
	}
	putchar( '\n' );
	#endif

	// re-sort data (could use insertion sort if we cared enough)
	qsort( s->data, s->n, sizeof(char *), &cmpstringp );

	#if 0
	printf( "debug: AFTER qsort, set ptrs are: " );
	for( int i=0; i<s->n; i++ )
	{
		printf( "%s,", s->data[i] );
	}
	putchar( '\n' );

	printf( "debug: after appending %s, set is ", item );
	print_set( s, stdout ); putchar( '\n' );
	char *beyond = s->buf + s->buflen;
	printf( "debug: set buffer is: " );
	for( char *p = s->buf; p<beyond; p++ )
	{
		putchar( *p=='\0' ? 'X' : *p );
	}
	putchar( '\n' );
	printf( "debug: set ptrs are: " );
	for( int i=0; i<s->n; i++ )
	{
		printf( "%s,", s->data[i] );
	}
	putchar( '\n' );
	#endif
}


// bool isin = in_set( s, value );
//	Given a set s, return true iff value is "in" s.
//	Let's use bsearch()...
//
bool in_set( set s, char *value )
{
	char **res = bsearch( &value, s->data, s->n, sizeof(char *),
			      &cmpstringp );
	return res != NULL;

	#if 0
	for( int i=0; i<s->n; i++ )
	{
		if( strcmp( value, s->data[i] ) == 0 ) return true;
	}
	return false;
	#endif
}


// foreach_set( s, smf, stateptr );
//   iterator function: call smf( element, stateptr )
//   foreach element in s.
//
void foreach_set( set s, set_member_func smf, void * stateptr )
{
	assert( smf != NULL );
	for( int i=0; i<s->n; i++ ) {
		(*smf)( s->data[i], stateptr );
	}
}


// print_set( s, out );
//	Print set s to open writable file out
//	as a CSV list.
//
void print_set( set s, FILE *out )
{
	for( int i=0; i<s->n; i++ )
	{
		if( i>0 ) putc( ',', out );
		printf( "%s", s->data[i] );
	}
}


// free_set( s );
//	Free set s
//
void free_set( set s )
{
	free( s->data );
	free( s->buf );
	free( s );
}
