#include "js_buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


static const char *Buffer_prototype;


static int parseInt(const char *s, int *value)
{
	int sign=1;
	if ((*s) == '-')
	{
		sign=-1;
		s++;
	}
	if (! isdigit(*s)) return 0;
	
	(*value)=0;
	do
	{
		(*value)=(*value)*10 + (*s++)-'0';
	} while (isdigit(*s));
	
	(*value)*=sign;
	
	return (*s) == 0;
}


static int Buffer_HasProperty(js_State *J, void *p, const char *name)
{
	Buffer_t *buf=(Buffer_t*)p;
	
	int index, isIndex=parseInt(name, &index);
	
	if (isIndex && (index >= 0) && (index < buf->length) )
		js_pushnumber(J, buf->data[index]); else
	if (! strcmp(name, "length"))
		js_pushnumber(J, buf->length); else
		return 0;
	
	return 1;
}


static int Buffer_Put(js_State *J, void *p, const char *name)
{
	Buffer_t *buf=(Buffer_t*)p;
	
	int index, isIndex=parseInt(name, &index);
	
	if (isIndex && (index >= 0) && (index <= buf->length) )
	{
		buf->data[index]=(uint8_t)(js_tointeger(J, -1) & 0xff);
		return 1;
	} else
	{
		return 0;
	}
}


static int Buffer_Delete(js_State *J, void *p, const char *name)
{
	return 0;
}


static void Buffer_Finalize(js_State *J, void *p)
{
	free(p);
}


static void jsB_alloc(js_State *J)
{
	if (! js_isnumber(J, 1)) js_error(J, "bad arguments");
	int len = js_tointeger(J, 1);
	if (len < 0) js_error(J, "negative length");
	Buffer_t *buf = js_new_Buffer(J, len);
	if (! buf) js_error(J, "alloc failed");
}


static void jsB_toString(js_State *J)
{
	Buffer_t *buf=js_touserdata(J, -1, "Buffer");
	if (! buf) js_error(J, "not a Buffer");
	
	js_pushlstring(J, (const char*)buf->data, buf->length);
}


void js_buffer_init(js_State *J)
{
	js_newobject(J);
	
	js_newcfunction(J, jsB_alloc, "alloc", 1);
	js_defproperty(J, -2, "alloc", JS_DONTENUM);
	
	js_newcfunction(J, jsB_toString, "toString", 0);
	js_defproperty(J, -2, "toString", JS_DONTENUM);
	
	js_dup(J);
	Buffer_prototype=js_ref(J);
	
	js_setglobal(J, "Buffer");
}


Buffer_t* js_new_Buffer(js_State *J, int length)
{
	Buffer_t *buf = (Buffer_t*)calloc(1, sizeof(Buffer_t) + length);
	if (! buf) return 0;
	buf->length=length;
	
	js_getregistry(J, Buffer_prototype);
	js_newuserdatax(J, "Buffer", buf, Buffer_HasProperty, Buffer_Put, Buffer_Delete, Buffer_Finalize);
	
	return buf;
}


Buffer_t* js_toBuffer(js_State *J, int idx)
{
	Buffer_t *buf=js_touserdata(J, idx, "Buffer");
	return buf;
}
