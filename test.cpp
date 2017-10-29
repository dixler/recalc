#include <cstdio>
#include "proj5Stack.h"

int main(){
   MyStack *myStack = new MyStack();
   for(int i = 0; i < 10; i++){
      Token *tok = new Token();
      tok->setToValue(i);
      //printf("%d\n", tok->getValue());
      myStack->push(tok);
   }
   for(int i = 0; i < 5; i++){
      Token tok = myStack->top();
      myStack->pop();
   }
   for(int i = 0; i < 10; i++){
      Token *tok = new Token();
      tok->setToValue(40*i);
      myStack->push(tok);
   }
   for(int i = 0; i < 10; i++){
      Token *tok = new Token();
      tok->setToValue(i);
      //printf("%d\n", tok->getValue());
      myStack->push(tok);
   }
   for(int i = 0; i < 35; i++){
      Token tok = myStack->top();
      if(myStack->isEmpty()){
         printf("EMPTY!!!!\n");
      }
      printf("%d\n", tok.getValue());
      myStack->pop();
   }

}
