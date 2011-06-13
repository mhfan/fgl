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

#define _func_name render_triangle_cs
#define _func_name_sprite render_sprite_cs
#define _func_name_raster raster_triangle_cs
#define _polygon_color
#include "_render.h"

#define _func_name render_triangle_ci
#define _func_name_raster raster_triangle_ci
#define _vertex_color
#include "_render.h"

#define _func_name render_triangle_tn
#define _func_name_sprite render_sprite_tn
#define _func_name_raster raster_triangle_tn
#define _texture
#include "_render.h"

#define _func_name render_triangle_cs_tn
#define _func_name_sprite render_sprite_cs_tn
#define _func_name_raster raster_triangle_cs_tn
#define _polygon_color
#define _texture
#include "_render.h"

#define _func_name render_triangle_ci_tn
#define _func_name_raster raster_triangle_ci_tn
#define _vertex_color
#define _texture
#include "_render.h"

#define _func_name render_triangle_cl_cs
#define _func_name_raster raster_triangle_cl_cs
#define _polygon_color
#define _clip
#include "_render.h"

#define _func_name render_triangle_cl_ci
#define _func_name_raster raster_triangle_cl_ci
#define _vertex_color
#define _clip
#include "_render.h"

#define _func_name render_triangle_cl_tn
#define _func_name_raster raster_triangle_cl_tn
#define _texture
#define _clip
#include "_render.h"

#define _func_name render_triangle_cl_cs_tn
#define _func_name_raster raster_triangle_cl_cs_tn
#define _polygon_color
#define _texture
#define _clip
#include "_render.h"

#define _func_name render_triangle_cl_ci_tn
#define _func_name_raster raster_triangle_cl_ci_tn
#define _vertex_color
#define _texture
#define _clip
#include "_render.h"



#define _func_name render_triangle_dw_dt_cs
#define _func_name_sprite render_sprite_dw_dt_cs
#define _func_name_raster raster_triangle_dw_dt_cs
#define _depth_write
#define _depth_test
#define _polygon_color
#include "_render.h"

#define _func_name render_triangle_dw_dt_ci
#define _func_name_raster raster_triangle_dw_dt_ci
#define _depth_write
#define _depth_test
#define _vertex_color
#include "_render.h"

#define _func_name render_triangle_dw_dt_tn
#define _func_name_sprite render_sprite_dw_dt_tn
#define _func_name_raster raster_triangle_dw_dt_tn
#define _depth_write
#define _depth_test
#define _texture
#include "_render.h"

#define _func_name render_triangle_dw_dt_cs_tn
#define _func_name_sprite render_sprite_dw_dt_cs_tn
#define _func_name_raster raster_triangle_dw_dt_cs_tn
#define _depth_write
#define _depth_test
#define _polygon_color
#define _texture
#include "_render.h"

#define _func_name render_triangle_dw_dt_ci_tn
#define _func_name_raster raster_triangle_dw_dt_ci_tn
#define _depth_write
#define _depth_test
#define _vertex_color
#define _texture
#include "_render.h"

#define _func_name render_triangle_dw_dt_cl_cs
#define _func_name_raster raster_triangle_dw_dt_cl_cs
#define _depth_write
#define _depth_test
#define _polygon_color
#define _clip
#include "_render.h"

#define _func_name render_triangle_dw_dt_cl_ci
#define _func_name_raster raster_triangle_dw_dt_cl_ci
#define _depth_write
#define _depth_test
#define _vertex_color
#define _clip
#include "_render.h"

#define _func_name render_triangle_dw_dt_cl_tn
#define _func_name_raster raster_triangle_dw_dt_cl_tn
#define _depth_write
#define _depth_test
#define _texture
#define _clip
#include "_render.h"

#define _func_name render_triangle_dw_dt_cl_cs_tn
#define _func_name_raster raster_triangle_dw_dt_cl_cs_tn
#define _depth_write
#define _depth_test
#define _polygon_color
#define _texture
#define _clip
#include "_render.h"

#define _func_name render_triangle_dw_dt_cl_ci_tn
#define _func_name_raster raster_triangle_dw_dt_cl_ci_tn
#define _depth_write
#define _depth_test
#define _vertex_color
#define _texture
#define _clip
#include "_render.h"
