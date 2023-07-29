#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>

#include "player.h"

#define BLANK            '?'
#define BLANK_ALT        ' '
#define MAX_WORD_LENGTH  7
#define MAX_LENGTH_BONUS 50

static int scores[] = {1,3,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10};


// int score = tile_score( tile );
//	Return the tile score of the given tile:
//	The score of '?' or ' ' is 0;
//	the score of a letter is looked up in the scores[] array,
//	so 'a' or 'A' has score 1, 'b' or 'B' has score 3 etc,
//	and anything else has score -1.
//
int tile_score( char tile ) {
  if (tile == BLANK || tile == BLANK_ALT) {
    return 0;
  }
  
  if (!isalpha(tile)) {
    return -1;
  }
 
  tile = tolower(tile);
  
  return scores[tile - 'a']; 
}


// int score = compute_score( played_tiles, sm[] );
//	This computes the whole hand score, including letter scores,
//	score modifiers (letter-multiplying and whole-word-multiplying)
//	and "whole hand bonus".  Return the whole score.
//
int compute_score( char *played_tiles, ScoreModifier *sm ) {
  int word_length = strlen(played_tiles);
  int word_multiplier = 1;
  int word_score = 0;

  for (int i = 0; i < word_length; i++) {
    int score = tile_score(played_tiles[i]);

    switch (sm[i]) {
      case NONE:
        // Do nothing to the score
        break;

      case DOUBLE_LETTER:
        score *= 2;
        break;

      case TRIPLE_LETTER:
        score *= 3;
        break;

      case DOUBLE_WORD:
        word_multiplier = 2;
        break;

      case TRIPLE_WORD:
        word_multiplier = 3;
        break;
    }

    word_score += score;
  }

  word_score *= word_multiplier;

  if (word_length == MAX_WORD_LENGTH) {
    word_score += MAX_LENGTH_BONUS;
  }

  return word_score;
}


// bool canformword = form_word( word, tiles, played_tiles );
//	Attempts to form <word> from <tiles>.  If this can be done,
//	it sets <played_tiles> to the tiles that were used (ideally
//	in the same order as in <word>), and return true.
//	If it can't be done, return false.
//
bool form_word( char *word, char *tiles, char *played_tiles ) {
  if (!*word) {
    // The player has played sufficient tiles to form the word
    *played_tiles = '\0';
    return true;
  }
  
  char *tile = strchr(tiles, *word);

  if (tile == NULL) {
    // Character does not exist in tiles so we search for blanks
    tile = strchr(tiles, BLANK);
    if (tile == NULL) {
      return false;
    }
  }  

  // The current letter can be represented by a tile,
  // so we play the tile and remove it from the player's hand
  char remaining_tiles[strlen(tiles) + 1];
  strcpy(remaining_tiles, tiles);
  int tile_index = tile - tiles;
  remaining_tiles[tile_index] = '\0';
  strcat(remaining_tiles, tile + 1);

  if (form_word(word + 1, remaining_tiles, played_tiles + 1)) {
    *played_tiles = *tile; 
    return true;
  }

  return false;
}


// int maxsc = highest_score( tiles, sm[], word );
//	Given a particular collection of tiles and score modifiers,
//	work out the highest word score (and corresponding word) that can
//	be achieved using any of the words in the supplied dictionary.
//	If it not possible to make any word in the supplied dictionary from
//	the tiles then the function should return -1. Otherwise store
//	the word attaining the highest word score into word, and return
//	that highest word score.
//
int highest_score( char *tiles, ScoreModifier *sm, char *word ) {
  int max_score = -1;
  strcpy(word, "");

  FILE * in = fopen("words.txt", "r");
  if( in == NULL ) {
    fprintf( stderr, "Could not open dictionary file 'words.txt'\n" );
    return -1;
  }

  char target[512], played_tiles[80];
  while( fgets( target, 512, in ) != NULL ) {
    int len = strlen(target);
    if( len>0 && target[len-1] == '\n' ) target[len-1] = '\0';
    if( form_word(target, tiles, played_tiles) ) {
      int score = compute_score(played_tiles, sm);
      if( score > max_score ) {
	strcpy(word, target);
	max_score = score;
      }
    }
  }
  fclose( in );
  return max_score;
}
