#include "js_process.h"

#include <stdlib.h>


static void jsB_exit(js_State *J)
{
	int ret=0;
	if (js_isnumber(J, 1))
		ret=js_tonumber(J, 1);
	
	exit(ret);
}


void js_process_init(js_State *J, int argc, char **argv)
{
	js_newobject(J);
	
	// process.argv[]
	js_newarray(J);
	js_pushstring(J, argv[0]);
	js_setindex(J, -2, 0);
	
	if (argc >= 2)
		js_pushstring(J, argv[1]); else
		js_pushstring(J, "[interactive]");
	js_setindex(J, -2, 1);
	
	for (int n=2; n<argc; n++)
	{
		js_pushstring(J, argv[n]);
		js_setindex(J, -2, n);
	}
	js_setproperty(J, -2, "argv");
	
	
	// process.exit()
	js_newcfunction(J, jsB_exit, "exit", 1);
	js_setproperty(J, -2, "exit");
	
	
	js_setglobal(J, "process");
}
