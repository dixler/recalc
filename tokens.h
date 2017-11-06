/* This file contains the user interface code for the Infix Evaluation Project
 *  Project 5 for CS 211 for Fall 2017
 *
 * Date: 10/21/17
 *
 * Author: Pat Troy
 *
 */

/**************************************************************/
/*                                                            */
/*  The Code below this point should NOT need to be modified  */
/*  for this program.   If you feel you must modify the code  */
/*  below this point, you are probably trying to solve a      */
/*  more difficult problem that you are being asked to solve  */
/*                                                            */
/**************************************************************/

#include <stdio.h>
#include <string.h>

// Enumarated Type specifying all of the Tokens
typedef enum TokenType{
   ERROR, OPERATOR, VALUE, EOLN, QUIT, HELP, EOFILE
}TokenType;


// Class to hold the Token information
typedef struct Token Token;
/*{
  private:
  TokenType type;
  char      op;       // using '$' as undefined/error
  int       val;      // using -999 as undefined/error

  public:

*/
// Default to initialize to the ERROR TokenType
extern Token *token_create();

// free memory associated with token
extern void token_free(Token *t);

// Initialize to a specific TokenType
extern Token *token_create(TokenType t);

// Set to a specific TokenType
extern void token_set_to_type(Token *tok, TokenType t);

// Set to a OPERATOR TokenType with specific operator value
extern void token_set_to_operator(Token *tok, char c);

// Set to a VALUE TokenType with a specific numeric value
extern void token_set_to_value(Token *tok, int v);

// return true if the Current Token is of the given TokenType
extern int token_equals_type(Token *tok, TokenType t);

extern int token_get_type(Token *tok);

// return true if the Current Token is of the OPERATOR TokenType
//     and contains the given operator character
extern int token_equals_operator(Token *tok, char c);

// Return the Operator for the current Token
//   verify the current Token is of the OPERATOR TokenType
extern char token_get_operator (Token *tok);

// Return the Value for the current Token
//   verify the current token is of the value TokenType
extern int token_get_value(Token *tok);

typedef struct TokenReader TokenReader;
/*{
  private:
  char inputline[300];  // this assumes that all input lines are 300 characters or less in length
  bool needline;
  int pos;


  public:

*/
// initialize the TokenReader class to read from Standard Input
extern TokenReader *TokenReader_create(char *line);

// Force the next getNextToken to read in a line of input
extern void TokenReader_set_line(TokenReader *tr, char *line);

// Return the next Token from the input line
extern Token *TokenReader_get_next_token(TokenReader *tr);
//};

