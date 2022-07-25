#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mujs.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "js_process.h"
#include "js_console.h"
#include "js_path.h"
#include "js_buffer.h"
#include "js_global.h"
#include "js_fs.h"


// Embedded init code
static const unsigned char js_init[]={
#include "js_init.h"
	, 0x00	// line end
};


static int runFile(js_State *J, const char *fname)
{
	if (js_try(J))
	{
		js_report(J, js_trystring(J, -1, "Error"));
		js_pop(J, 1);
		return -1;
	}
	js_loadfile(J, fname);
	js_pushundefined(J);
	js_call(J, 0);
	int ret=0;
	if (js_isnumber(J, -1)) ret=js_tonumber(J, -1);
	js_pop(J, 1);
	js_endtry(J);
	
	return ret;
}


static int runScript(js_State *J, const char *script)
{
	if (js_try(J))
	{
		js_report(J, js_trystring(J, -1, "Error"));
		js_pop(J, 1);
		return -1;
	}
	js_loadstring(J, "[commandLine]", script);
	js_pushundefined(J);
	js_call(J, 0);
	int ret=0;
	if (js_isnumber(J, -1)) ret=js_tonumber(J, -1);
	js_pop(J, 1);
	js_endtry(J);
	
	return ret;
}


#define PS1	"> "


static int eval_print(js_State *J, const char *source)
{
	if (js_ploadstring(J, "[stdin]", source))
	{
		fprintf(stderr, "%s\n", js_trystring(J, -1, "Error"));
		js_pop(J, 1);
		return -1;
	}
	
	js_pushundefined(J);
	if (js_pcall(J, 0))
	{
		fprintf(stderr, "%s\n", js_trystring(J, -1, "Error"));
		js_pop(J, 1);
		return -1;
	}
	
	if (js_isdefined(J, -1))
	{
		printf("%s\n", js_tryrepr(J, -1, "can't convert to string"));
	}
	
	int ret=js_tonumber(J, -1);
	
	js_pop(J, 1);
	return ret;
}


int main(int argc, char **argv)
{
	int ret=0;
	
	// Checking command line
	if ( (argc == 2) && (strcmp(argv[1], "--help") == 0) )
	{
		fprintf(stderr, "Usage: %s <script.js> [args]\n", argv[0]);
		fprintf(stderr, "       %s <js code>   [args]\n", argv[0]);
		fprintf(stderr, "       %s             <-- interactive shell\n", argv[0]);
		return -1;
	}
	
	// Creating JavaScript engine
	js_State *J=js_newstate(NULL, NULL, JS_STRICT);
	
	// Initializing library
	js_dostring(J, (const char*)js_init);
	js_process_init(J, argc, argv);
	js_console_init(J);
	js_path_init(J);
	js_buffer_init(J);
	js_global_init(J);
	js_fs_init(J);
	
	
	if (argc < 2)
	{
		// Interactive mode
		if (! isatty(0))
		{
			fprintf(stderr, "Not a terminal\n");
			return -1;
		}
		
		using_history();
		rl_bind_key('\t', rl_insert);
		const char *input;
		while ( (input=readline(PS1)) )
		{
			if (*input)
			{
				ret=eval_print(J, input);
				add_history(input);
			}
			free((void*)input);
		}
		putchar('\n');
	} else
	{
		// File/script
		
		// Checking if script is a file
		FILE *f=fopen(argv[1], "r");
		if (f)
		{
			// It's a file
			fclose(f);
			ret=runFile(J, argv[1]);
		} else
		{
			// It's a script
			ret=runScript(J, argv[1]);
		}
		
		// Deleting JS engine
		js_freestate(J);
	}
	
	return ret;
}
