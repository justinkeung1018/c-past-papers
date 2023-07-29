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
		printf( "c2inc: " );
		print_bst( a->c2inc, stdout );
		putchar( '\n' );
	}

	free_analysis( a );

	return 0;
}
