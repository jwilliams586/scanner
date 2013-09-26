#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "token.h"

int main(int argc, char *argv[])
{

   int i = 0;
   int line = 1;
   FILE *file;
   tokenType s;

   if(argc == 2)
	  file = fopen(argv[1], "r");

   if(argc == 1)
	  file = stdin;

   if(!file)
   {
      printf("Error: file did not open!\n");
	  return 1;
   }

   if(file)
   {
      while(1)
	  {
		 s = scanner(file, line);
		 printf("String:%s Token:%s Line#:%d\n", s.str, s.tokenId, s.line);
		 line = s.line;
		 if(strcmp(s.tokenId, "EOFtk") == 0)
			break;
	  }
	  fclose(file);
   }

   return 0;
}
