typedef enum {
	NONE, DOUBLE_LETTER, TRIPLE_LETTER, DOUBLE_WORD, TRIPLE_WORD
} ScoreModifier;

extern int tile_score( char tile );
extern int compute_score( char * played_tiles, ScoreModifier * sm );
extern bool form_word( char * word, char * tiles, char * played_tiles );
extern int highest_score( char * tiles, ScoreModifier * sm, char * word );

