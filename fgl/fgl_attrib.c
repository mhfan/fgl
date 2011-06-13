#include "fgl_attrib.h"
#include <stdlib.h>

#include "fgl_texture.h"

bool _fgl_texture_enable      = false;
bool _fgl_texture_perspective = false;
bool _fgl_depth_test_enable   = false;
bool _fgl_depth_write_enable  = false;
bool _fgl_transform_enable    = true;
bool _fgl_clip_enable         = true;
bool _fgl_cull_face_enable    = false;

extern fgl_texture* _fgl_texture;
extern uint16_t*    _fgl_depth_buffer;



bool fgl_enable(uint8_t inCap) {
	switch(inCap) {
		case FGL_TEXTURE_2D:
			if(!_fgl_texture_enable && (_fgl_texture == NULL))
				return false;
			_fgl_texture_enable = true;
			return true;
		case FGL_DEPTH_TEST:
			if(_fgl_depth_buffer == NULL)
				return false;
			_fgl_depth_test_enable = true;
			return true;
		case FGL_DEPTH_WRITE:
			return false;
		case FGL_TRANSFORM:
			_fgl_transform_enable = true;
			return true;
		case FGL_CLIP:
			_fgl_clip_enable = true;
			return true;
		case FGL_CULL_FACE:
			_fgl_cull_face_enable = true;
			return true;
		case FGL_TEXTURE_PERSPECTIVE:
			return false;
	}
	return false;
}

bool fgl_disable(uint8_t inCap) {
	switch(inCap) {
		case FGL_TEXTURE_2D:
			_fgl_texture_enable = false;
			return true;
		case FGL_DEPTH_TEST:
			_fgl_depth_test_enable = false;
			return true;
		case FGL_DEPTH_WRITE:
			_fgl_depth_test_enable = false;
			return true;
		case FGL_TRANSFORM:
			_fgl_transform_enable = false;
			return true;
		case FGL_CLIP:
			_fgl_clip_enable = false;
			return true;
		case FGL_CULL_FACE:
			_fgl_cull_face_enable = false;
			return true;
		case FGL_TEXTURE_PERSPECTIVE:
			_fgl_texture_perspective = false;
			return true;
	}
	return false;
}
