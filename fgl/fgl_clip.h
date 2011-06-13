#ifndef __fgl_clip_h__
#define __fgl_clip_h__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

#include <fgl/fgl_vertex.h>

extern int  fgl_clip_point(fgl_vertex_t* inV, fgl_vertex_t* outV);
extern int  fgl_clip_line(fgl_vertex_t* inV0, fgl_vertex_t* inV1, fgl_vertex_t* outV);
extern int  fgl_clip_triangle(fgl_vertex_t* inV, fgl_vertex_t* outV);
extern int  fgl_clip_sprite(fgl_vertex_t* inV, fgl_vertex_t* outV);

extern void fgl_clip_line_x(fgl_vertex_t* inV0, fgl_vertex_t* inV1, fgl_vertex_t* outV);
extern void fgl_clip_line_y(fgl_vertex_t* inV0, fgl_vertex_t* inV1, fgl_vertex_t* outV);
extern void fgl_clip_line_z(fgl_vertex_t* inV0, fgl_vertex_t* inV1, fgl_vertex_t* outV);

#ifdef __cplusplus
}
#endif

#endif
