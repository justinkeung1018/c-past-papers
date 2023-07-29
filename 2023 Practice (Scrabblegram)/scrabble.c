#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "player.h"

static void test_form_word( char *word, char *tiles ) {
  char played_tiles[10];
  bool success = form_word( word, tiles, played_tiles );
  printf( "The word '%s' %s be made from the tiles '%s'\n",
	  word, success ? "can" : "cannot", tiles );
  if( success ) {
    printf( "Played tiles: %s\n", played_tiles );
  }
}


static void test_hi_score( char *hand, ScoreModifier *sm, char *smdesc ) {
  char word[32];
  int score = highest_score( hand, sm, word );
  printf( "The highest scoring word that can be made from tiles '%s'\n", hand );
  printf( "with %s is: ", smdesc );
  if( score < 0 ) {
    printf( "(no word found)\n" );
  } else {
    printf( "'%s' (%d points)\n", word, score );
  }
  printf( "\n" );
}
  
int main( void ) {

  printf( "====================== Question 1 ======================\n" );

  printf( "Tile score for 'P' is %d\n", tile_score('P') );
  printf( "Tile score for 'i' is %d\n", tile_score('i') );
  printf( "Tile score for 'Z' is %d\n", tile_score('Z') );
  printf( "Tile score for '?' is %d\n", tile_score('?') );
  printf( "Tile score for '2' is %d\n", tile_score('2') );
  printf( "\n" );

  printf( "====================== Question 2 ======================\n" );

  test_form_word( "LEAP", "ZLEAP?S" );
  test_form_word( "APPLES", "ZLEAP?S" );
  test_form_word( "SPATZLE", "ZLEAP?S" );
  test_form_word( "PIZZA", "ZLEAP?S" );
  printf( "\n" );

  printf( "====================== Question 3 ======================\n" );

  ScoreModifier sm0[]={NONE, TRIPLE_LETTER, NONE, NONE, DOUBLE_WORD, NONE, NONE};

  int score = compute_score("LEAP", sm0); 
  printf( "LEAP scores %d\n", score );
  
  score = compute_score("LEAPS", sm0);
  printf( "LEAPS scores %d\n", score );

  score = compute_score("AP?LES", sm0);
  printf( "AP?LES scores %d\n", score );

  score = compute_score("SPA?ZLE", sm0);
  printf( "SPA?ZLE scores %d\n", score );

  printf( "\n" );

  printf( "================== Highest Word Scores =================\n" );

  ScoreModifier sm1[]={NONE, NONE, DOUBLE_LETTER, NONE, NONE, NONE, NONE};

  test_hi_score( "WBNNOER", sm1,
  	"a double letter score on the third letter" );

  ScoreModifier sm2[]={DOUBLE_LETTER, NONE, NONE, NONE, NONE, TRIPLE_WORD, NONE};
  test_hi_score( "N?ESHRH", sm2,
  	"a double letter score on the 1st position\n"
	"and a triple word score on the 6th position" );

  ScoreModifier sm3[]={NONE, NONE, NONE, NONE, TRIPLE_LETTER, NONE, DOUBLE_WORD};
  test_hi_score( "L?IEY?A", sm3,
  	"a triple letter score on the 5th position\n"
	"and a double word score on the 7th position" );

  printf( "======================= The End ========================\n" );
  
  return 0;
}
