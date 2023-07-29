#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "bst.h"
#include "testutils.h"


// (key,value) callback: invoked with a key, a value and some generic state.
static void mykvf( char *k, void *v, void *state ) {
	printf( "debug: kvf: key=%s, value=%s\n", k, (char *)v );
}

static void myprintfunc( FILE *out, char *key, void *value )
{
	fprintf( out, "%s=>%s", key, (char *)value );
}


int main( void )
{
	bst b = make_empty_bst( &myprintfunc, NULL );

	testint( in_bst( b, "hello" ), false, "hello in?" );
	testint( in_bst( b, "there" ), false, "there in?" );
	testint( in_bst( b, "hi" ), false, "hi in?" );

	add_bst( b, "hello", "bonjour" );
	testint( in_bst( b, "hello" ), true, "hello in?#2" );
	testint( in_bst( b, "there" ), false, "there in?#2" );
	testint( in_bst( b, "hi" ), false, "hi in?#2" );

	char *value = get_bst( b, "hello" );
	teststring( value, "bonjour", "get(hello)" );

	add_bst( b, "there", "alors" );
	testint( in_bst( b, "hello" ), true, "hello in?#3" );
	testint( in_bst( b, "there" ), true, "there in?#3" );
	testint( in_bst( b, "hi" ), false, "hi in?#3" );

	value = get_bst( b, "there" );
	teststring( value, "alors", "get(there)" );

	add_bst( b, "goodbye", "ciao" );
	testint( in_bst( b, "hello" ), true, "hello in?#4" );
	testint( in_bst( b, "there" ), true, "there in?#4" );
	testint( in_bst( b, "hi" ), false, "hi in?#4" );
	testint( in_bst( b, "goodbye" ), true, "goodbye in?#4" );

	value = get_bst( b, "goodbye" );
	teststring( value, "ciao", "get(goodbye)" );

	printf( "debug: b: " );
	print_bst( b, stdout );
	putchar( '\n' );

	printf( "debug: foreach gives\n" );
	foreach_bst( b, &mykvf, NULL );

	free_bst( b );
	return 0;
}
