// set.h:	a simple set of strings, to which items are only ever added
//

struct set;		// incomplete type
typedef struct set *set;


// every member callback: invoked with a member and a state pointer
typedef void (*set_member_func)( char *, void * );


// prototypes

extern set make_set( void );
extern void add_set( set s, char * item );
extern bool in_set( set s, char * value );
extern void foreach_set( set s, set_member_func smf, void * stateptr );
extern void print_set( set s, FILE * out );
extern void free_set( set s );
