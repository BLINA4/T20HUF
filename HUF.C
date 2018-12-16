 /* Zipping programm based on Huffman's algorithm 
  * Algorithm file 
  * Protected by GNU GPL license v3 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "HUF.H"
#include "BITRW.H"

TREE *Nodes[256];
long Freq[256];
int NumOfNodes;

CODE NewCodes[256], CurCode;

/* Frequencies counting function */
void CountFreq( FILE *F )
{
  int i;

  /* Frequencies clearing */
  memset(Freq, 0, sizeof Freq);

  /* Gathering frequencies info */
  while ((i = fgetc(F)) != EOF)
    Freq[i]++;
} /* End of 'CountFreq' function */

/* Tree placing function */
void PlaceNode( char Ch, long Freq, TREE *L, TREE *R )
{
  int i;  
  TREE *T;

  if ((T = malloc(sizeof(TREE))) == NULL)
    return;

  T->Ch = Ch;
  T->Freq = Freq;
  T->Left = L;
  T->Right = R;

  /* In that part of function, we create "forest" of frequencies in Big->Low sequency */
  i = NumOfNodes - 1;
  while (i >= 0 && T->Freq > Nodes[i]->Freq)
    Nodes[i + 1] = Nodes[i], i--;
  Nodes[i + 1] = T;
  NumOfNodes++;
} /* End of 'PlaceNode' function */

/* New codes building function */
void BuildCodes( TREE *T )
{
  if (T->Left == NULL)
  {
    NewCodes[(unsigned char)T->Ch] = CurCode;
    return;  
  }

  CurCode.Code[CurCode.Len++] = 0;
  BuildCodes(T->Left);
  CurCode.Len--;

  CurCode.Code[CurCode.Len++] = 1;
  BuildCodes(T->Right);
  CurCode.Len--;
} /* End of 'BuildCodes' function */

/* Statistic function */
int Stat ( char *FileName )
{
  int i, total, unpack;
  double ratio;
  FILE *F;

  F = fopen(FileName, "rb");
  if (F == NULL)
  {
    printf("File not found!\n");  
    return 0;
  }
  
  CountFreq(F);

  for (i = 0; i < 256; i++)
    if (Freq[i] > 0)
      PlaceNode(i, Freq[i], NULL, NULL);

  while (NumOfNodes != 1)
  {  
    TREE 
      *Last = Nodes[--NumOfNodes],
      *PreLast = Nodes[--NumOfNodes];
   
    PlaceNode(' ', Last->Freq + PreLast->Freq, PreLast, Last);
  }    
  
  CurCode.Len = 0;
  BuildCodes(Nodes[0]);
  
  total = 0;
  for (i = 0; i < 256; i++)
    total += NewCodes[i].Len * Freq[i];
  unpack = Nodes[0]->Freq * 8;
  ratio = (double)unpack / total;

  printf("The best compress ratio is %lf\n", ratio);

  fclose(F);
  return 1;
} /* End of 'stat' function */

/* File compressing function */
int Compress( char *OutFileName, char *InFileName )
{
  int i, ch;
  FILE *InF, *OutF;

  InF = fopen(InFileName, "rb"); 

  if (InF == NULL)
  { 
    printf("File not found!\n");  
    return 0;
  }

  OutF = fopen(OutFileName, "wb");

  if (OutF == NULL)
  {
    printf("File creating error!\n");
    return 0;
  }    

  /* Count frequencies */
  CountFreq(InF);

  /* Build "forest" */
  for (i = 0; i < 256; i++)
    if (Freq[i] > 0)
      PlaceNode(i, Freq[i], NULL, NULL);
 
  /* Compressing to TREE */
  while (NumOfNodes != 1)
  {
    TREE
      *Last = Nodes[--NumOfNodes],
      *PreLast = Nodes[--NumOfNodes];
 
    PlaceNode(' ', Last->Freq + PreLast->Freq, PreLast, Last);
  }
  
  /* Building new codes */
  CurCode.Len = 0;
  BuildCodes(Nodes[0]);
  
  /* Write bits to file */
  rewind(InF);
  fwrite(Freq, 4, 256, OutF);
  WriteBitInit(OutF);
  while ((ch = fgetc(InF)) != EOF)
    for (i = 0; i < NewCodes[ch].Len; i++)
      WriteBit(NewCodes[ch].Code[i]);
  WriteBitClose();

  fclose(InF);
  fclose(OutF); 
  return 1;
} /* End of 'Compress' function */

/* File decompressing function */
int Decompress( char *OutFileName, char *InFileName )
{
  int i, n;
  TREE *T;
  FILE *InF, *OutF;
 
  InF = fopen(InFileName, "rb");
 
  if (InF == NULL)
  {
    printf("File not found!\n");
    return 0;
  }
 
  OutF = fopen(OutFileName, "wb");

  if (OutF == NULL)
  {
    printf("File creating error!\n");
    return 0;
  }
 
  /* Reading frequencies */
  fread(Freq, 4, 256, InF);
 
  /* Building "forest" */
  for (i = 0; i < 256; i++)
    if (Freq[i] > 0)
      PlaceNode(i, Freq[i], NULL, NULL);

  /* Compressing to TREE */
  while (NumOfNodes != 1)
  {
    TREE
      *Last = Nodes[--NumOfNodes],
      *PreLast = Nodes[--NumOfNodes];
 
    PlaceNode(' ', Last->Freq + PreLast->Freq, PreLast, Last);
  }
   
  /* Reading bits */
  rewind(InF);
  n = Nodes[0]->Freq;
  T = Nodes[0];
  ReadBitInit(InF);
  while (n > 0)
    if (T->Left == NULL)
    {
      fputc(T->Ch, OutF);
      n--;
      T = Nodes[0];
    }  
    else
      if (ReadBit())
        T = T->Right;
      else
        T = T->Left;
  
  fclose(InF);
  fclose(OutF);
  return 1;
} /* End of 'Decompress' function */

/* END OF 'HUF.C' FILE */

