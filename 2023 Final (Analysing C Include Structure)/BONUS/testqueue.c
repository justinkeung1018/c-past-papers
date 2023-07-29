#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "queue.h"
#include "testutils.h"


typedef struct
{
	char *add;
	int   result;
} pair;

pair ot_tests[] =
{
	// value to add,                result (3 * prevvalue + len(value))
	{ "one",                        3, },
	{ "two",                        12, },
	{ "three",                      41, },
	{ NULL, -1 },
};


static int formula = 0;
static void formula_callback( char *element )
{
	formula *= 3;
	formula += strlen( element );
}

int compute_formula( queue q )
{
	formula = 0;
	foreach_queue( q, &formula_callback );
	return formula;
}


static char strcat_buf[256];
static void strcat_callback( char *s )
{
	strcat( strcat_buf, s );
}


int main( void )
{
	queue q = make_queue( 10 );
	for( int i=0; ot_tests[i].add != NULL; i++ )
	{
		en_queue( q, ot_tests[i].add );
		int formula = compute_formula( q );
		int expected = ot_tests[i].result;
		testint( formula, expected, "formula" );
	}
	foreach_queue( q, &strcat_callback );
	printf( "debug: strcat_buf = %s\n", strcat_buf );

	teststring( strcat_buf, "onetwothree", "strcat" );

	printf( "debug: q: " ); print_queue( q, stdout ); putchar( '\n' );

	// new dequeue tests
	for( int i=0; ot_tests[i].add != NULL; i++ )
	{
		assert( ! isempty_queue(q) );
		printf( "debug: q: " ); print_queue( q, stdout ); putchar( '\n' );
		char *got = de_queue( q );
		char *expected = ot_tests[i].add;
		teststring( got, expected, "dequeue" );
	}

	free_queue( q );
	return 0;
}
