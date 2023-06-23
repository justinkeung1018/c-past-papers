#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "bst.h"


//struct bintree;
typedef struct bintree *bintree; // pure binary bintree of key, value pairs
struct bintree
{
	char *key;		// the key: a string
	void *value;		// the value: a generic pointer
	bintree left, right;	// the left and right subtrees
};


struct bst
{
	bst_printkv_func pf;  // the (k,v) print function
	bst_freev_func ff;    // the value free function
	bintree t;	      // the binary bintree of (k,v) pairs itself
};


// bst d = make_empty_bst( pf, ff );
//	Create a new empty BST, with an element print function <pf>,
//	an element free function <ff>, and an empty bintree.
//	Abort if any memory allocation fails.
//
bst make_empty_bst( bst_printkv_func pf, bst_freev_func ff )
{
	// TASK 3a: IMPLEMENT THIS.
	return NULL;
}


// bintree t = makenode( key, value );
//	Make a new bintree node containing (<key>,<value>),
//	duplicating <key> via strdup().
//	Abort if any memory allocation - even the strdup() - fails.
//
static bintree makenode( char *key, void *value )
{
	// TASK 3b: IMPLEMENT THIS.
	return NULL;
}


// add_bst( b, key, value );
//	Add (<key>,<value>) to <b>.  If the given key is already present,
//	free it's old value and then store <value> instead,
//	otherwise add a completely new (<key>,<value>) node.
//	The key (but NOT the value) should be duplicated via strdup().
//	Abort if any memory allocation fails.
//
void add_bst( bst b, char *key, void *value )
{
	// TASK 3c: IMPLEMENT THIS.
}


// bintree result = find( t, k );
//	If <k> is not in <t>, return null.
//	Otherwise return the ptr to the node containing <k> and
//	it's corresponding value.
//
static bintree find( bintree t, char *k )
{
	while( t != NULL )
	{
		int cmp = strcmp( k, t->key );
	if( cmp == 0 ) return t;
		t = ( cmp<0 ) ? t->left : t->right;
	}
	return NULL;
}


// bool present = in_bst( b, key );
//	Return true iff <key> is in <b>.  Else return false.
//
bool in_bst( bst b, char *key )
{
	assert( b != NULL );
	bintree t = find( b->t, key );
	return t != NULL;
}


// void * value = get_bst( b, key );
//	If <key> is in <b>, return the value.  Else return NULL.
//
void * get_bst( bst b, char *key )
{
	assert( b != NULL );
	bintree t = find( b->t, key );
	return t != NULL ? t->value : NULL;
}


// each_bintree( t, &elementfunc, state );
//	Iterate over every (k,v) pair in <t>, invoking the
//	given per-kv callback function for each pair
//	(with the key, the value and the state pointer as parameters)
//
static void each_bintree( bintree t, bst_kv_func kvf, void *state )
{
	assert( t != NULL );
	if( t->left != NULL ) each_bintree( t->left, kvf, state );
	(*kvf)( t->key, t->value, state );
	if( t->right != NULL ) each_bintree( t->right, kvf, state );
}


// foreach_bst( b, &elementfunc, state );
//	Iterate over every (k,v) pair in <b>, invoking the
//	given per-element callback function for each pair
//	(with the key, the value and the state pointer as parameters)
//
void foreach_bst( bst b, bst_kv_func kvf, void *state )
{
	assert( b != NULL );
	if( b->t != NULL ) each_bintree( b->t, kvf, state );
}


// int ip = print_bintree( t, pf, out, itemsprinted );
//	Print the bintree <t> to <out>, as an ordered (key,value) sequence,
//	separated by commas, and invoking the print function <pf>
//	for each value.  <itemsprinted> is the number of items already printed
//	Returns the number of items printed after printing this bintree.
//
static int print_bintree( bintree t, bst_printkv_func pf, FILE *out, int ip )
{
	assert( t != NULL );
	if( t->left != NULL ) ip = print_bintree( t->left, pf, out, ip );
	if( ip>0 ) fputc( ',', out );
	(*pf)( out, t->key, t->value );
	ip++;
	if( t->right != NULL ) ip = print_bintree( t->right, pf, out, ip );
	return ip;
}


// print_bst( b, out );
//	Print the bst <b> to <out>, as an ordered (key,value) sequence,
//	separated by commas, and invoking the built in print function
//	for each (key,value) pair.
//
void print_bst( bst b, FILE *out )
{
	assert( b != NULL );
	assert( b->t != NULL );
	fprintf( out, "{ " );
	(void) print_bintree( b->t, b->pf, out, 0 );
	fprintf( out, " }" );
}


// free_bintree( t, &freefunc );
//	free bintree <t>.
//
static void free_bintree( bintree t, bst_freev_func ff )
{
	assert( t != NULL );
	if( t->left != NULL  ) free_bintree( t->left, ff );
	if( t->right != NULL ) free_bintree( t->right, ff );
	if( ff != NULL )       (*ff)( t->value );
	free( t->key );			// was strdup()ed, remember
	free( t );
}


// free_bst( b );
//	Free the given bst <b>, invoking the free function
//	for each value.
//
void free_bst( bst b )
{
	assert( b != NULL );
	assert( b->t != NULL );
	free_bintree( b->t, b->ff );
	free( b );
}
