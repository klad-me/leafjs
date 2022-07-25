#include "js_global.h"

#include <stdlib.h>


static void jsB_gc(js_State *J)
{
	js_gc(J, 0);
	js_pushundefined(J);
}


void js_global_init(js_State *J)
{
	js_newcfunction(J, jsB_gc, "gc", 0);
	js_setglobal(J, "gc");
}
