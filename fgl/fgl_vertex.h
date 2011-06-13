#ifndef __fgl_vertex_h__
#define __fgl_vertex_h__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>

#include <libfixmath/fixmath.h>

#include <fgl/fgl_color.h>

typedef struct {
	fix16_t x, y, z;
	fix16_t u, v;
	fgl_color_t c;
} fgl_vertex_t;

extern void fgl_vertex_print(fgl_vertex_t inVertex);

#ifdef __cplusplus
}
#endif

#endif
