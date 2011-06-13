#include "fgl_color.h"

#ifndef max
	#define max(x, y) ((x) > (y) ? (x) : (y))
#endif

#ifndef min
	#define min(x, y) ((x) < (y) ? (x) : (y))
#endif


// TODO - Optimize functions to use lookups.

fgl_color_t _fgl_color = FGL_COLOR_WHITE;

fgl_color_t fgl_color_halfblend(fgl_color_t inColor0, fgl_color_t inColor1) {
	if(inColor0 == inColor1)
		return inColor0;

	uint32_t tempOut;
	tempOut  = (((inColor0 & 0x001F) + (inColor1 & 0x001F)) >> 1);
	tempOut |= (((inColor0 & 0x07E0) + (inColor1 & 0x07E0)) >> 1) & 0x07E0;
	tempOut |= (((inColor0 & 0xF800) + (inColor1 & 0xF800)) >> 1) & 0xF800;
	return (fgl_color_t)tempOut;
}

fgl_color_t fgl_color_multiply(fgl_color_t inColor0, fgl_color_t inColor1) {
	if(inColor0 == FGL_COLOR_WHITE)
		return inColor1;
	if(inColor1 == FGL_COLOR_WHITE)
		return inColor0;
	if((inColor0 == FGL_COLOR_BLACK) || (inColor1 == FGL_COLOR_BLACK))
		return FGL_COLOR_BLACK;

	uint16_t tempRed, tempGreen, tempBlue;
	tempRed     = fgl_color_red(inColor0);
	tempRed    += (tempRed * fgl_color_red(inColor1));
	tempRed   >>= 5;
	tempGreen   = fgl_color_green(inColor0);
	tempGreen  += (tempGreen * fgl_color_green(inColor1));
	tempGreen >>= 6;
	tempBlue    = fgl_color_blue(inColor0);
	tempBlue   += (tempBlue * fgl_color_blue(inColor1));
	tempBlue  >>= 5;
	return ((tempRed << 11) | (tempGreen << 5) | tempBlue);
}



void fgl_color_set(fgl_color_t inColor) {
	_fgl_color = inColor;
}

void fgl_color_set_3f(fix16_t inRed, fix16_t inGreen, fix16_t inBlue) {
	_fgl_color = fgl_color_rgb((max(max(inRed, 0), 0xFFFF) >> 11), (max(max(inGreen, 0), 0xFFFF) >> 10), (max(max(inBlue, 0), 0xFFFF) >> 11));
}

void fgl_color_set_3ub(uint8_t inRed, uint8_t inGreen, uint8_t inBlue) {
	_fgl_color = fgl_color_rgb((inRed >> 3), (inGreen >> 2), (inBlue >> 3));
}

void fgl_color_set_3uh(uint16_t inRed, uint16_t inGreen, uint16_t inBlue) {
	_fgl_color = fgl_color_rgb((inRed >> 11), (inGreen >> 10), (inBlue >> 11));
}



void fgl_color_get(fgl_color_t* outColor) {
	*outColor = _fgl_color;
}

void fgl_color_get_3f(fix16_t* outRed, fix16_t* outGreen, fix16_t* outBlue) {
	*outRed   = ((fgl_color_red(_fgl_color) << 11) | (fgl_color_red(_fgl_color) << 6) | (fgl_color_red(_fgl_color) << 1) | (fgl_color_red(_fgl_color) >> 4));
	if(*outRed > 0x00008000) (*outRed)++;
	*outGreen = ((fgl_color_green(_fgl_color) << 10) | (fgl_color_green(_fgl_color) << 4) | (fgl_color_green(_fgl_color) >> 2));
	if(*outGreen > 0x00008000) (*outGreen)++;
	*outBlue  = ((fgl_color_blue(_fgl_color) << 11) | (fgl_color_blue(_fgl_color) << 6) | (fgl_color_blue(_fgl_color) << 1) | (fgl_color_blue(_fgl_color) >> 4));
	if(*outGreen > 0x00008000) (*outGreen)++;
}

void fgl_color_get_3ub(uint8_t* outRed, uint8_t* outGreen, uint8_t* outBlue) {
	*outRed   = ((fgl_color_red(_fgl_color) << 3) | (fgl_color_red(_fgl_color) >> 2));
	*outGreen = ((fgl_color_green(_fgl_color) << 2) | (fgl_color_green(_fgl_color) >> 4));
	*outBlue  = ((fgl_color_blue(_fgl_color) << 3) | (fgl_color_blue(_fgl_color) >> 2));
}

void fgl_color_get_3uh(uint16_t* outRed, uint16_t* outGreen, uint16_t* outBlue) {
	*outRed   = ((fgl_color_red(_fgl_color) << 11) | (fgl_color_red(_fgl_color) << 6) | (fgl_color_red(_fgl_color) << 1) | (fgl_color_red(_fgl_color) >> 4));
	*outGreen = ((fgl_color_green(_fgl_color) << 10) | (fgl_color_green(_fgl_color) << 4) | (fgl_color_green(_fgl_color) >> 2));
	*outBlue  = ((fgl_color_blue(_fgl_color) << 11) | (fgl_color_blue(_fgl_color) << 6) | (fgl_color_blue(_fgl_color) << 1) | (fgl_color_blue(_fgl_color) >> 4));
}
