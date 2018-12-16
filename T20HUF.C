/* Zipping programm based on Huffman's algorithm 
 * Main programm file 
 * Protected by GNU GPL license v3 */

#include <stdio.h>

#include "HUF.C"

/* Main programm function */
void main( int argc, char *argv[] )
{   
  char InFileName[255], OutFileName[255];

  if (argc == 1)
  {
    printf("Enter mode (1 - Statistic, 2 - Compress, 3 - Decompress): \n");

    switch(getchar())
    {
    case '1':
      printf("Enter file name:\n");
      scanf("%s", &InFileName);
      Stat(InFileName);
      break;  
    case '2':
      printf("Enter file name (input): \n");
      scanf("%s", &InFileName);
      printf("Enter file name (output): \n");
      scanf("%s", &OutFileName);
      Compress(InFileName, OutFileName); 
      break;
    case '3':
      printf("Enter file name (input): \n");
      scanf("%s", &InFileName);
      printf("Enter file name (output): \n");
      scanf("%s", &OutFileName);
      Decompress(InFileName, OutFileName);
      break;
    }
  }
  else
    if (argv[1][0] == 'c')
      Compress(argv[3], argv[2]);
    else if (argv[1][0] == 'd')
      Decompress(argv[3], argv[2]);
    else if (argv[1][0] == 's')
      Stat(argv[2]);
    else 
      printf("Unrecongized option!\n");
} /* End of 'main' function */

/* END OF 'T20HUF.C' FILE */

