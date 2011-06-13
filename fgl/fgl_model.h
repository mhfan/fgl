#ifndef __fgl_model_h__
#define __fgl_model_h__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>

#include <libfixmath/fixmath.h>

#include <fgl/fgl_vertex.h>

typedef struct {
	uintptr_t     frame_count;
	uintptr_t     vert_count;
	fgl_vertex_t* vert_data;
} fgl_model;

extern fgl_model* fgl_model_load_md2(const char* inPath);
extern void       fgl_model_delete(fgl_model* inModel);

extern void       fgl_model_draw(fgl_model* inModel, fix16_t inFrame);

#ifdef __cplusplus
}
#endif

#endif
