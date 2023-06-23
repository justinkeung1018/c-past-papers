// analyse.[ch]: find all includes and main() in *.[ch] files and build
//		 some data structures to represent what we find

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <glob.h>

#include "bst.h"
#include "set.h"
#include "analyse.h"
#include "everyline.h"
#include "match.h"


static void recordmain( char *filename )
{
	printf( "debug: %s contains main()\n", filename );
	// TASK 4: build mainset.
}


static void recordfileexists( char *filename )
{
	printf( "debug: file %s exists\n", filename );
	// TASK 4: build existset.
}


static void recordinclude( char *filename, char *oneinc )
{
	printf( "debug: %s directly includes %s\n", filename, oneinc );
	// TASK 4: build c2inc.
}


static char *include[] = { "#include", NULL };
static char *hasmain[] = { "int", "main", "(", NULL };


static void examineline( char *filename, int ln, char *line )
{
	//printf( "debug: examine line %d, %s\n", ln, line );

	char *inc = matchwords( line, include );
	if( inc != NULL && *inc == '"' )
	{
		inc++;
		char *lastquote = strchr(inc,'"');
		if( lastquote != NULL ) *lastquote = '\0';
		recordinclude( filename, inc );
	}
	char *main = matchwords( line, hasmain );
	if( main != NULL )
	{
		recordmain( filename );
	}
}


static void print_wrapper( FILE *out, char *key, void *value )
{
	fprintf( out, "%s=>", key );
	print_set( value, out );	// value is itself a set
}


static void free_wrapper( void *value )
{
	free_set( (set)value );
}


static analysis make_analysis( void )
{
	analysis a = malloc( sizeof(struct analysis) );
	assert( a != NULL );
	a->existset = NULL;
	a->mainset = NULL;
	a->c2inc = NULL;
	return a;
}


analysis analyse( void )
{
	analysis a = make_analysis();

	glob_t globbuf;
	glob( "*.[ch]", 0, NULL, &globbuf );
	int nfiles = globbuf.gl_pathc;    // How many paths matched

	for( int fileno=0; fileno<nfiles; fileno++ )
	{
		char *filename = globbuf.gl_pathv[fileno];
		assert( filename != NULL );
		recordfileexists( filename );
		foreveryline( filename, &examineline );
	}
	globfree( &globbuf );
	return a;
}


void free_analysis( analysis a )
{
	if( a->mainset != NULL ) free_set( a->mainset );
	if( a->existset != NULL ) free_set( a->existset );
	if( a->c2inc != NULL ) free_bst( a->c2inc );
	free( a );
}
