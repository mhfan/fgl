#ifndef __render_h__
#define __render_h__

#include <stdbool.h>
#include <stdint.h>

#include <fgl/fgl_color.h>
#include <fgl/fgl_vertex.h>
#include <fgl/fgl_texture.h>

extern void render_triangle_cs(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_vertex_t inV2, fgl_color_t inColor);
extern void render_triangle_ci(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_vertex_t inV2);
extern void render_triangle_tn(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_vertex_t inV2, fgl_texture* inTexture);
extern void render_triangle_cs_tn(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_vertex_t inV2, fgl_texture* inTexture, fgl_color_t inColor);
extern void render_triangle_ci_tn(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_vertex_t inV2, fgl_texture* inTexture);

extern void render_triangle_cl_cs(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_vertex_t inV2, fgl_color_t inColor);
extern void render_triangle_cl_ci(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_vertex_t inV2);
extern void render_triangle_cl_tn(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_vertex_t inV2, fgl_texture* inTexture);
extern void render_triangle_cl_cs_tn(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_vertex_t inV2, fgl_texture* inTexture, fgl_color_t inColor);
extern void render_triangle_cl_ci_tn(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_vertex_t inV2, fgl_texture* inTexture);

extern void render_triangle_dw_dt_cs(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_vertex_t inV2, fgl_color_t inColor);
extern void render_triangle_dw_dt_ci(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_vertex_t inV2);
extern void render_triangle_dw_dt_tn(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_vertex_t inV2, fgl_texture* inTexture);
extern void render_triangle_dw_dt_cs_tn(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_vertex_t inV2, fgl_texture* inTexture, fgl_color_t inColor);
extern void render_triangle_dw_dt_ci_tn(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_vertex_t inV2, fgl_texture* inTexture);

extern void render_triangle_dw_dt_cl_cs(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_vertex_t inV2, fgl_color_t inColor);
extern void render_triangle_dw_dt_cl_ci(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_vertex_t inV2);
extern void render_triangle_dw_dt_cl_tn(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_vertex_t inV2, fgl_texture* inTexture);
extern void render_triangle_dw_dt_cl_cs_tn(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_vertex_t inV2, fgl_texture* inTexture, fgl_color_t inColor);
extern void render_triangle_dw_dt_cl_ci_tn(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_vertex_t inV2, fgl_texture* inTexture);

extern void render_sprite_cs(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_color_t inColor);
extern void render_sprite_ci(fgl_vertex_t inV0, fgl_vertex_t inV1);
extern void render_sprite_tn(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_texture* inTexture);
extern void render_sprite_cs_tn(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_texture* inTexture, fgl_color_t inColor);
extern void render_sprite_ci_tn(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_texture* inTexture);

extern void render_sprite_dw_dt_cs(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_color_t inColor);
extern void render_sprite_dw_dt_ci(fgl_vertex_t inV0, fgl_vertex_t inV1);
extern void render_sprite_dw_dt_tn(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_texture* inTexture);
extern void render_sprite_dw_dt_cs_tn(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_texture* inTexture, fgl_color_t inColor);
extern void render_sprite_dw_dt_ci_tn(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_texture* inTexture);

#endif
