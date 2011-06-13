#ifndef __fgl_matrix_h__
#define __fgl_matrix_h__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <libfixmath/fixmath.h>

#define FGL_MATRIX_MODEL      0
#define FGL_MATRIX_VIEW       1
#define FGL_MATRIX_PROJECTION 2
#define FGL_MATRIX_VIEWPORT   3
#define FGL_MATRIX_TEXTURE    4
#define FGL_MATRIX_COLOR      5
#define FGL_MATRIX_LAST FGL_MATRIX_COLOR

extern void    fgl_matrix_print(fix16_t* inMatrix);
extern void    fgl_matrix_copy(fix16_t* outMatrix, fix16_t* inMatrix);
extern void    fgl_matrix_transpose(fix16_t* outMatrix, fix16_t* inMatrix);
extern void    fgl_matrix_multiply(fix16_t* outMatrix, fix16_t* inMatrix0, fix16_t* inMatrix1);

extern void    fgl_matrix_mode_set(uint8_t inMode);
extern uint8_t fgl_matrix_mode_get();
extern void    fgl_matrix_identity();
extern void    fgl_matrix_push();
extern void    fgl_matrix_pop();

extern void    fgl_matrix_set(fix16_t* inMatrix);
extern void    fgl_matrix_get(fix16_t* outMatrix);

extern void    fgl_matrix_mult(fix16_t* inMatrix);

#ifdef __cplusplus
}
#endif

#endif
