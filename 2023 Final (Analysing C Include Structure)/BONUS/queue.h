// queue.[ch] module:
//	Provides a simple queue of strings (which are duplicated).
//	Elements are enqued to the end, and dequeued from the front.

// every item callback: invoked with the item.
typedef void (*queue_el_func)( char * );

struct queue;			// incomplete type
typedef struct queue *queue;	// our ADT..

// prototypes go here.. you'll need to maintain them as usual
extern queue make_queue( int firstcap );
extern int isempty_queue( queue q );
extern int length_queue( queue q );
extern void en_queue( queue q, char * element );
extern char * de_queue( queue q );
extern void foreach_queue( queue q, queue_el_func cb );
extern void print_queue( queue q, FILE * out );
extern void free_queue( queue q );
