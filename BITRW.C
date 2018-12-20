 /* Zipping programm based on Huffman's algorithm 
  * File which contains functions to work with bit writing 
  * Protected by GNU GPL license v3 */

#include <stdio.h>

static FILE *BitF;
static int BitAccum, BitPos;

/* Bit writing init function */
void WriteBitInit( FILE *F )
{
  BitF = F;
  BitAccum = 0;
  BitPos = 7;
} /* End of 'WriteBitInit' function */

/* Bit writing function */
void WriteBit( int Bit )
{
  BitAccum |= Bit << BitPos--;
  
  if (BitPos < 0)
  {
    fputc(BitAccum, BitF);
    BitAccum = 0;
    BitPos = 7;
  }   
} /* End of 'WriteBit' function */

/* Bit writing close function */
void WriteBitClose( void )
{
  if (BitPos < 7)
    fputc(BitAccum, BitF);
} /* End of 'WriteBitClose' function */

/* Bit reading init function */
void ReadBitInit( FILE *F )
{
  BitF = F;
  BitPos = -1;
} /* End of 'WriteBitInit' function */   

/* Bit reading function */
int ReadBit( void )
{
  if (BitPos < 0)
    BitAccum = fgetc(BitF), BitPos = 7;
  return (BitAccum >> BitPos--) & 1;
} /* End of 'ReadBit' function  */

/* END OF 'BITRW.C' FILE */

