#ifndef __fgl_buffer_h__
#define __fgl_buffer_h__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>

#include <fgl/fgl_color.h>
#include <fgl/fgl_texture.h>

#define FGL_COLOR_BUFFER_BIT 1
#define FGL_DEPTH_BUFFER_BIT 2

extern void         fgl_draw_buffer_set(fgl_texture* inBuffer);
extern fgl_texture* fgl_draw_buffer_get();

extern void         fgl_depth_buffer_set(uint16_t* inBuffer);
extern uint16_t*    fgl_depth_buffer_get();

extern void         fgl_clear_color(fgl_color_t inColor);
extern void         fgl_clear_depth(uint16_t inDepth);

extern void         fgl_clear(uint8_t inMask);

#ifdef __cplusplus
}
#endif

#endif
