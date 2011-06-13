#ifndef __fgl_color_h__
#define __fgl_color_h__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>

#include <libfixmath/fixmath.h>

typedef uint16_t fgl_color_t;
typedef uint32_t fgl_color_pair_t;

#define fgl_color_red(inColor)   ((inColor) >> 11)
#define fgl_color_green(inColor) (((inColor) & 0x07E0) >> 5)
#define fgl_color_blue(inColor)  ((inColor) & 0x001F)

#define fgl_color_rgb(inRed, inGreen, inBlue) (((inRed) << 11) | ((inGreen) << 5) | (inBlue))

#define FGL_COLOR_WHITE 0xFFFF
#define FGL_COLOR_BLACK 0x0000

extern fgl_color_t _fgl_color;

extern fgl_color_t fgl_color_halfblend(fgl_color_t inColor0, fgl_color_t inColor1);
extern fgl_color_t fgl_color_multiply(fgl_color_t inColor0, fgl_color_t inColor1);

extern void fgl_color_set(fgl_color_t inColor);
extern void fgl_color_set_3f(fix16_t inRed, fix16_t inGreen, fix16_t inBlue);
extern void fgl_color_set_3ub(uint8_t inRed, uint8_t inGreen, uint8_t inBlue);
extern void fgl_color_set_3uh(uint16_t inRed, uint16_t inGreen, uint16_t inBlue);

extern void fgl_color_get(fgl_color_t* outColor);
extern void fgl_color_get_3f(fix16_t* outRed, fix16_t* outGreen, fix16_t* outBlue);
extern void fgl_color_get_3ub(uint8_t* outRed, uint8_t* outGreen, uint8_t* outBlue);
extern void fgl_color_get_3uh(uint16_t* outRed, uint16_t* outGreen, uint16_t* outBlue);

#ifdef __cplusplus
}
#endif

#endif
