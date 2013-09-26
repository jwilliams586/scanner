#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "scanner.h"
#include "token.h"

tokenType scanner(FILE *fp, int line)
{

   char heading[24] = {'0', '=', '<', '>', '.', ':', '!', '+', '-', '*', '/', '%', '(', ')', ',', '{', '}', ';', '[', ']', 'l', 'd', 'w', 'e'};

   int T[11][24] = {{1, 6, 5, 4, -26, -22, -14, -9, -10, -11, -12, -13, -16, -17, -15, -20, -21, -23, -18, -19, 7, 2, 1, -25},
			 	  {2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 2, -1, -1},
				  {3, -24, -24, -24, -24, -24, -24, -24, -24, -24, -24, -24, -24, -24, -24, -24, -24, -24, -24, -24, -24, 3, -24, -24},
				  {4, 11, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5},
				  {5, 10, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7},
				  {6, 9, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3},
				  {7, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, 7, 7, -2, -2},
				  {8, -26, -26, -26, -26, -26, -26, -26, -26, -26, -26, -26, -26, -26, -26, -26, -26, -26, -26, -26, -26, 3, -26, -26},
				  {9, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4},
				  {10, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8},
				  {11, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6}};

   int i = 0, j = 0, k = 0, m = 0, t = 1, eofCheck, noMatch = 0, tableCol, nextCol, state = 1, nextState = 1; 
   int tokenNum, totTokens = 0, totKeywords = 0, keywordMatch = 0;
   char currentChar, origChar, lookahead, origLook, currentString[20];
   char tokens[30][16], keywords[20][16], keywordTokens[20][16];
   FILE *file;
   tokenType r;

   file = fopen("tokens.txt", "r");

   if(!file)
   {
      printf("Error: token file did not open!\n");
   }

   if(file)
   {
	  while(fgets(tokens[i], 1024, file) != NULL)
	  {
		 tokens[i][strlen(tokens[i]) - 1] = '\0';
		 i++;
		 totTokens++;
	  }
   }
   fclose(file);
   i = 0;
  
   file = fopen("keywords.txt", "r");

   if(!file)
   {
      printf("Error: keyword file did not open!\n");
   }

   if(file)
   {
	  while(fgets(keywords[i], 1024, file) != NULL)
	  {
		 keywords[i][strlen(keywords[i]) - 1] = '\0';
	     i++;
		 totKeywords++;
	  }
   }
   fclose(file);
   i = 0;

   file = fopen("keywordTokens.txt", "r");

   if(!file)
   {
	  printf("Error: keyword tokens file did not open!\n");
   }

   if(file)
   {
 	  while(fgets(keywordTokens[i], 1024, file) != NULL)
	  {
		 keywordTokens[i][strlen(keywordTokens[i]) - 1] = '\0';
	     i++;
	  }
   }
   fclose(file);
   i = 0;

   memset(currentString, 0, sizeof currentString);
 
   
	  do {

      currentChar = tolower(getc(fp));  
	  origChar = currentChar;

	  lookahead = tolower(getc(fp));
	  origLook = lookahead;

      if(currentChar == 'ÿ')     // EOF
	  {
	     currentChar = 'e';
	  }

	  else if (currentChar == '#')
	  {
		 ungetc(origLook, fp);
         currentChar = getc(fp);
		 while(currentChar != '#')
		 {
			  currentChar = getc(fp);
		 }
		 currentChar = 'w';
		 lookahead = tolower(getc(fp));
		 origLook = lookahead;
	  }

	  else if(isspace(currentChar))
	  {
		 if(currentChar == '\n')
		 {
			currentChar = 'w';
		 }
	  }

      else if (isalpha(currentChar))
	  {
		 currentString[k++] = currentChar;
         currentChar = 'l';
	  }

	  else if (isdigit(currentChar))
	  {
		 currentString[k++] = currentChar;
		 currentChar = 'd';
	  }

	  else 
	  {
         currentString[k++] = currentChar;
      }

	  if(isspace(lookahead))
	  {
		 if(lookahead == '\n')
		    line++;
	     lookahead = 'w';
	  }

	  else if (isalpha(lookahead))
	  {
	     lookahead = 'l';
	  }

	  else if (isdigit(lookahead))
	  {
		 lookahead = 'd';
	  }

	  
	  for(m = 0; m < 24; m++)
	  {
	     if(heading[m] == currentChar)
		    tableCol = m;
         else
			noMatch++;

         if(heading[m] == lookahead)
			nextCol = m;
	  }
        
      if(noMatch == 24)
	  {
	     printf("Error!! Char %c on line %d not found in table!\n", currentChar, line);
         exit(0);
	  }

	  noMatch = 0;

	  state = T[state - 1][tableCol];
	  nextState = T[state - 1][nextCol];

	  if(!isspace(origLook))
	  {
	  ungetc(origLook, fp);
	  }

	  if(nextState < 0 || state < 0)
	  {
		 strcpy(r.str, currentString);
		 if(nextState < 0)
            tokenNum = nextState * (-1);
         if(state < 0)
			tokenNum = state * (-1);
		 if(tokenNum == 2)
		 {
			for(m = 0; m < totKeywords; m++)
			{
			   if(strcmp(r.str, keywords[m]) == 0)
			   {
				  keywordMatch = 1;
                  strcpy(r.tokenId, keywordTokens[m]);
			   }
            }
		    
			if(keywordMatch != 1)
			   strcpy(r.tokenId, tokens[tokenNum]);
		 }

		 else
		 {
		    strcpy(r.tokenId, tokens[tokenNum]);
		 }
		 r.line = line;

		 return r;
	  }
	  }while(state > 0 || nextState > 0);

}
