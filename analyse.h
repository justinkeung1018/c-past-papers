// analyse.[ch]: find all includes and main() in *.[ch] files and build
//		 some data structures to represent what we find

struct analysis
{
	set existset;		// set of .c and .h filenames that exist
	set mainset;		// set of .c filenames containing main()
	bst c2inc;		// .c filename -> set of included .h files
};

typedef struct analysis *analysis;


// prototypes go here.. you'll need to maintain them as usual
extern analysis analyse( void );
extern void free_analysis( analysis a );
