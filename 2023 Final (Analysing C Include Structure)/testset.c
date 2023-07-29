#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

//#include "bst.h"
#include "set.h"
#include "testutils.h"


static char strcat_buf[256];

static void strcat_callback( char *s, void *state )
{
	strcat( strcat_buf, s );
}


int main( void )
{
	set s = make_set();

	testint( in_set( s, "hello" ), false, "hello in?" );
	testint( in_set( s, "there" ), false, "there in?" );
	testint( in_set( s, "hi" ), false, "hi in?" );

	add_set( s, "hello" );
	testint( in_set( s, "hello" ), true, "hello in?#2" );
	testint( in_set( s, "there" ), false, "there in?#2" );
	testint( in_set( s, "hi" ), false, "hi in?#2"    );
	printf( "debug: s: " );
	print_set( s, stdout );
	putchar( '\n' );

	add_set( s, "there" );
	testint( in_set( s, "hello" ), true, "hello in?#3" );
	testint( in_set( s, "there" ), true, "there in?#3" );
	testint( in_set( s, "hi" ), false, "hi in?#3"    );
	printf( "debug: s: " );
	print_set( s, stdout );
	putchar( '\n' );

	add_set( s, "goodbye" );
	testint( in_set( s, "hello" ), true, "hello in?#4" );
	testint( in_set( s, "there" ), true, "there in?#4" );
	testint( in_set( s, "hi" ), false, "hi in?#4"    );
	testint( in_set( s, "goodbye" ), true, "goodbye in?#4" );

	printf( "debug: s: " );
	print_set( s, stdout );
	putchar( '\n' );

	foreach_set( s, &strcat_callback, NULL );
	printf( "debug: strcat_buf = %s\n", strcat_buf );

	teststring( strcat_buf, "goodbyehellothere", "strcat_buf" );

	add_set( s, "fluffy" );
	add_set( s, "bunny" );
	add_set( s, "wibbly" );
	add_set( s, "wobbly" );
	add_set( s, "timey" );
	add_set( s, "wimey" );
	add_set( s, "limey" );
	add_set( s, "limes" );
	add_set( s, "times" );
	add_set( s, "tomes" );
	add_set( s, "topes" );
	add_set( s, "hopes" );
	add_set( s, "lopes" );
	add_set( s, "lopps" );

	printf( "debug: s: " );
	print_set( s, stdout );
	putchar( '\n' );

	free_set( s );
	return 0;
}
