/* This file contains the user interface code for the Infix Evaluation Project
 *  Project 5 for CS 211 for Fall 2017
 *
 * Date: 10/21/17
 *
 * Author: Kyle Dixler
 *
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <vector>
#include "proj5Tokens.h"

// Class to hold the Token information
class MyStack
{
   private:
      Token **stack;
      int size;
      int capacity;

   public:

      MyStack();

      bool isEmpty();

      void push(Token *data);

      Token top();

      void pop();

      void reset();

};
