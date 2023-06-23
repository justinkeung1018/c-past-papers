// everyline.[ch] module:
//	Provided I/O support functions, most notably one to read every line
//	of a file and invoke a callback with that line.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "everyline.h"

// int n = foreveryline( filename, eachlinef );
//	open the given filename, read every line from that a file,
//	remove the trailing newline (if present) and invoke the given
//	callback eachlinef with the filename, the line number and the line.
//	Return the number of lines read, -1 if the file can't be opened.
//
int foreveryline( char *filename, everylinecb eachlinef )
{
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    // File failed to open
    return -1;
  }

  int num_lines = 0;
  line buffer;
  while (fgets(buffer, MAXLINELEN, fp)) {
    num_lines++;
    if (buffer[strlen(buffer) - 1] == '\n') {
      buffer[strlen(buffer) - 1] = '\0';
    }
    eachlinef(filename, num_lines, buffer);
  }

  fclose(fp);

  return num_lines;
}

