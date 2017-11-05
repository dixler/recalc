
/* This file contains the user interface code for the Infix Evaluation Project
 *  Project 5 for CS 211 for Fall 2017
 *
 * Date: 10/21/17
 *
 * Author: Kyle Dixler
 *
 */

#include "stack.h"
//#include "tokens.h"


// Class to hold the Token information
typedef struct stack{
   void **stack;
   int size;
   int capacity;

}stack;

stack *stk_create(){
   stack *this = (stack*)malloc(sizeof(stack));
   this->capacity = 2;
   this->size = 0;
   this->stack = (void**)malloc(sizeof(void*)*2);
   return this;
}

int stk_is_empty(stack *this){
   return this->size == 0;
}

void stk_push(stack *this, void *data){
   if(this->size == this->capacity){
      this->capacity *= 2;
      this->stack = (void**)realloc(this->stack, sizeof(void*)*this->capacity);
   }
   this->stack[this->size] = data;
   this->size += 1;
   //printf("\nsize %d, capacity %d\n", this->size, this->capacity);
   return;
}

void *stk_top(stack *this){
   if(this->size == 0){
      fprintf(stderr, "stack empty\n");
      return NULL;
   }
   return this->stack[this->size-1];
}

void stk_pop(stack *this){
   //this->stack->pop_back();
   this->size += -1;
   return;
}

void stk_reset(stack *this){
   this->size = 0;
   return;
}

