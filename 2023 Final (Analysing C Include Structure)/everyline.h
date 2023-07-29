// everyline.[ch] module:
//	Provided I/O support functions, most notably one to read every line
//	of a file and invoke a callback with that line.


#define MAXLINELEN 256
typedef char line[MAXLINELEN];		// a line of text

// every line callback: invoked with a filename, a line number and the line
typedef void (*everylinecb)( char *, int, char * );

// prototypes go here.. you'll need to maintain them as usual
extern int foreveryline( char * filename, everylinecb eachlinef );
