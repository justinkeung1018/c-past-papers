#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <glob.h>

#include "everyline.h"
#include "match.h"
#include "bst.h"
#include "set.h"
#include "analyse.h"


void print_one_c2inc( char *key, void *value, void *state )
{
	set s = value;
	printf( "c2inc{%s}: ", key );
	print_set( s, stdout );
	putchar( '\n' );
}


int main( void )
{
	analysis a = analyse();

	if( a->mainset != NULL )
	{
		printf( "mainset: " );
		print_set( a->mainset, stdout );
		putchar( '\n' );
	}
	if( a->existset != NULL )
	{
		printf( "existset: " );
		print_set( a->existset, stdout );
		putchar( '\n' );
	}
	if( a->c2inc != NULL )
	{
		foreach_bst( a->c2inc, &print_one_c2inc, NULL );
	}

	free_analysis( a );

	return 0;
}
