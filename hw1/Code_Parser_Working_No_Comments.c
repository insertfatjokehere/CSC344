#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[]) {

  FILE *file;
  char c;
  int keywordCount = 0;
  char stack[50];
  int stackPointer = 0;
  int boolWord = 0;
  int boolQuote = 0;
  int boolSingle = 0;
  int boolCommentBlock = 0;
  int boolCommentWarning = 0;

	if (argc != 2) {
	  printf("Usage: %s filename\n", argv[0]);

	} else {


	  file = fopen(argv[1], "r");

      if (file == NULL) {
      	printf("Could not read/open file\n");

      } else {

        int check;
        unsigned char tempLetter;
        char letter;
        int counter = 0;
        getNextChar:
        while((check = fgetc(file)) != EOF) {

          tempLetter = (unsigned char)check;
          letter = tempLetter;

          processState: 

          if ((letter == '[' || letter == '{' || letter == '(') && boolQuote == 0 && boolSingle == 0) {
            stack[stackPointer] = letter;
            stackPointer++;

            if (boolWord == 1) {
              keywordCount++;
              boolWord = 0;
            }
          }

          else if (letter == '\\') {

            if (stack[stackPointer - 1] == '\\') {
                stackPointer--;
            } else {
              stack[stackPointer] = letter;
              stackPointer++;

              if (boolWord == 1) {
               keywordCount++;
               boolWord = 0;
              }
            }

          }

          else if (isalpha(letter) && stack[stackPointer - 1] != '\\' && (boolQuote == 0 && boolSingle == 0)) {
            
            boolWord = 1;
          }

          else if (isalpha(letter) && stack[stackPointer - 1] == '\\') {
            stackPointer--;
          }

          else if (letter == ' ' && boolWord == 1 && (boolQuote == 0 && boolSingle == 0)) {
            keywordCount++;
            boolWord = 0;
          }

          else if ((letter == ']' || letter == '}' || letter == ')') && boolQuote == 0 && boolSingle == 0) {

            if (boolWord == 1) {
              keywordCount++;
              boolWord = 0;
            }

            int tempPointer = stackPointer - 1;

            if (stackPointer == 0 || !((letter == ']' && stack[tempPointer] == '[') || (letter == '}'
                && stack[tempPointer] == '{') || (letter == ')' && stack[tempPointer] == '('))) {

              printf("=================================================================\n");
              printf("Error: There was no left handed matching symbol for %c\n", letter);
              printf("Error: Number of keywords found before hand: %d\n", keywordCount);
              printf("=================================================================\n");
              return 1;

            } else {
              stackPointer--;
            }

          }

          else if ((stack[stackPointer - 1] != '\\' || stackPointer == 0) && boolQuote == 0 && boolSingle == 0 && letter == '\"') {
            boolQuote = 1;
            stack[stackPointer] = letter;
            stackPointer++;

            if (boolWord == 1) {
              keywordCount++;
              boolWord = 0;
            }
          }

          else if ((stack[stackPointer - 1] != '\\' || stackPointer == 0) && boolQuote == 0 && boolSingle == 0 && letter == '\'') {
            boolSingle = 1;
            stack[stackPointer] = letter;
            stackPointer++;

            if (boolWord == 1) {
              keywordCount++;
              boolWord = 0;
            }
          }

          else if (stack[stackPointer - 1] == '\\' && (letter == '\'' || letter == '\"')) {

              stackPointer--;
          }

          else if (stack[stackPointer - 1] != '\\' && boolQuote == 1 && boolSingle == 0 && letter == '\"') {

            boolQuote = 0;
            stackPointer--;
            if (boolWord == 1) {
              keywordCount++;
              boolWord = 0;
            }

          }

          else if (stack[stackPointer - 1] != '\\' && boolQuote == 1 && boolSingle == 1 && letter == '\"') {

            if (stack[stackPointer - 1] == '\"') {
              boolQuote = 0;
              stackPointer--;

              if (boolWord == 1) {
                keywordCount++;
                boolWord = 0;
              }
            } else {

              printf("=================================================================\n");
              printf("Error: There is no matching double quotation inside a single quotation\n");
              printf("Error: Number of keywords found before hand: %d\n", keywordCount);
              printf("=================================================================\n");
              return 1;
            }

          }


          else if (stack[stackPointer - 1] != '\\' && boolQuote == 0 && boolSingle == 1 && letter == '\'') {

            boolSingle = 0;
            stackPointer--;

            if (boolWord == 1) {
              keywordCount++;
              boolWord = 0;
            }

          }

          else if (stack[stackPointer - 1] != '\\' && boolQuote == 1 && boolSingle == 1 && letter == '\'') {

            if (stack[stackPointer - 1] == '\'') {
              boolSingle = 0;
              stackPointer--;

              if (boolWord == 1) {
                keywordCount++;
                boolWord = 0;
              }

            } else {

              printf("=================================================================\n");
              printf("Error: There is no matching double quotation inside a single quotation\n");
              printf("Error: Number of keywords found before hand: %d\n", keywordCount);
              printf("=================================================================\n");
              return 1;

            }
          }

          else if (stack[stackPointer - 1] != '\\' && boolQuote == 1 && boolSingle == 0 && letter == '\'') {

            boolSingle = 1;
            stack[stackPointer] = letter;
            stackPointer++;

          }

          else if (stack[stackPointer - 1] != '\\' && boolQuote == 0 && boolSingle == 1 && letter == '\"') {

            boolQuote = 1;
            stack[stackPointer] = letter;
            stackPointer++;

          }

        }
      }


      printf("=================================================================\n");
      printf("========================== End of File ==========================\n");
      printf("=================================================================\n");
      if (boolWord == 1) { boolWord == 0; keywordCount++; }
      printf("Number of keywords found: %d\n", keywordCount);
      fclose(file);

      if (stackPointer != 0 || boolQuote == 1 || boolSingle == 1) {

        printf("Error: There are one or more symbol(s) that dont match\n");

        int i = 0;
        for (i; i < stackPointer; ++i) {

             printf("Error: matching character missing for %c\n", stack[i]);
        }

          if (boolQuote == 1) {
            printf("Error: missing matching double quotation\n");
          }

          if (boolSingle == 1) {
            printf("Error: missing matching single quotation\n");
          }

        printf("=================================================================\n");

        return 1;
      } else {

       printf("The Code has all matching symbols\n");
       printf("=================================================================\n");
       return 0;

    }
  } 
}
