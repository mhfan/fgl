#ifndef __fgl_transform_h__
#define __fgl_transform_h__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>

#include <libfixmath/fixmath.h>

#include <fgl/fgl_matrix.h>
#include <fgl/fgl_vertex.h>

extern fgl_vertex_t fgl_transform_vertex(fgl_vertex_t inVertex, fix16_t* inMatrix);

extern void fgl_translate(fix16_t inX, fix16_t inY, fix16_t inZ);
extern void fgl_scale(fix16_t inX, fix16_t inY, fix16_t inZ);
extern void fgl_rotate_x(fix16_t inAngle);
extern void fgl_rotate_y(fix16_t inAngle);
extern void fgl_rotate_z(fix16_t inAngle);

extern void fgl_ortho(fix16_t inLeft, fix16_t inRight, fix16_t inTop, fix16_t inBottom, fix16_t inNear, fix16_t inFar);
extern void fgl_ortho_2d(fix16_t inLeft, fix16_t inRight, fix16_t inTop, fix16_t inBottom);
extern void fgl_frustum(fix16_t inLeft, fix16_t inRight, fix16_t inTop, fix16_t inBottom, fix16_t inNear, fix16_t inFar);
extern void fgl_perspective(fix16_t inFovY, fix16_t inAspect, fix16_t inNear, fix16_t inFar);
extern void fgl_viewport(fix16_t inX, fix16_t inY, fix16_t inWidth, fix16_t inHeight);

#ifdef __cplusplus
}
#endif

#endif
