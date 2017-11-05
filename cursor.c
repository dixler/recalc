#include "cursor.h"

typedef struct node node;

typedef struct node{
   char data;
   node* next;
   node* prev;
}node;

typedef struct cursor{
   node *home;
   node *cur;
   node *end;
   int pos;
   int len;
}cursor;

cursor *cursor_create(){
   // we always have an end cursor
   node *sym = (node*)malloc(sizeof(node));
   sym->data = '\0';
   sym->next = NULL;
   sym->prev = NULL;

   cursor *this = (cursor*)malloc(sizeof(cursor));
   this->home = this->cur = this->end = sym;
   this->len = this->pos = 0;
   return this;
}

// TODO
void cursor_clear(cursor *this){
   node *freed = this->home;
   while(freed != NULL){
      node *temp = freed;
      freed = freed->next;
      free(temp);
   }
   return;
}

void cursor_free(cursor *this){
   cursor_clear(this);
   free(this);
   return;
}

void cursor_home(cursor *this){
   this->cur = this->home;
   this->pos = 0;
   return;
}

void cursor_end(cursor *this){
   this->cur = this->end;
   this->pos = this->len;
   return;
}

void cursor_next(cursor *this){
   if(this->cur->next != NULL){
      this->cur = this->cur->next;
      this->pos += 1;
   }
   return;
}

void cursor_prev(cursor *this){
   if(this->cur->prev != NULL){
      this->cur = this->cur->prev;
      this->pos -= 1;
   }
   return;
}


// TODO
// delete the current cursor node and
// set the next node as the cursor
void cursor_delete(cursor *this){
   if(this->cur != this->home){
      // if the cursor is not on the home 
      // cursor, there is something to delete
      
      // we need to delete the previous letter
   }
   return;
}

// TODO
// delete the previous cursor node 
void cursor_backspace(cursor *this){
   // if the cursor is not on the home 
   if(this->cur != this->home){
      // there is a node before cur
      if(this->cur->prev == this->home){
         // if the previous node is the home
         // we're about to delete the home node and 
         // the cur will be the home
         free(this->home);
         this->home = this->cur;
         this->cur->prev = NULL;
      }
      else{
         // otherwise, we're in the middle of the list
         // and have a lot of bookkeeping to do
         node *freed = this->cur->prev;
         this->cur->prev->prev->next = this->cur;
         this->cur->prev = this->cur->prev->prev;
         free(freed);
      }
      this->pos -= 1;
      this->len -= 1;
   }
   return;
}


/* the cursor is never empty and always contains a 
 * NULL terminus insert(like the name implies) puts 
 * the node before the current node 
 *
 * the cursor will remain at its current position however
 */

void cursor_insert(cursor *this, char val){

   node *inserted = (node*)malloc(sizeof(node));
   inserted->data = val;
   // make the INSERTED node's next be the current node
   inserted->next = this->cur;

   this->pos += 1;
   this->len += 1;

   if(this->cur->prev != NULL){
      // make the INSERTED node's prev be the current node's
      // previous
      inserted->prev = this->cur->prev;
      // make the previous node point to the INSERTED node
      this->cur->prev->next = inserted;
   }
   else{
      // otherwise we just overwrote the front node and we need to update it
      this->home = inserted;
      inserted->prev = NULL;
   }
   // make the current node's previous be the inserted node
   this->cur->prev = inserted;
   return;

}

int cursor_get_x(cursor *this){
   return this->pos;
}

char *cursor_get_string(cursor *this){
   node *cur_node = this->home;
   char *string = (char*)malloc(sizeof(char)*this->len);
   for(int index = 0; index < this->len+1; index++){
      string[index] = cur_node->data;
      cur_node = cur_node->next;
   }
   return string;
}
