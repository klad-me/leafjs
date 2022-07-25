#include "js_path.h"

#include <stdlib.h>


static void jsB_resolve(js_State *J)
{
	if (! js_isstring(J, 1)) js_error(J, "bad arguments");
	const char *filename = js_tostring(J, 1);
	char *result = realpath(filename, NULL);
	if (result)
	{
		js_pushstring(J, result);
		free(result);
	} else
	{
		js_pushundefined(J);
	}
}


void js_path_init(js_State *J)
{
	js_newobject(J);
	
	js_newcfunction(J, jsB_resolve, "resolve", 1);
	js_setproperty(J, -2, "resolve");
	
	js_setglobal(J, "path");
}
