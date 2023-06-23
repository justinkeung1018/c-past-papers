#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "match.h"


// char *wds[] = { "one", "two", NULL };
// char *result = matchwords( target, wds );
//      Given a target string, and a NULL terminated array of words (each
//      word is a char *), attempt to match every word in wds against the
//      target, starting the matching process at the start of the target.
//      More specifically, you are to:
//      - match (and skip) any amount of whitespace (including none) at
//        the start of the target,
//      - then match wds[0] against the next part of the target exactly as is,
//      - then match (and skip) any amount of whitespace (including none),
//      - then match wds[1] etc.
//      If the target doesn't match all words, return NULL
//      If the target matches all words (not necessarily matching the
//      whole string), then match and skip any final amount of whitespace
//      (including none) after the last word is matched, and return a pointer
//      to the first unmatched char beyond that.
//
char * matchwords( char *target, char **wds )
{
	// TASK 2: IMPLEMENT THIS
	return NULL;
}
