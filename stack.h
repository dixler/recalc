/* This file contains the user interface code for the Infix Evaluation Project
 *  Project 5 for CS 211 for Fall 2017
 *
 * Date: 10/21/17
 *
 * Author: Kyle Dixler
 *
 */

#include <stdio.h>
#include <stdlib.h>
//#include "tokens.h"

// Class to hold the Token information
typedef struct stack stack;
/*
   private:
   Token **stack;
   int size;
   int capacity;
   */
extern stack *stk_create();

extern int stk_is_empty(stack *mystack);

extern void stk_push(stack *mystack, void *data);

extern void *stk_top(stack *mystack);

extern void stk_pop(stack *mystack);

extern void stk_reset(stack *mystack);

