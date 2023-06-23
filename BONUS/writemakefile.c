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
#include "queue.h"

#define MAX_QUEUE_CAP 1000
#define RESULT_SIZE   10000

// let's try to write a Makefile..  on stdout for now, but make that
// easy to change via this variable..

static FILE *out;


typedef struct
{
  bool wantspace;
  bool basename;
} pf_data;

typedef struct {
  analysis a;
  queue todo;
  set done;
} object_data;

// print out the filename (whole or basename, i.e. all but the last
// two characters ".c").  The state pointer is really a pointer to a
// pf_data, we add a leading space if wantspace is true.
// (set wantspace to true for next time).
//
static void printfilename( char *filename, void *state )
{
  pf_data *d = (pf_data *)state;
  if( d->wantspace ) putc( ' ', out );
  d->wantspace = true;
  int len = strlen(filename);
  fprintf( out, "%.*s", d->basename?len-2:len, filename );
}


static void printrule( char *target, void *disguisedset, void *state )
{
  set s = (set)disguisedset;
  int len = strlen(target);
  fprintf( out, "%.*s.o:\t", len-2, target );
  pf_data d = {false, false};
  foreach_set( s, &printfilename, &d );
  fprintf( out, "\n\n" );
}

// Caller must free pointer returned
static char *replace_extension(char *filename, char extension) {
  char *new_filename = strdup(filename);
  assert(new_filename != NULL);
  int len = strlen(new_filename);
  new_filename[len - 1] = extension;
  return new_filename;
}

static void add_src_file(char *h_filename, void *state) {
  object_data *data = (object_data *)state;
  char *src_filename = replace_extension(h_filename, 'c');
  if (in_set(data->a->existset, src_filename) && !in_set(data->done, src_filename)) {
    en_queue(data->todo, src_filename);
  }
}

static void print_objects(char *src_filename, void *state) {
  char *result = (char *)state;

  // Add 1 to account for the space
  assert(strlen(result) + strlen(src_filename) + 1 < RESULT_SIZE);

  char *obj_filename = replace_extension(src_filename, 'o');
  (void)strcat(result, obj_filename);
  (void)strcat(result, " ");
}

// char *objects = allobjects( mainsrcfilename, a );
//      Given <mainsrcfilename>, the filename of a C source file
//      containing main(), and an initialized analysis pointer <a>,
//      generate and return a malloc()d string containing
//      a space separated list of all object filenames (.o files)
//      that should be linked together to make the executable
//      of mainsrcfilename.
//      
char *allobjects( char *mainsrcfilename, analysis a )
{
  // TASK 5 (BONUS): IMPLEMENT THIS PROPERLY.
  queue todo = make_queue(MAX_QUEUE_CAP);
  en_queue(todo, mainsrcfilename);
  set done = make_set();

  object_data state = (object_data){ a, todo, done };

  while (!isempty_queue(todo)) {
    char *src_filename = de_queue(todo);
    add_set(done, src_filename);
    set includes = get_bst(a->c2inc, src_filename);
    foreach_set(includes, add_src_file, &state);   
  }

  char result[RESULT_SIZE] = "";
  foreach_set(done, print_objects, &result);
  // Remove trailing space
  result[strlen(result) - 1] = '\0';

  char *result_dup = strdup(result);
  assert(result_dup != NULL);

  free_queue(todo);
  free_set(done);

  return result_dup;
}


// print out the link target for filename;
// The state pointer is really a pointer to an analysis struct.
//
static void printlink( char *filename, void *state )
{
  analysis ap = (analysis)state;
  char *objects = allobjects( filename, ap );

  int len = strlen(filename);
  fprintf( out, "%.*s:\t%s\n\n", len-2, filename, objects );
  free( objects );
}


int main( void )
{
  analysis a = analyse();

  out = stdout;                   // make it easy to change

  fprintf( out, "# Makefile automatically generated by... us:-)\n\n" );
  fprintf( out, "CFLAGS = -g -Wall -I..\n" );
  fprintf( out, "CC = gcc\n" );

  // build all main programs, basenames with no initial space:
  fprintf( out, "BUILD = " );
  if( a->mainset != NULL )
  {
    pf_data d;
    d.wantspace = false;
    d.basename = true;
    foreach_set( a->mainset, &printfilename, &d );
  }
  fputc( '\n', out );

  fprintf( out, "\nall:\t$(BUILD)\n\n" );

  fprintf( out, "clean:\t\n\t/bin/rm -f $(BUILD) *.o core .nfs*\n\n" );

  // now print out, for each c2inc, a dependency rule on all includes
  if( a->c2inc != NULL )
  {
    foreach_bst( a->c2inc, &printrule, NULL );
  }

  // finally, print out, for each main program, a link rule
  if( a->mainset != NULL )
  {
    foreach_set( a->mainset, &printlink, a );
  }

  free_analysis( a );

  return 0;
}
