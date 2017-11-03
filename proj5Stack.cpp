
/* This file contains the user interface code for the Infix Evaluation Project
 *  Project 5 for CS 211 for Fall 2017
 *
 * Date: 10/21/17
 *
 * Author: Kyle Dixler
 *
 */

#include "proj5Stack.h"

// Class to hold the Token information

MyStack::MyStack(){
   this->capacity = 2;
   this->size = 0;
   this->stack = (Token**)malloc(sizeof(Token*)*2);
}

bool MyStack::isEmpty(){
   return this->size == 0;
}

void MyStack::push(Token *data){
   if(size == capacity){
      this->capacity += 2;
      this->stack = (Token**)realloc(this->stack, sizeof(Token*)*this->capacity);
   }
   this->stack[this->size] = data;
   this->size += 1;
   //printf("\nsize %d, capacity %d\n", this->size, this->capacity);
   return;
}

Token MyStack::top(){
   if(this->size == 0){
      fprintf(stderr, "stack empty\n");
      return (TokenType)-999;
   }
   return *this->stack[this->size-1];
}

void MyStack::pop(){
   //this->stack->pop_back();
   this->size += -1;
   return;
}

void MyStack::reset(){
   this->size = 0;
}

