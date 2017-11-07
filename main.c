/* This file contains the user interface code for the Infix Evaluation Project
 *  Project 5 for CS 211 for Fall 2017
 *
 * Date: 10/21/17
 *
 * Authors: Pat Troy, Kyle Dixler
 *
 */

#define DEBUGMODE if(debugMode)
#include <ncurses.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "stack.h"
#include "tokens.h"
#include "cursor.h"


#define UP 65
#define DOWN 66
#define BACKSPACE 127
#define ENTER 10
#define DELETE 126
#define SPACE 32
#define HOME 72
#define END 70

int debugMode = 0;

void drawString(char *string) {
   int len = strlen(string);
   stack *expo_stack = stk_create(free);
   {
      int *node = (int*)malloc(sizeof(int));
      *node = 0;
      stk_push(expo_stack,(void*)node);
   }
   int x = 0, y = 8;
   int offset_y = 0;
   for(int index = 0; index < len; index++){
      // print the character
      if(isdigit(string[index]))
         mvprintw(y-offset_y, x++, "%c", string[index]);
      else{
         if(string[index] == '^'){
            mvprintw(y-offset_y, x++, "%c", string[index]);
            // we have an exponent so increase height
            *(int*)stk_top(expo_stack) += 1;
            --x;
            offset_y += 1;
            continue;
         }
         else if(string[index] == '('){
            // we have an expression so it may have its own stuff
            int *node = (int*)malloc(sizeof(int));
            *node = 0;
            stk_push(expo_stack,(void*)node);
         }
         else if(string[index] == ')'){
            // we finished the expression pop it off the stack
            offset_y -= *(int*)stk_top(expo_stack);
            stk_pop(expo_stack);
         }
         else{
            // we don't have an exponent to increase height so just
            // reset the height back to normal
            offset_y -= *(int*)stk_top(expo_stack);
            *(int*)stk_top(expo_stack) = 0;
         }
         mvprintw(y-offset_y, x++, "%c", string[index]);
      }
   }
   
   stk_free(expo_stack);
   return;
}

void printCommands() {
   printf ("The commands for this program are:\n\n");
   printf ("q - to quit the program\n");
   printf ("? - to list the accepted commands\n");
   printf ("or any infix mathematical expression using operators of (), *, /, +, -\n");
   return;
}


int eval (Token *val1, Token *op, Token *val2){
   int result = -999;
   if(val1 == NULL || op == NULL || val2 == NULL){
      return result;
   }

   switch(token_get_operator(op)){
      case '^':
         // exponent the arguments
         result = (int)pow((double)token_get_value(val1),(double)token_get_value(val2));
         break;
      case '*':
         // multiply the arguments
         result = (token_get_value(val1) * token_get_value(val2));
         break;
      case '/':
         result = (token_get_value(val1) / token_get_value(val2));
         break;
      case '-':
         result = (token_get_value(val1) - token_get_value(val2));
         break;
      case '+':
         result = (token_get_value(val1) + token_get_value(val2));
         break;
      default:
         fprintf(stderr, "passed illegal operator \'%c\'\n", token_get_operator(op));
   }
   return result;
}

void stk_popAndEval(stack *ValueStack, stack *OperatorStack){
   Token *op = stk_top(OperatorStack);
   stk_pop(OperatorStack);
   Token *v2 = stk_top(ValueStack);
   stk_pop(ValueStack);
   Token *v1 = stk_top(ValueStack);
   stk_pop(ValueStack);
   Token *v3 = token_create(VALUE);
   token_set_to_value(v3, eval ( v1, op, v2 ));
   //printf("evaluating: %d %c %d = %d\n",  v1.token_get_value(), op.token_get_operator(), v2.token_get_value(), v3->token_get_value());


   stk_push(ValueStack, v3);
}

int *processExpression (Token *inputToken, TokenReader *tr) {
   /**********************************************/
   /* Declare both stack head pointers here      */
   stack *ValueStack = stk_create(token_free);
   stack *OperatorStack = stk_create(token_free);
   int *solution = NULL;

   /* Loop until the expression reaches its End */
   while (!token_equals_type(inputToken,EOLN))
   {
      /* The expression contain a VALUE */
      if (token_equals_type(inputToken,VALUE))
      {
         /* make this a debugMode statement */
         DEBUGMODE printf("Val: %d, ", token_get_value(inputToken) );

         // add code to perform this operation here
         Token *tok = token_create(VALUE);
         token_set_to_value(tok, token_get_value(inputToken));

         stk_push(ValueStack,tok);
      }

      /* The expression contains an OPERATOR */
      else if (token_equals_type(inputToken,OPERATOR))
      {
         /* make this a debugMode statement */
         DEBUGMODE printf ("OP: %c, ", token_get_operator(inputToken) );

         // add code to perform this operation here
         Token *optok = token_create(OPERATOR);
         token_set_to_operator(optok, token_get_operator(inputToken));

         if(token_get_operator(inputToken) == '('){     
            // if ( the current operator is an Open Parenthesis ){
            stk_push(OperatorStack,optok);            
            // push the Open Parenthesis onto the OperatorStack 
         }
         else if(token_get_operator(inputToken) == '+' || token_get_operator(inputToken) == '-'){     
            // if ( the current operator is + or - ){
            while(!stk_is_empty(OperatorStack) && (
                        token_get_operator(stk_top(OperatorStack)) == '+' || token_get_operator(stk_top(OperatorStack)) == '-'
                     || token_get_operator(stk_top(OperatorStack)) == '*' || token_get_operator(stk_top(OperatorStack)) == '/'
                     || token_get_operator(stk_top(OperatorStack)) == '^')){

               // while ( the OperatorStack is not Empty && the top of the OperatorStack is +, -, * or / ){
                  stk_popAndEval(ValueStack, OperatorStack);   
                  // popAndEval ( )
            }
            stk_push(OperatorStack,optok);              
            // push the current operator on the OperatorStack
         }
         else if(token_get_operator(inputToken) == '*' || token_get_operator(inputToken) == '/'){   
            // if ( the current operator is * or / )
            while(!stk_is_empty(OperatorStack) && 
                  (token_get_operator(stk_top(OperatorStack)) == '*' || token_get_operator(stk_top(OperatorStack)) == '/'
                     || token_get_operator(stk_top(OperatorStack)) == '^')){
               // while ( the OperatorStack is not Empty && the top of the OperatorStack is * or / )
               stk_popAndEval (ValueStack, OperatorStack);
            }
            stk_push(OperatorStack,optok);              
            // push the current operator on the OperatorStack
         }
         else if(token_get_operator(inputToken) == '^'){   
            // if ( the current operator is ^ )
            while(!stk_is_empty(OperatorStack) && 
                  (token_get_operator(stk_top(OperatorStack)) == '^')){
               // while ( the OperatorStack is not Empty && the top of the OperatorStack is * or / )
               stk_popAndEval (ValueStack, OperatorStack);
            }
            stk_push(OperatorStack,optok);              
            // push the current operator on the OperatorStack
         }
         else if(token_get_operator(inputToken) == ')'){     
            // if ( the current operator is a Closing Parenthesis )
            while(!stk_is_empty(OperatorStack) && token_get_operator(stk_top(OperatorStack)) != '('){  
               // while ( the Operator Stack is not Empty && the top of the OperatorStack is not an Open Parenthesis ){
               stk_popAndEval (ValueStack, OperatorStack);
            }
               if(stk_is_empty(OperatorStack)){   
                  // if (the OperatorStack is Empty )
                  fprintf(stderr, "error OperatorStack is empty\n"); 
                  // print an error message
               }
               else{
                  stk_pop(OperatorStack);  
                  // pop the Open Parenthesis from the OperatorStack
               }
         }
         else{
            printf("Illegal operator\n");
            Token *tok = token_create(VALUE);
            token_set_to_value(tok,-999);
            stk_push(ValueStack,tok); 
            // handle input cleanup
            goto INVALID;
         }
      }
         /* get next token from input */
         inputToken = TokenReader_get_next_token(tr);
   }

   /* The expression has reached its end */
   
   while(!stk_is_empty(OperatorStack)){   
      if(token_get_operator(stk_top(OperatorStack)) == '('){
         // unclosed braces
         printf("Unclosed parenthesis ignoring\n");
         goto INVALID;
      }
      stk_popAndEval (ValueStack, OperatorStack);
   }
   if(stk_top(ValueStack) != NULL){
      solution = (int*)malloc(sizeof(int));
      *solution = token_get_value(stk_top(ValueStack));
   }
   //printf("Evaluated to: %d\n", solution);
INVALID:
   while (!token_equals_type(inputToken,EOLN))
      inputToken = TokenReader_get_next_token(tr);
   stk_reset(OperatorStack);
   stk_reset(ValueStack);

   //printf ("\n");
   return solution;
}


int main(int argc, char *argv[]) {

   int cur_char = '\0';
   int row,col;
   cursor *Cursor = cursor_create();
   TokenReader *tr = TokenReader_create(NULL);
   Token *inputToken;
   initscr();
   noecho();
   getmaxyx(stdscr,row,col);
   char *string;
   int *solution = NULL;
   for(int num_char = 0;; num_char++){
      keypad(stdscr, TRUE);
      cur_char = getch();
      switch(cur_char){
         //case(KEY_BACKSPACE):
         case(BACKSPACE):
            cursor_backspace(Cursor);
            break;
         case(ENTER):
            break;
         case(KEY_DC):
            cursor_delete(Cursor);
            break;
         case(KEY_HOME):
            cursor_home(Cursor);
            break;
         case(KEY_END):
            cursor_end(Cursor);
            break;
         case(KEY_LEFT):
            cursor_prev(Cursor);
            break;
         case(KEY_RIGHT):
            cursor_next(Cursor);
            break;

         case(KEY_UP):
         case(KEY_DOWN):
            break;

         default:
            cursor_insert(Cursor, cur_char);
            break;
      }
      //char *rev_string = cursor_get_rev_string(Cursor);

      string = cursor_get_string(Cursor);
      TokenReader_set_line(tr, string);
      inputToken = TokenReader_get_next_token(tr);
      solution = processExpression(inputToken, tr);
      clear();
      // print string
      mvprintw(0,0, "%s", string);
      drawString(string);
      if(solution != NULL){
         mvprintw(1,0, "%d", *solution);
      }
      //mvprintw(2,0, "%s", rev_string);
      move(cursor_get_x(Cursor)/col,cursor_get_x(Cursor)%col);
      free(string);
      solution = 0;
      //free(rev_string);
   }
   endwin();

   return 0;
}


