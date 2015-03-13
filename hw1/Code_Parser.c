#include <stdio.h>
#include <ctype.h>


// Assignment 1 - CSC-344:
// -----------------------
// Write a program that:
//
// * Checks that all "begin" symbols in a C program are matched with their "end" symbols {...}, [...], (...),
//   "...", and '...', and if not, reports excess, missing, or improperly nested ones.
//
// * Counts the number of identifiers (including keywords). For example, reporting 7 for:
//   void func(int arg) { int index = 27; index += 1; }.
//
// * You can use any C library functions except for regular expression or parse libraries in your program.


int main(int argc, char *argv[]) {

  // Declarations
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
	  // File needs a file in the arguments, or the program throws this message
	  printf("Usage: %s filename\n", argv[0]);

	} else {

	  // Note: FILE, EOF and NULL are case sensitive

	  file = fopen(argv[1], "r");
	  // DEBUG:
          // printf("User entered a filename\n");

      if (file == NULL) {
      	printf("Could not read/open file\n");

      } else {

      	// DEBUG:
      	// printf("File was successfully opened\n");

        int check;
        unsigned char tempLetter;
        char letter;
        int counter = 0;
        getNextChar:
        while((check = fgetc(file)) != EOF) {

          // Note: fgetc(File *file) returns an unsigned char CASTED into an int
          // If this is used to compare with the EOF, which is negative. so to compare
          // these characters, you cast the int, into an unsigned char variable. then
          // optionally you can cast the unsigned char into a regular char.

          tempLetter = (unsigned char)check;
          letter = tempLetter;

          processState: 

          // Case 1: Keep Track of all ", ', {, [, ('s and add the counter
          // Note: To check all lefthanded symbols, use stackPointer - 1, and check if it's zero before hand
          if ((letter == '[' || letter == '{' || letter == '(') && boolQuote == 0 && boolSingle == 0) {
            // Debug
            // printf("Found a %c\n", letter);
            stack[stackPointer] = letter;
            // Debug
            // printf("Character added to stack point %d\n", stackPointer);
            stackPointer++;

            // Check if the previous character was a word
            // increment work count
            if (boolWord == 1) {
              keywordCount++;
              boolWord = 0;
            }
          }

          // Case 1.5: Keep Strack of 
          else if (letter == '\\') {

            if (stack[stackPointer - 1] == '\\') {
                stackPointer--;
                // Debug
                // printf("Found two double slashes\n");
            } else {
              // Debug
              // printf("Found a %c\n", letter);
              stack[stackPointer] = letter;
              // Debug
              // printf("Character added to stack point %d\n", stackPointer);
              stackPointer++;

              // Check if the previous character was a word
              if (boolWord == 1) {
              keywordCount++;
              // printf("End of identifier, current count: %d\n", keywordCount);
              boolWord = 0;
              }
            }

          }

          // Case 2: The letter is an alphabet character, aka an indentifier or keyword and not in quotes
          else if (isalpha(letter) && stack[stackPointer - 1] != '\\' && (boolQuote == 0 && boolSingle == 0)) {
            
            boolWord = 1;
          }

          // Case 3: There is a special character i.e. \n or \t
          else if (isalpha(letter) && stack[stackPointer - 1] == '\\') {
            // Debug
            // printf("Part of an escape character with: \\%c\n", letter);
            stackPointer--;
          }

          // Case 4: There is a space in between an indentifier and some other character
          else if (letter == ' ' && boolWord == 1 && (boolQuote == 0 && boolSingle == 0)) {
            keywordCount++;
            // Debug
            // printf("End of identifier, current count: %d\n", keywordCount);
            boolWord = 0;
          }

          // Case 5: You find ], }, )'s and check  to see if the there is an already existing symbol to match
          // if there is a match, decrement to counter by one afterwards.
          // if the array is not empty, check the most recent one
          else if ((letter == ']' || letter == '}' || letter == ')') && boolQuote == 0 && boolSingle == 0) {
            
    
            // check if there was a word before hand
            if (boolWord == 1) {
              keywordCount++;
              // Debug
              // printf("End of identifier, current count: %d\n", keywordCount);
              boolWord = 0;
            }

            int tempPointer = stackPointer - 1;
            // if there is nothing to match the the right handed symbols,
            // or there the characters dont match, throw an error
            if (stackPointer == 0 || !((letter == ']' && stack[tempPointer] == '[') || (letter == '}'
                && stack[tempPointer] == '{') || (letter == ')' && stack[tempPointer] == '('))) {

              printf("=================================================================\n");
              printf("Error: There was no left handed matching symbol for %c\n", letter);
              printf("Error: Number of keywords found before hand: %d\n", keywordCount);
              printf("=================================================================\n");
              return 1;

            } else {
              // the characters match
              stackPointer--;
              // Debug
              // printf("Character found a match %c\n", letter);
            }

          }

          // Case 6: Store the first quotation
          // Note the escape charactor is followed by a " OR a '

          else if ((stack[stackPointer - 1] != '\\' || stackPointer == 0) && boolQuote == 0 && boolSingle == 0 && letter == '\"') {
            boolQuote = 1;
            // Debug
            // printf("Start of double quotation\n");
            // printf("Found a %c\n", letter);
            stack[stackPointer] = letter;
            // Debug
            // printf("Character added to stack point %d\n", stackPointer);
            stackPointer++;

            // Check if the previous character was a word
            // Increment if word is was beforehand
            if (boolWord == 1) {
              keywordCount++;
              boolWord = 0;
            }
          }

          // Case 7: Store the first single quotation

          else if ((stack[stackPointer - 1] != '\\' || stackPointer == 0) && boolQuote == 0 && boolSingle == 0 && letter == '\'') {
            boolSingle = 1;
            // Debug
            // printf("Start of single quotation\n");
            // printf("Found a %c\n", letter);
            stack[stackPointer] = letter;
            // Debug
            // printf("Character added to stack point %d\n", stackPointer);
            stackPointer++;

            // Check if the previous character was a word
            if (boolWord == 1) {
              keywordCount++;
              // printf("End of identifier, current count: %d\n", keywordCount);
              boolWord = 0;
            }
          }

          // Case 8: This is the either the following character after the escape \
          // or this is the end of a quotation
          else if (stack[stackPointer - 1] == '\\' && (letter == '\'' || letter == '\"')) {

              stackPointer--;
              // Debug
              // printf("Escape character found: \\%c\n", letter);
              // printf("Escape character remove from stack, current point: %d\n", stackPointer);
          }

          // Case 9: 2 quotation " " marks with nothing inside

          else if (stack[stackPointer - 1] != '\\' && boolQuote == 1 && boolSingle == 0 && letter == '\"') {

            boolQuote = 0;
            // Debug
            // printf("End of Double Quotation\n");
            stackPointer--;
            // Check if the previous character was a word
            if (boolWord == 1) {
              keywordCount++;
              // printf("End of identifier, current count: %d\n", keywordCount);
              boolWord = 0;
            }

          }

          // Case 10: single quote ' inside 2 " " == " ' " which is an error
          else if (stack[stackPointer - 1] != '\\' && boolQuote == 1 && boolSingle == 1 && letter == '\"') {

            
            if (stack[stackPointer - 1] == '\"') {
              boolQuote = 0;
              // Debug
              // printf("End of Double Quotation\n");
              stackPointer--;

              // Check if the previous character was a word
              // increment if there was a word beforehand
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


          // Case 11: 2 single quotation '' marks with nothing inside

          else if (stack[stackPointer - 1] != '\\' && boolQuote == 0 && boolSingle == 1 && letter == '\'') {

            boolSingle = 0;
            // Debug
            // printf("End of single Quotation\n");
            stackPointer--;

            // Check if the previous character was a word
            if (boolWord == 1) {
              keywordCount++;
              // printf("End of identifier, current count: %d\n", keywordCount);
              boolWord = 0;
            }

          }

          // Case 12: double quote " inside 2 '' == ' " ' which is an error
          else if (stack[stackPointer - 1] != '\\' && boolQuote == 1 && boolSingle == 1 && letter == '\'') {

            if (stack[stackPointer - 1] == '\'') {
              boolSingle = 0;
              // Debug
              // printf("End of single Quotation\n");
              stackPointer--;

              // Check if the previous character was a word
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

          // Case 13: double quote then a single quote
          else if (stack[stackPointer - 1] != '\\' && boolQuote == 1 && boolSingle == 0 && letter == '\'') {

            boolSingle = 1;
            stack[stackPointer] = letter;
            // Debug
            // printf("Character added to stack point %d\n", stackPointer);
            stackPointer++;

          }

          // Case 14: single quote then a double quote
          else if (stack[stackPointer - 1] != '\\' && boolQuote == 0 && boolSingle == 1 && letter == '\"') {

            boolQuote = 1;
            stack[stackPointer] = letter;
            // Debug
            // printf("Character added to stack point %d\n", stackPointer);
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

        // Note: for (int i = 0; i < count; ++i) is only allowed in C99
        // with GCC, compile with option -std=c99 or -std=gnu99
        // otherwords, initialize variable outside of the loop

        int i = 0;
        for (i; i < stackPointer; ++i) {

          // There is a backslash that either escapes an important character
          // or id doesnt escape anything
          // if (stack[i] != '\\') {
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
