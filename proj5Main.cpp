/* This file contains the user interface code for the Infix Evaluation Project
 *  Project 5 for CS 211 for Fall 2017
 *
 * Date: 10/21/17
 *
 * Author: Pat Troy
 *
 */

//#include "proj5Tokens.h"
#define DEBUGMODE if(debugMode)
#include "proj5Stack.h"

bool debugMode = false;

void printCommands();

int eval (Token val1, Token op, Token val2){
   int result = -999;
   switch(op.getOperator()){
      case '*':
         // multiply the arguments
         result = (val1.getValue() * val2.getValue());
         break;
      case '/':
         result = (val1.getValue() / val2.getValue());
         break;
      case '-':
         result = (val1.getValue() - val2.getValue());
         break;
      case '+':
         result = (val1.getValue() + val2.getValue());
         break;
      default:
         fprintf(stderr, "passed illegal operator \'%c\'\n", op.getOperator());
   }
   return result;
}

void popAndEval(MyStack *ValueStack, MyStack *OperatorStack){
   Token op = OperatorStack->top ();
   OperatorStack->pop ();
   Token v2 = ValueStack->top ();
   ValueStack->pop ();
   Token v1 = ValueStack->top ();
   ValueStack->pop ();
   Token *v3 = new Token(VALUE);
   v3->setToValue(eval ( v1, op, v2 ));
   //printf("evaluating: %d %c %d = %d\n",  v1.getValue(), op.getOperator(), v2.getValue(), v3->getValue());


   ValueStack->push (v3);
}

void processExpression (Token inputToken, TokenReader *tr);

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
            debugMode = true;
            printf (" Debugging Information \n"); 
         }
      }
   }



   Token inputToken;
   TokenReader tr;

   printf ("Starting Expression Evaluation Program\n\n");
   printf ("Enter Expression: ");


   inputToken = tr.getNextToken ();

   while (inputToken.equalsType(QUIT) == false)
   {
      /* check first Token on Line of input */
      if(inputToken.equalsType(HELP))
      {
         printCommands();
         tr.clearToEoln();
      }
      else if(inputToken.equalsType(ERROR))
      {
         printf ("Invalid Input - For a list of valid commands, type ?\n");
         tr.clearToEoln();
      }
      else if(inputToken.equalsType(EOLN))
      {
         printf ("Blank Line - Do Nothing\n");
         /* blank line - do nothing */
      }
      else
      {
         processExpression(inputToken, &tr);
      }

      printf ("\nEnter Expression: ");
      inputToken = tr.getNextToken ();
   }

   printf ("Quitting Program\n");
   return 1;
}

void printCommands()
{
   printf ("The commands for this program are:\n\n");
   printf ("q - to quit the program\n");
   printf ("? - to list the accepted commands\n");
   printf ("or any infix mathematical expression using operators of (), *, /, +, -\n");
}

void processExpression (Token inputToken, TokenReader *tr)
{
   /**********************************************/
   /* Declare both stack head pointers here      */
   MyStack *ValueStack = new MyStack();
   MyStack *OperatorStack = new MyStack();

   /* Loop until the expression reaches its End */
   while (inputToken.equalsType(EOLN) == false)
   {
      /* The expression contain a VALUE */
      if (inputToken.equalsType(VALUE))
      {
         /* make this a debugMode statement */
         DEBUGMODE printf ("Val: %d, ", inputToken.getValue() );

         // add code to perform this operation here
         Token *tok = new Token(VALUE);
         tok->setToValue(inputToken.getValue());

         ValueStack->push(tok);
      }

      /* The expression contains an OPERATOR */
      else if (inputToken.equalsType(OPERATOR))
      {
         /* make this a debugMode statement */
         DEBUGMODE printf ("OP: %c, ", inputToken.getOperator() );

         // add code to perform this operation here
         Token *optok = new Token(OPERATOR);
         optok->setToOperator(inputToken.getOperator());

         if(inputToken.getOperator() == '('){     
            // if ( the current operator is an Open Parenthesis ){
            OperatorStack->push(optok);            
            // push the Open Parenthesis onto the OperatorStack 
         }
         else if(inputToken.getOperator() == '+' || inputToken.getOperator() == '-'){     
            // if ( the current operator is + or - ){
            while(!OperatorStack->isEmpty() && (
                        OperatorStack->top().getOperator() == '+' || OperatorStack->top().getOperator() == '-'
                     || OperatorStack->top().getOperator() == '*' || OperatorStack->top().getOperator() == '/')){      

               // while ( the OperatorStack is not Empty && the top of the OperatorStack is +, -, * or / ){
                  popAndEval(ValueStack, OperatorStack);   
                  // popAndEval ( )
               }
               OperatorStack->push(optok);              
               // push the current operator on the OperatorStack
            }
         else if(inputToken.getOperator() == '*' || inputToken.getOperator() == '/'){   
            // if ( the current operator is * or / )
            while(!OperatorStack->isEmpty() && 
                  (OperatorStack->top().getOperator() == '*' || OperatorStack->top().getOperator() == '/')){  
               // while ( the OperatorStack is not Empty && the top of the OperatorStack is * or / )
               popAndEval (ValueStack, OperatorStack);
            }
            OperatorStack->push(optok);              
            // push the current operator on the OperatorStack
         }
         else if(inputToken.getOperator() == ')'){     
            // if ( the current operator is a Closing Parenthesis )
            while(!OperatorStack->isEmpty() && OperatorStack->top().getOperator() != '('){  
               // while ( the Operator Stack is not Empty && the top of the OperatorStack is not an Open Parenthesis ){
               popAndEval (ValueStack, OperatorStack);
            }
               if(OperatorStack->isEmpty()){   
                  // if (the OperatorStack is Empty )
                  fprintf(stderr, "error OperatorStack is empty\n"); 
                  // print an error message
               }
               else{
                  OperatorStack->pop();  
                  // pop the Open Parenthesis from the OperatorStack
               }
         }
      }
      else{
         printf("Illegal operator\n");
         Token *tok = new Token(VALUE);
         tok->setToValue(-999);
         ValueStack->push(tok); 
         // handle input cleanup
         while (inputToken.equalsType(EOLN) == false)
            inputToken = tr->getNextToken ();
         return;
      }
         /* get next token from input */
         inputToken = tr->getNextToken ();
   }

   /* The expression has reached its end */
   
   while(!OperatorStack->isEmpty()){   
      popAndEval (ValueStack, OperatorStack);
   }
   printf("evaluated to: %d\n", ValueStack->top().getValue());
   OperatorStack->reset();
   ValueStack->reset();

   printf ("\n");
}

