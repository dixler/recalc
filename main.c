/* This file contains the user interface code for the Infix Evaluation Project
 *  Project 5 for CS 211 for Fall 2017
 *
 * Date: 10/21/17
 *
 * Authors: Pat Troy, Kyle Dixler
 *
 */

#define DEBUGMODE if(debugMode)
#include "stack.h"
#include "tokens.h"

int debugMode = 0;

void printCommands() {
   printf ("The commands for this program are:\n\n");
   printf ("q - to quit the program\n");
   printf ("? - to list the accepted commands\n");
   printf ("or any infix mathematical expression using operators of (), *, /, +, -\n");
   return;
}


int eval (Token *val1, Token *op, Token *val2){
   int result = -999;
   switch(token_get_operator(op)){
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

void processExpression (Token *inputToken, TokenReader *tr) {
   /**********************************************/
   /* Declare both stack head pointers here      */
   stack *ValueStack = stk_create();
   stack *OperatorStack = stk_create();

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
                     || token_get_operator(stk_top(OperatorStack)) == '*' || token_get_operator(stk_top(OperatorStack)) == '/')){

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
                  (token_get_operator(stk_top(OperatorStack)) == '*' || token_get_operator(stk_top(OperatorStack)) == '/')){
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
      }
      else{
         printf("Illegal operator\n");
         Token *tok = token_create(VALUE);
         token_set_to_value(tok,-999);
         stk_push(ValueStack,tok); 
         // handle input cleanup
         goto INVALID;
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
   printf("Evaluated to: %d\n", token_get_value(stk_top(ValueStack)));
INVALID:
   while (!token_equals_type(inputToken,EOLN))
      inputToken = TokenReader_get_next_token(tr);
   stk_reset(OperatorStack);
   stk_reset(ValueStack);

   printf ("\n");
}

int main(int argc, char *argv[])
{
   /***************************************************************/
   /* Add code for checking command line arguments for debug mode */
      /* verify the proper number of command line arguments were given */
   if(argc > 1) {
      for(int i = 1; i< argc; i++){
         // argv[0] is pwd
         if(!strcmp("-d", argv[i])){
            // debug mode
            debugMode = 1;
            printf (" Debugging Information \n"); 
         }
      }
   }



   Token *inputToken;
   TokenReader *tr = TokenReader_create();

   printf ("Starting Expression Evaluation Program\n\n");
   printf ("Enter Expression: ");


   inputToken = TokenReader_get_next_token(tr);

   while (!token_equals_type(inputToken,QUIT))
   {
      /* check first Token on Line of input */
      if(token_equals_type(inputToken,HELP))
      {
         printCommands();
         TokenReader_clear_to_eoln(tr);
      }
      else if(token_equals_type(inputToken,ERROR))
      {
         printf ("Invalid Input - For a list of valid commands, type ?\n");
         TokenReader_clear_to_eoln(tr);
      }
      else if(token_equals_type(inputToken,EOLN))
      {
         printf ("Blank Line - Do Nothing\n");
         /* blank line - do nothing */
      }
      else
      {
         processExpression(inputToken, tr);
      }

      printf ("\nEnter Expression: ");
      inputToken = TokenReader_get_next_token(tr);
   }

   printf ("Quitting Program\n");
   return 1;
}


