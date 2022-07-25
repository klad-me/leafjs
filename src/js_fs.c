#include "js_fs.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "js_buffer.h"


static void jsB_readFileSync(js_State *J)
{
	const char *filename = js_tostring(J, 1);
	Buffer_t *buf=0;
	
	FILE *f=fopen(filename, "rb");
	if (! f) goto error;
	if (fseek(f, 0, SEEK_END) < 0) goto error;
	int len=ftell(f);
	if (len < 0) goto error;
	if (fseek(f, 0, SEEK_SET) < 0) goto error;
	
	buf = js_new_Buffer(J, len);
	if (! buf)
	{
		fclose(f);
		js_error(J, "alloc failed");
	}
	
	int rd = fread(buf->data, 1, len, f);
	if (rd != len) goto error;
	
	return;
	
error:
	if (buf) js_pop(J, 1);
	if (f) fclose(f);
	js_error(J, "%s: %s", filename, strerror(errno));
}


static void jsB_writeFileSync(js_State *J)
{
	const char *filename = js_tostring(J, 1);
	Buffer_t *buf=js_toBuffer(J, 2);;
	
	if (! buf) js_error(J, "bad arguments");
	
	FILE *f=fopen(filename, "wb");
	if (! f) goto error;
	
	if (fwrite(buf->data, 1, buf->length, f) != buf->length) goto error;
	
	fclose(f);
	js_pushundefined(J);
	return;
	
error:
	if (f) fclose(f);
	js_error(J, "%s: %s", filename, strerror(errno));
}


void js_fs_init(js_State *J)
{
	js_newobject(J);
	
	js_newcfunction(J, jsB_readFileSync, "readFileSync", 1);
	js_setproperty(J, -2, "readFileSync");
	
	js_newcfunction(J, jsB_writeFileSync, "writeFileSync", 2);
	js_setproperty(J, -2, "writeFileSync");
	
	js_setglobal(J, "fs");
}
