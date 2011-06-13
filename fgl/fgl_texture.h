#ifndef __fgl_texture_h__
#define __fgl_texture_h__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>

#include <fgl/fgl_color.h>

typedef struct {
	uint16_t     width, height;
	uint32_t     reserved;
	fgl_color_t* data;
	void*        mipmap;
} fgl_texture;

#define fgl_texture_texel(inTexture, inX, inY) ((inTexture)->data[((inY) * (inTexture)->width) + (inX)])

extern fgl_texture  _fgl_texture_default;
extern fgl_texture* _fgl_texture;

extern fgl_texture* fgl_texture_create(uint16_t inWidth, uint16_t inHeight);
extern void         fgl_texture_delete(fgl_texture* inTexture);

extern fgl_texture* fgl_texture_load_tga(const char* inPath);
extern bool         fgl_texture_save_tga(const char* inPath, fgl_texture* inTexture);

extern bool         fgl_texture_bind(fgl_texture* inTexture);
extern void         fgl_texture_clear(fgl_texture* inTexture);
extern bool         fgl_texture_mipmap(fgl_texture* inTexture);

#ifdef __cplusplus
}
#endif

#endif
