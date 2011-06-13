#include "fgl_draw.h"
#include <stdlib.h>
#include <stdio.h>

#include "render.h"
#include "fgl_draw_line.h"
#include "fgl_color.h"
#include "fgl_texture.h"
#include "fgl_matrix.h"
#include "fgl_transform.h"
#include "fgl_clip.h"

extern fgl_color_t  _fgl_color;
extern fgl_texture* _fgl_texture;
extern bool         _fgl_texture_enable;
extern fix16_t      _fgl_matrix_transform[16];
extern void         _fgl_matrix_transform_update();
extern bool         _fgl_transform_enable;
extern bool         _fgl_clip_enable;
extern bool         _fgl_depth_test_enable;
extern bool         _fgl_depth_write_enable;

void _fgl_draw_array_decode_line_strip(fgl_vertex_t* inVerts, uintptr_t inVertCount, fgl_vertex_t* outVerts) {
	uintptr_t i, j;
	outVerts[0] = inVerts[0];
	outVerts[1] = inVerts[1];
	for(i = 1, j = 0; i < inVertCount; i++, j += 2) {
		outVerts[j + 0] = inVerts[i - 1];
		outVerts[j + 1] = inVerts[i - 0];
	}
}

void _fgl_draw_array_decode_triangle_strip(fgl_vertex_t* inVerts, uintptr_t inVertCount, fgl_vertex_t* outVerts) {
	uintptr_t i, j;
	outVerts[0] = inVerts[0];
	outVerts[1] = inVerts[1];
	for(i = 2, j = 0; i < inVertCount; i++, j += 3) {
		outVerts[j + 0] = inVerts[i - 2];
		outVerts[j + 1] = inVerts[i - 1];
		outVerts[j + 2] = inVerts[i - 0];
		i++, j+=3; if(i >= inVertCount) break;
		outVerts[j + 0] = inVerts[i - 0];
		outVerts[j + 1] = inVerts[i - 1];
		outVerts[j + 2] = inVerts[i - 2];
	}
}

void _fgl_draw_array_decode_triangle_fan(fgl_vertex_t* inVerts, uintptr_t inVertCount, fgl_vertex_t* outVerts) {
	uintptr_t i, j;
	outVerts[0] = inVerts[0];
	outVerts[1] = inVerts[1];
	for(i = 2, j = 0; i < inVertCount; i++, j += 3) {
		outVerts[j + 0] = inVerts[0];
		outVerts[j + 1] = inVerts[i - 1];
		outVerts[j + 2] = inVerts[i];
	}
}

void fgl_draw_array(uint8_t inType, fgl_vertex_t* inVerts, uintptr_t inVertCount) {
	// Validate incoming vertex array.
	switch(inType) {
		case FGL_POINTS:
			if(inVertCount < 1)
				return;
			break;
		case FGL_LINES:
		case FGL_LINE_STRIP:
		case FGL_LINE_LOOP:
		case FGL_SPRITES:
			if(inVertCount < 2)
				return;
			break;
		case FGL_TRIANGLES:
		case FGL_TRIANGLE_STRIP:
		case FGL_TRIANGLE_FAN:
			if(inVertCount < 3)
				return;
			break;
		default:
			return;
	}

	// Chop off extra verteces.
	if((inType == FGL_LINES) || (inType == FGL_SPRITES))
		inVertCount -= (inVertCount & 1);
	else if(inType == FGL_TRIANGLES)
		inVertCount -= (inVertCount % 3);

	// Vertex transformation.
	uintptr_t i, j;
	fgl_vertex_t* tempTransformArray;
	uintptr_t     tempTransformCount;
	if(_fgl_transform_enable) {
		tempTransformCount = inVertCount;
		tempTransformArray = (fgl_vertex_t*)malloc(tempTransformCount * sizeof(fgl_vertex_t));
		if(tempTransformArray == NULL) {
			printf("FGL_ERROR: Unable to transform verteces, out of memory"); // TODO - Proper error.
			return;
		}
		_fgl_matrix_transform_update();
		for(i = 0; i < inVertCount; i++)
			tempTransformArray[i] = fgl_transform_vertex(inVerts[i], _fgl_matrix_transform);
	} else {
		tempTransformArray = inVerts;
		tempTransformCount = inVertCount;
	}



	// Decode incoming vertex array (e.g. triangle strip -> triangles).
	fgl_vertex_t* tempDecodeArray;
	uintptr_t     tempDecodeCount;
	switch(inType) {
			case FGL_LINE_STRIP:
			case FGL_LINE_LOOP:
				tempDecodeCount = (tempTransformCount << 1) - (inType == FGL_LINE_STRIP ? 2 : 0);
				tempDecodeArray = (fgl_vertex_t*)malloc(tempDecodeCount * sizeof(fgl_vertex_t));
				if(tempDecodeArray == NULL) {
					if(tempTransformArray != inVerts)
						free(tempTransformArray);
					printf("FGL_ERROR: Unable to decode array, out of memory."); // TODO - Proper error.
					return;
				}
				_fgl_draw_array_decode_line_strip(tempTransformArray, tempTransformCount, tempDecodeArray);
				if(FGL_LINE_LOOP) {
					tempDecodeArray[tempDecodeCount - 2] = tempTransformArray[tempTransformCount - 1];
					tempDecodeArray[tempDecodeCount - 1] = tempTransformArray[0];
				}
				inType = FGL_LINES;
				break;
			case FGL_TRIANGLE_STRIP:
			case FGL_TRIANGLE_FAN:
				tempDecodeCount = ((tempTransformCount - 2) * 3);
				tempDecodeArray = (fgl_vertex_t*)malloc(tempDecodeCount * sizeof(fgl_vertex_t));
				if(tempDecodeArray == NULL) {
					if(tempTransformArray != inVerts)
						free(tempTransformArray);
					printf("FGL_ERROR: Unable to decode array, out of memory."); // TODO - Proper error.
					return;
				}
				if(inType == FGL_TRIANGLE_STRIP)
					_fgl_draw_array_decode_triangle_strip(tempTransformArray, tempTransformCount, tempDecodeArray);
				else
					_fgl_draw_array_decode_triangle_fan(tempTransformArray, tempTransformCount, tempDecodeArray);
				inType = FGL_TRIANGLES;
				break;
			default:
				tempDecodeCount = tempTransformCount;
				tempDecodeArray = tempTransformArray;
				tempTransformArray = inVerts;
				break;
	}
	if(tempTransformArray != inVerts)
		free(tempTransformArray);

	// Re-order array. (TODO - Move after clipping, or perform during clipping for lines.)
	fgl_vertex_t tempVertex;
	if(inType == FGL_LINES) {
		for(i = 0 ; i < tempDecodeCount; i += 2) {
			if(tempDecodeArray[i + 0].y > tempDecodeArray[i + 1].y) {
				tempVertex = tempDecodeArray[i + 0];
				tempDecodeArray[i + 0] = tempDecodeArray[i + 1];
				tempDecodeArray[i + 1] = tempVertex;
			}
		}
	}

	// Per-Polygon Clipping
	fgl_vertex_t* tempClipArray;
	uintptr_t     tempClipCount;
	if(_fgl_clip_enable) {
		switch(inType) {
			case FGL_POINTS:
				tempClipCount = tempDecodeCount;
				tempClipArray = (fgl_vertex_t*)malloc(tempClipCount * sizeof(fgl_vertex_t));
				if(tempClipArray == NULL) {
					if(tempDecodeArray != inVerts)
						free(tempDecodeArray);
					printf("FGL_ERROR: Unable to clip array, out of memory.\n"); // TODO - Proper error.
					return;
				}
				for(i = 0, j = 0; i < tempDecodeCount; i++)
					j += fgl_clip_point(&tempDecodeArray[i], &tempClipArray[j]);
				tempClipCount = j;
				break;
			case FGL_LINES:
				tempClipCount = tempDecodeCount;
				tempClipArray = (fgl_vertex_t*)malloc(tempClipCount * sizeof(fgl_vertex_t));
				if(tempClipArray == NULL) {
					if(tempDecodeArray != inVerts)
						free(tempDecodeArray);
					printf("FGL_ERROR: Unable to clip array, out of memory.\n"); // TODO - Proper error.
					return;
				}
				for(i = 0, j = 0; i < tempDecodeCount; i += 2)
					j += fgl_clip_line(&tempDecodeArray[i], &tempDecodeArray[i + 1], &tempClipArray[j]);
				tempClipCount = j;
				break;
			case FGL_SPRITES:
				tempClipCount = tempDecodeCount;
				tempClipArray = (fgl_vertex_t*)malloc(tempClipCount * sizeof(fgl_vertex_t));
				if(tempClipArray == NULL) {
					if(tempDecodeArray != inVerts)
						free(tempDecodeArray);
					printf("FGL_ERROR: Unable to clip array, out of memory.\n"); // TODO - Proper error.
					return;
				}
				for(i = 0, j = 0; i < tempDecodeCount; i += 2)
					j += fgl_clip_sprite(&tempDecodeArray[i], &tempClipArray[j]);
				tempClipCount = j;
				break;
			case FGL_TRIANGLES:
				tempClipCount = tempDecodeCount;
				tempClipArray = (fgl_vertex_t*)malloc(tempClipCount * sizeof(fgl_vertex_t));
				if(tempClipArray == NULL) {
					if(tempDecodeArray != inVerts)
						free(tempDecodeArray);
					printf("FGL_ERROR: Unable to clip array, out of memory.\n"); // TODO - Proper error.
					return;
				}
				for(i = 0, j = 0; i < tempDecodeCount; i += 3)
					j += fgl_clip_triangle(&tempDecodeArray[i], &tempClipArray[j]);
				tempClipCount = j;
				break;
		}
		if(tempDecodeArray != inVerts)
			free(tempDecodeArray);
	} else {
		tempClipArray = tempDecodeArray;
		tempClipCount = tempDecodeCount;
		tempDecodeArray = inVerts;
	}

	switch(inType) {
		case FGL_POINTS:
			break;
		case FGL_LINES:
			if(_fgl_depth_test_enable) {
				if(_fgl_texture_enable) {
					if(_fgl_color == FGL_COLOR_WHITE) {
						for(i = 0; i < tempClipCount; i += 2)
							fgl_draw_line_dw_dt_tn(tempClipArray[i], tempClipArray[i + 1], _fgl_texture);
					} else {
						for(i = 0; i < tempClipCount; i += 2) {
							if(tempClipArray[i].c == tempClipArray[i + 1].c)
								fgl_draw_line_dw_dt_cs_tn(tempClipArray[i], tempClipArray[i + 1], _fgl_texture, _fgl_color);
							else
								fgl_draw_line_dw_dt_ci_tn(tempClipArray[i], tempClipArray[i + 1], _fgl_texture);
						}
					}
				} else {
					for(i = 0; i < tempClipCount; i += 2) {
						if(tempClipArray[i].c == tempClipArray[i + 1].c)
							fgl_draw_line_dw_dt_cs(tempClipArray[i], tempClipArray[i + 1], _fgl_color);
						else
							fgl_draw_line_dw_dt_ci(tempClipArray[i], tempClipArray[i + 1]);
					}
				}
			} else {
				if(_fgl_texture_enable) {
					if(_fgl_color == FGL_COLOR_WHITE) {
						for(i = 0; i < tempClipCount; i += 2)
							fgl_draw_line_tn(tempClipArray[i], tempClipArray[i + 1], _fgl_texture);
					} else {
						for(i = 0; i < tempClipCount; i += 2) {
							if(tempClipArray[i].c == tempClipArray[i + 1].c)
								fgl_draw_line_cs_tn(tempClipArray[i], tempClipArray[i + 1], _fgl_texture, _fgl_color);
							else
								fgl_draw_line_ci_tn(tempClipArray[i], tempClipArray[i + 1], _fgl_texture);
						}
					}
				} else {
					for(i = 0; i < tempClipCount; i += 2) {
						if(tempClipArray[i].c == tempClipArray[i + 1].c)
							fgl_draw_line_cs(tempClipArray[i], tempClipArray[i + 1], _fgl_color);
						else
							fgl_draw_line_ci(tempClipArray[i], tempClipArray[i + 1]);
					}
				}
			}
			break;
		case FGL_SPRITES:
			if(_fgl_depth_test_enable) {
				if(_fgl_texture_enable) {
					if(_fgl_color == FGL_COLOR_WHITE) {
						for(i = 0; i < tempClipCount; i += 2)
							render_sprite_dw_dt_tn(tempClipArray[i], tempClipArray[i + 1], _fgl_texture);
					} else {
						for(i = 0; i < tempClipCount; i += 2)
							render_sprite_dw_dt_cs_tn(tempClipArray[i], tempClipArray[i + 1], _fgl_texture, _fgl_color);
					}
				} else {
					for(i = 0; i < tempClipCount; i += 2)
						render_sprite_dw_dt_cs(tempClipArray[i], tempClipArray[i + 1], _fgl_color);
				}
			} else {
				if(_fgl_texture_enable) {
					if(_fgl_color == FGL_COLOR_WHITE) {
						for(i = 0; i < tempClipCount; i += 2)
							render_sprite_tn(tempClipArray[i], tempClipArray[i + 1], _fgl_texture);
					} else {
						for(i = 0; i < tempClipCount; i += 2)
							render_sprite_cs_tn(tempClipArray[i], tempClipArray[i + 1], _fgl_texture, _fgl_color);
					}
				} else {
					for(i = 0; i < tempClipCount; i += 2)
						render_sprite_cs(tempClipArray[i], tempClipArray[i + 1], _fgl_color);
				}
			}
			break;
		case FGL_TRIANGLES:
			if(_fgl_depth_test_enable) {
				if(_fgl_clip_enable) {
					if(_fgl_texture_enable) {
						for(i = 0; i < tempClipCount; i += 3) {
							if((tempClipArray[i].c == tempClipArray[i + 1].c) && (tempClipArray[i].c == tempClipArray[i + 2].c)) {
								if(tempClipArray[i].c == FGL_COLOR_WHITE)
									render_triangle_dw_dt_cl_tn(tempClipArray[i], tempClipArray[i + 1], tempClipArray[i + 2], _fgl_texture);
								else
									render_triangle_dw_dt_cl_cs_tn(tempClipArray[i], tempClipArray[i + 1], tempClipArray[i + 2], _fgl_texture, tempClipArray[i].c);
							} else
								render_triangle_dw_dt_cl_ci_tn(tempClipArray[i], tempClipArray[i + 1], tempClipArray[i + 2], _fgl_texture);
						}
					} else {
						for(i = 0; i < tempClipCount; i += 3) {
							if((tempClipArray[i].c == tempClipArray[i + 1].c) && (tempClipArray[i].c == tempClipArray[i + 2].c))
								render_triangle_dw_dt_cl_cs(tempClipArray[i], tempClipArray[i + 1], tempClipArray[i + 2], tempClipArray[i].c);
							else
								render_triangle_dw_dt_cl_ci(tempClipArray[i], tempClipArray[i + 1], tempClipArray[i + 2]);
						}
					}
				} else {
					if(_fgl_texture_enable) {
						for(i = 0; i < tempClipCount; i += 3) {
							if((tempClipArray[i].c == tempClipArray[i + 1].c) && (tempClipArray[i].c == tempClipArray[i + 2].c)) {
								if(tempClipArray[i].c == FGL_COLOR_WHITE)
									render_triangle_dw_dt_tn(tempClipArray[i], tempClipArray[i + 1], tempClipArray[i + 2], _fgl_texture);
								else
									render_triangle_dw_dt_cs_tn(tempClipArray[i], tempClipArray[i + 1], tempClipArray[i + 2], _fgl_texture, tempClipArray[i].c);
							} else
								render_triangle_dw_dt_ci_tn(tempClipArray[i], tempClipArray[i + 1], tempClipArray[i + 2], _fgl_texture);
						}
					} else {
						for(i = 0; i < tempClipCount; i += 3) {
							if((tempClipArray[i].c == tempClipArray[i + 1].c) && (tempClipArray[i].c == tempClipArray[i + 2].c))
								render_triangle_dw_dt_cs(tempClipArray[i], tempClipArray[i + 1], tempClipArray[i + 2], tempClipArray[i].c);
							else
								render_triangle_dw_dt_ci(tempClipArray[i], tempClipArray[i + 1], tempClipArray[i + 2]);
						}
					}

				}
			} else {
				if(_fgl_clip_enable) {
					if(_fgl_texture_enable) {
						for(i = 0; i < tempClipCount; i += 3) {
							if((tempClipArray[i].c == tempClipArray[i + 1].c) && (tempClipArray[i].c == tempClipArray[i + 2].c)) {
								if(tempClipArray[i].c == FGL_COLOR_WHITE)
									render_triangle_cl_tn(tempClipArray[i], tempClipArray[i + 1], tempClipArray[i + 2], _fgl_texture);
								else
									render_triangle_cl_cs_tn(tempClipArray[i], tempClipArray[i + 1], tempClipArray[i + 2], _fgl_texture, tempClipArray[i].c);
							} else
								render_triangle_cl_ci_tn(tempClipArray[i], tempClipArray[i + 1], tempClipArray[i + 2], _fgl_texture);
						}
					} else {
						for(i = 0; i < tempClipCount; i += 3) {
							if((tempClipArray[i].c == tempClipArray[i + 1].c) && (tempClipArray[i].c == tempClipArray[i + 2].c))
								render_triangle_cl_cs(tempClipArray[i], tempClipArray[i + 1], tempClipArray[i + 2], tempClipArray[i].c);
							else
								render_triangle_cl_ci(tempClipArray[i], tempClipArray[i + 1], tempClipArray[i + 2]);
						}
					}
				} else {
					if(_fgl_texture_enable) {
						for(i = 0; i < tempClipCount; i += 3) {
							if((tempClipArray[i].c == tempClipArray[i + 1].c) && (tempClipArray[i].c == tempClipArray[i + 2].c)) {
								if(tempClipArray[i].c == FGL_COLOR_WHITE)
									render_triangle_tn(tempClipArray[i], tempClipArray[i + 1], tempClipArray[i + 2], _fgl_texture);
								else
									render_triangle_cs_tn(tempClipArray[i], tempClipArray[i + 1], tempClipArray[i + 2], _fgl_texture, tempClipArray[i].c);
							} else
								render_triangle_ci_tn(tempClipArray[i], tempClipArray[i + 1], tempClipArray[i + 2], _fgl_texture);
						}
					} else {
						for(i = 0; i < tempClipCount; i += 3) {
							if((tempClipArray[i].c == tempClipArray[i + 1].c) && (tempClipArray[i].c == tempClipArray[i + 2].c))
								render_triangle_cs(tempClipArray[i], tempClipArray[i + 1], tempClipArray[i + 2], tempClipArray[i].c);
							else
								render_triangle_ci(tempClipArray[i], tempClipArray[i + 1], tempClipArray[i + 2]);
						}
					}

				}
			}
			break;
	}

	if(tempClipArray != inVerts)
		free(tempClipArray);
}
