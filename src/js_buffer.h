#ifndef JS_BUFFER_H
#define JS_BUFFER_H


#include <mujs.h>
#include <stdint.h>


typedef struct
{
	int length;
	uint8_t data[0];
} Buffer_t;


void js_buffer_init(js_State *J);
Buffer_t* js_new_Buffer(js_State *J, int length);
Buffer_t* js_toBuffer(js_State *J, int idx);


#endif
