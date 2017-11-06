#include <stdlib.h>
typedef struct cursor cursor;

extern cursor *cursor_create();

extern void cursor_clear(cursor *cur);

extern void cursor_free(cursor *cur);

extern void cursor_home(cursor *cur);

extern void cursor_end(cursor *cur);

extern void cursor_next(cursor *cur);

extern void cursor_prev(cursor *cur);

extern void cursor_delete(cursor *cur);

extern void cursor_backspace(cursor *cur);

extern void cursor_insert(cursor *cur, char val);

// going to become defunct after implementation of stuff
extern int cursor_get_x(cursor *cur);

extern char *cursor_get_string(cursor *cur);

//extern char *cursor_get_rev_string(cursor *cur);

