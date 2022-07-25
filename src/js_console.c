#include "js_console.h"

#include <stdio.h>


static void jsB_log_info(js_State *J)
{
	int i, top = js_gettop(J);
	for (i = 1; i < top; ++i)
	{
		const char *s = js_tostring(J, i);
		fputs(s, stdout);
		if (i < top-1) fputc(' ', stdout);
	}
	js_pushundefined(J);
	fputs("\n", stdout);
	fflush(stdout);
}


static void jsB_warn_error(js_State *J)
{
	int i, top = js_gettop(J);
	for (i = 1; i < top; ++i)
	{
		const char *s = js_tostring(J, i);
		fputs(s, stderr);
		if (i < top-1) fputc(' ', stderr);
	}
	js_pushundefined(J);
	fputs("\n", stderr);
	fflush(stderr);
}


void js_console_init(js_State *J)
{
	js_newobject(J);
	
	// console.log()
	js_newcfunction(J, jsB_log_info, "log", 1);
	js_setproperty(J, -2, "log");
	
	// console.info()
	js_newcfunction(J, jsB_log_info, "info", 1);
	js_setproperty(J, -2, "info");
	
	// console.warn()
	js_newcfunction(J, jsB_warn_error, "warn", 1);
	js_setproperty(J, -2, "warn");
	
	// console.error()
	js_newcfunction(J, jsB_warn_error, "error", 1);
	js_setproperty(J, -2, "error");
	
	
	js_setglobal(J, "console");
}
