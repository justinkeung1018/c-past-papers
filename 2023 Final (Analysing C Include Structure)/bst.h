// bst.[ch] module:
//	Provides a Binary Search Tree mapping keys: strings, to
//	values: generic (void *) pointers.
//	Let's assume that elements are never removed.

// print kv callback: invoked with a FILE *, a key and the value.
typedef void (*bst_printkv_func)( FILE *, char *, void * );

// every (k,v) callback: invoked with a key, the value and a state pointer.
typedef void (*bst_kv_func)( char *, void *, void * );

// free value callback: invoked with the value to be freed.
typedef void (*bst_freev_func)( void * );

struct bst;			// incomplete declaration
typedef struct bst *bst;	// our ADT..

// prototypes go here.. you'll need to maintain them as usual
extern bst make_empty_bst( bst_printkv_func pf, bst_freev_func ff );
extern void add_bst_long( bst b, char * key, void * value );
extern void add_bst( bst b, char * key, void * value );
extern bool in_bst( bst b, char * key );
extern void * get_bst( bst b, char * key );
extern void foreach_bst( bst b, bst_kv_func cb, void * state );
extern void print_bst( bst b, FILE * out );
extern void free_bst( bst b );
