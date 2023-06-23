#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "everyline.h"


static void showline( char *filename, int lineno, char *theline )
{
	printf( "%s %4d: %s\n", filename, lineno, theline );
}


int main( void )
{
	int n = foreveryline( __FILE__, &showline );
	printf( "%d lines read\n", n );
	return 0;
}
