#include "fgl_buffer.h"
#include <stdlib.h>

fgl_texture* _fgl_draw_buffer  = NULL;
uint16_t*    _fgl_depth_buffer = NULL;
fgl_color_t  _fgl_clear_color  = 0x0000;
uint16_t     _fgl_clear_depth  = 0xFFFF;



void fgl_draw_buffer_set(fgl_texture* inBuffer) {
	_fgl_draw_buffer = inBuffer;
}

fgl_texture* fgl_draw_buffer_get() {
	return _fgl_draw_buffer;
}



void fgl_depth_buffer_set(uint16_t* inBuffer) {
	_fgl_depth_buffer = inBuffer;
}

uint16_t* fgl_depth_buffer_get() {
	return _fgl_depth_buffer;
}



void fgl_clear_color(fgl_color_t inColor) {
	_fgl_clear_color = inColor;
}

void fgl_clear_depth(uint16_t inDepth) {
	_fgl_clear_depth = inDepth;
}



void fgl_clear(uint8_t inMask) {
	if((inMask & FGL_COLOR_BUFFER_BIT) && (_fgl_draw_buffer != NULL)) {
		fgl_color_t* tempCPtr = _fgl_draw_buffer->data;
		fgl_color_t* tempCEnd = &tempCPtr[_fgl_draw_buffer->width * _fgl_draw_buffer->height];
		while(tempCPtr < tempCEnd)
			*(tempCPtr++) = _fgl_clear_color;
	}
	if((inMask & FGL_DEPTH_BUFFER_BIT) && (_fgl_depth_buffer != NULL)) {
		uint16_t* tempDPtr = _fgl_depth_buffer;
		uint16_t* tempDEnd = &tempDPtr[_fgl_draw_buffer->width * _fgl_draw_buffer->height];
		while(tempDPtr < tempDEnd)
			*(tempDPtr++) = _fgl_clear_depth;
	}
}
