#ifndef __fgl_draw_line_h__
#define __fgl_draw_line_h__

#include <stdbool.h>
#include <stdint.h>

#include <fgl/fgl_color.h>
#include <fgl/fgl_vertex.h>
#include <fgl/fgl_texture.h>

extern void fgl_draw_line_cs(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_color_t inColor);
extern void fgl_draw_line_ci(fgl_vertex_t inV0, fgl_vertex_t inV1);
extern void fgl_draw_line_tn(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_texture* inTexture);
extern void fgl_draw_line_cs_tn(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_texture* inTexture, fgl_color_t inColor);
extern void fgl_draw_line_ci_tn(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_texture* inTexture);

extern void fgl_draw_line_dw_dt_cs(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_color_t inColor);
extern void fgl_draw_line_dw_dt_ci(fgl_vertex_t inV0, fgl_vertex_t inV1);
extern void fgl_draw_line_dw_dt_tn(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_texture* inTexture);
extern void fgl_draw_line_dw_dt_cs_tn(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_texture* inTexture, fgl_color_t inColor);
extern void fgl_draw_line_dw_dt_ci_tn(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_texture* inTexture);

#endif
