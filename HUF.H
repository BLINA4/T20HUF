 /* Zipping programm based on Huffman's algorithm 
  * Header file for 'HUF.C' 
  * Protected by GNU GPL license v3 */

#ifndef HUF
#define HUF

typedef struct tagTREE TREE;
struct tagTREE
{
  char Ch;
  long Freq;
  TREE *Left, *Right;
};

typedef struct
{
  int Code[47], Len;
} CODE;

void PlaceNode( char Ch, long Freq, TREE *L, TREE *R );
void CountFreq( FILE *F );
void CuildCodes( TREE *T );
int Stat( char *FileName );
int Compress( char *InFileName, char *OutFileName );

#endif

/* END OF 'HUF.H' FILE */

