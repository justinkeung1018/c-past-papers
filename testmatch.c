#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "match.h"
#include "testutils.h"


char *ot_wds[] = { "one", "two", NULL };

typedef struct
{
	char *target;
	char *result;
} pair;

pair ot_tests[] =
{
	// target string,                result of match
	{"one two bu",                   "bu"},
	{"    one two   bu",             "bu"},
	{"one tw",                       NULL},
	{"one twe",                      NULL},
	{"one tweee",                    NULL},
	{"one twothree",                 "three"},
	{"one two",                      ""},
	{"onetwo",                       ""},
	{"      onetwo",                 ""},
	{"      one two",                ""},
	{"      one two ",               ""},
	{"      one two  ",              ""},
	{"      one two  x",             "x"},
	{" one  two buckle    my  shoe", "buckle    my  shoe"},
	{NULL,NULL},
};


int main( void )
{
	for( int i=0; ot_tests[i].target != NULL; i++ )
	{
		char *result = matchwords( ot_tests[i].target, ot_wds );
		char *expected = ot_tests[i].result;
		if( expected != NULL )
		{
			teststring( result, expected, "match" );
		} else
		{
			if( result != NULL )
			{
				printf( "T match found (%s) expected "
					"no match: FAIL\n", result );
			} else
			{
				printf( "T no match as expected: OK\n" );
			}
		}
	}
return 0;
}
