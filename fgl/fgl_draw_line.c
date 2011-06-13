#include "include/render.h"
#include <stdlib.h>

extern fix16_t _fgl_clip_width;
extern fix16_t _fgl_clip_height;

extern fgl_texture* _fgl_draw_buffer;
extern uint16_t*    _fgl_depth_buffer;

// cs - Color Solid
// ci - Color Interpolate
// tn - Texture Nearest
// dw - Depth Write
// dt - Depth Test
// cl - Clip

#define _fgl_draw_line fgl_draw_line_cs
#define _polygon_color
#include "_fgl_draw_line.h"

#define _fgl_draw_line fgl_draw_line_ci
#define _vertex_color
#include "_fgl_draw_line.h"

#define _fgl_draw_line fgl_draw_line_tn
#define _texture
#include "_fgl_draw_line.h"

#define _fgl_draw_line fgl_draw_line_cs_tn
#define _polygon_color
#define _texture
#include "_fgl_draw_line.h"

#define _fgl_draw_line fgl_draw_line_ci_tn
#define _vertex_color
#define _texture
#include "_fgl_draw_line.h"



#define _fgl_draw_line fgl_draw_line_dw_dt_cs
#define _depth_write
#define _depth_test
#define _polygon_color
#include "_fgl_draw_line.h"

#define _fgl_draw_line fgl_draw_line_dw_dt_ci
#define _depth_write
#define _depth_test
#define _vertex_color
#include "_fgl_draw_line.h"

#define _fgl_draw_line fgl_draw_line_dw_dt_tn
#define _depth_write
#define _depth_test
#define _texture
#include "_fgl_draw_line.h"

#define _fgl_draw_line fgl_draw_line_dw_dt_cs_tn
#define _depth_write
#define _depth_test
#define _polygon_color
#define _texture
#include "_fgl_draw_line.h"

#define _fgl_draw_line fgl_draw_line_dw_dt_ci_tn
#define _depth_write
#define _depth_test
#define _vertex_color
#define _texture
#include "_fgl_draw_line.h"
