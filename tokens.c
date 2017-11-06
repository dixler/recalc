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


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tokens.h"

#define isdigit(a) ((a) >= '0' && (a) <= '9')
#define isspace(a) ((a) == ' ')

typedef struct Token Token;

typedef struct Token{
   TokenType type;
   char op;
   int val;
}Token;

typedef struct TokenReader{
     char *inputline;  // this assumes that all input lines are 300 characters or less in length
     int needline;
     int pos;
}TokenReader;

// Initialize to a specific TokenType
Token* token_create(TokenType t)
{
   Token *tok = (Token*)malloc(sizeof(Token));
   tok->type = t;
   tok->op = '$';
   tok->val = -999;
   return tok;
}

void token_free(Token *t)
{
   free(t);
   return;
}

// Set to a specific TokenType
void token_set_to_type(Token *tok, TokenType t)
{
   tok->type = t;
   tok->op = '$';
   tok->val = -999;
}

// Set to a OPERATOR TokenType with specific operator value
void token_set_to_operator(Token *t, char c)
{
   t->type = OPERATOR;
   t->op = c;
   t->val = -999;
}

// Set to a VALUE TokenType with a specific numeric value
void token_set_to_value(Token *t, int v)
{
   t->type = VALUE;
   t->op = '$';
   t->val = v;
}

// return true if the Current Token is of the given TokenType
int token_equals_type(Token *tok, TokenType t)
{
   if(tok->type == t)
      return 1;
   else
      return 0;
}

// return true if the Current Token is of the OPERATOR TokenType
//     and contains the given operator character
int token_equals_operator(Token *tok, char c)
{
   if(tok->type == OPERATOR && tok->op == c)
      return 1;
   else
      return 0;
}

// Return the Operator for the current Token
//   verify the current Token is of the OPERATOR TokenType
char token_get_operator(Token *tok){
   if(tok->type == OPERATOR)
      return tok->op;
   else
      return '$';   // using $ to indicate an error value
}

// Return the Value for the current Token
//   verify the current token is of the value TokenType
int token_get_value(Token *tok)
{
   if(tok->type == VALUE)
      return tok->val;
   else
      return -999;  // using -999 to indicate an error value
}


// initialize the TokenReader class to read from Standard Input
TokenReader *TokenReader_create(char *line)
{
   // set to read from Standard Input
   TokenReader *tr = (TokenReader*)malloc(sizeof(TokenReader));
     tr->inputline = line;  // this assumes that all input lines are 300 characters or less in length
     tr->needline = 0;
     tr->pos = 0;
     return tr;
}

// Return the next Token from the input line
Token *TokenReader_get_next_token(TokenReader *tr) {

   //printf ("getToken %d, %d, %s\n", pos, needline, inputline);

   // skip over any white space characters in the beginning of the input
   while( tr->pos < (signed)strlen(tr->inputline) && isspace(tr->inputline[tr->pos]) )
   {
      tr->pos++;
   }

   // check for the end of the current line of input
   if(tr->pos >= (signed)strlen(tr->inputline))
   { // at end of line
      tr->needline = 1;
      Token *t = token_create(EOLN);
      return t;
   }

   // Get the next character from the input line
   char ch = tr->inputline[tr->pos]; tr->pos++;

   // check if 'q' or 'Q' was entered ==> QUIT Token
   if( 'q' == ch || 'Q' == ch )
   {
      return token_create(QUIT);
   }

   // check if "?" was entered ==> HELP Token
   if( '?' == ch )
   {
      return token_create(HELP);
   }

   // check for Operator values of: + - * / ^ ( )  ==> OPERATOR Token
   if(   ('+' == ch) || ('-' == ch) || ('*' == ch) || ('^' == ch) 
      || ('/' == ch) || ('(' == ch) || (')' == ch) )
   {
      Token *t = token_create(ERROR);
      token_set_to_operator( t, ch );
      return t;
   }

   // check for a number  ==> VALUE Token
   if(isdigit(ch)){
      int number = ch - '0';  // subtract ascii value of ch from ascii value of '0'

      ch = tr->inputline[tr->pos]; tr->pos++;
      while(isdigit(ch)){
         number = number * 10 + ch - '0';
         ch = tr->inputline[tr->pos]; tr->pos++;
      }
      tr->pos--; // since number calcuation check one character after the last digit
      Token *t = token_create(ERROR);
      token_set_to_value( t, number );
      return t;
   }

   // Input in not valid if code get to this part ==> ERROR Token
   return token_create(ERROR);
}


