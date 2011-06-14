#include "fgl_texture.h"
#include "fgl_color.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



typedef struct {
	uint8_t  ident_size;
	uint8_t  palette_type, image_type;
	uint16_t palette_start, palette_size;
	uint8_t  palette_bpp;
	int16_t  origin_x, origin_y;
	int16_t  image_width, image_height;
	uint8_t  image_bpp, image_descriptor;
} _tga_header;

fgl_texture  _fgl_texture_default = { 1, 2, 0xFFFFFFFF, (fgl_color_t*)&_fgl_texture_default.reserved, NULL };
fgl_texture* _fgl_texture = &_fgl_texture_default;



fgl_texture* fgl_texture_create(uint16_t inWidth, uint16_t inHeight) {
	fgl_texture* tempOut = (fgl_texture*)malloc(sizeof(fgl_texture) + ((inWidth * inHeight) << 1));
	if(tempOut == NULL)
		return NULL;
	tempOut->width    = inWidth;
	tempOut->height   = inHeight;
	tempOut->reserved = 0;
	tempOut->data     = (fgl_color_t*)((uintptr_t)tempOut + sizeof(fgl_texture));
	tempOut->mipmap   = NULL;
	return tempOut;
}

bool fgl_texture_bind(fgl_texture* inTexture) {
	if(inTexture == NULL)
		_fgl_texture = &_fgl_texture_default;
	_fgl_texture = inTexture;
	return true;
}

void fgl_texture_clear(fgl_texture* inTexture) {
	memset(inTexture->data, 0x00, ((inTexture->width * inTexture->height) << 1));
	if(inTexture->mipmap)
		fgl_texture_clear((fgl_texture*)inTexture->mipmap);
}

void fgl_texture_delete(fgl_texture* inTexture) {
	if(inTexture == &_fgl_texture_default)
		return;
	if(_fgl_texture == inTexture)
		_fgl_texture = &_fgl_texture_default;
	free(inTexture);
}



fgl_texture* fgl_texture_load_tga(const char* inPath) {
	if(inPath == NULL)
		return NULL;

	FILE* tempFile = fopen(inPath, "rb");
	if(tempFile == NULL)
		return NULL;

	_tga_header tempHeader;

	fread(&tempHeader.ident_size, 1, 1, tempFile);
	fread(&tempHeader.palette_type, 1, 1, tempFile);
	fread(&tempHeader.image_type, 1, 1, tempFile);

	fread(&tempHeader.palette_start, 2, 1, tempFile);
	fread(&tempHeader.palette_size, 2, 1, tempFile);
	fread(&tempHeader.palette_bpp, 1, 1, tempFile);

	fread(&tempHeader.origin_x, 2, 1, tempFile);
	fread(&tempHeader.origin_y, 2, 1, tempFile);
	fread(&tempHeader.image_width, 2, 1, tempFile);
	fread(&tempHeader.image_height, 2, 1, tempFile);
	fread(&tempHeader.image_bpp, 1, 1, tempFile);
	fread(&tempHeader.image_descriptor, 1, 1, tempFile);

	bool tempVFlip = (tempHeader.image_descriptor & 0x20);
	if(tempHeader.image_height < 0) {
		tempVFlip = !tempVFlip;
		tempHeader.image_height = -tempHeader.image_height;
	}

	bool tempHFlip = (tempHeader.image_descriptor & 0x10);
	if(tempHeader.image_width < 0) {
		tempHFlip = !tempHFlip;
		tempHeader.image_width = -tempHeader.image_width;
	}

	bool tempError = false;
	tempError |= (tempHeader.palette_type != 0);
	tempError |= (tempHeader.image_type != 2);
	tempError |= ((tempHeader.image_bpp != 24) && (tempHeader.image_bpp != 32));
	if(tempError) {
		fclose(tempFile);
		return NULL;
	}

	fseek(tempFile, tempHeader.ident_size, SEEK_CUR);

	fgl_texture* tempTexture = fgl_texture_create(tempHeader.image_width, tempHeader.image_height);
	if(tempTexture == NULL) {
		fclose(tempFile);
		return NULL;
	}

	uint8_t tempByPP = (tempHeader.image_bpp >> 3);
	uint8_t tempColor[4];

	intptr_t i, j;
	intptr_t i_start = (tempHFlip ? (tempHeader.image_width - 1) : 0);
	intptr_t j_start = (tempVFlip ? 0 : (tempHeader.image_height - 1));
	intptr_t i_inc   = (tempHFlip ? -1 : 1);
	intptr_t j_inc   = (tempVFlip ? 1 : -1);
	intptr_t i_end   = (tempHFlip ? -1 : tempHeader.image_width);
	intptr_t j_end   = (tempVFlip ? tempHeader.image_height : -1);

	for(j = j_start; j != j_end; j += j_inc) {
		for(i = i_start; i != i_end; i += i_inc) {
			fread(tempColor, 1, tempByPP, tempFile);
			tempTexture->data[(j * tempTexture->width) + i] = (((tempColor[2] & 0xF8) << 8) | ((tempColor[1] & 0xFC) << 3) | (tempColor[0] >> 3));
		}
	}

	fclose(tempFile);

	return tempTexture;
}

bool fgl_texture_save_tga(const char* inPath, fgl_texture* inTexture) {
	if((inPath == NULL) || (inTexture == NULL))
		return false;

	FILE* tempFile = fopen(inPath, "wb");
	if(tempFile == NULL)
		return false;

	_tga_header tempHeader;

	tempHeader.ident_size   = 0;
	tempHeader.palette_type = 0;
	tempHeader.image_type   = 2;

	tempHeader.palette_start = 18;
	tempHeader.palette_size  = 0;
	tempHeader.palette_bpp   = 24;

	tempHeader.origin_x         = 0;
	tempHeader.origin_y         = 0;
	tempHeader.image_width      = inTexture->width;
	tempHeader.image_height     = inTexture->height;
	tempHeader.image_bpp        = 24;
	tempHeader.image_descriptor = 0x20;

	fwrite(&tempHeader.ident_size, 1, 1, tempFile);
	fwrite(&tempHeader.palette_type, 1, 1, tempFile);
	fwrite(&tempHeader.image_type, 1, 1, tempFile);

	fwrite(&tempHeader.palette_start, 2, 1, tempFile);
	fwrite(&tempHeader.palette_size, 2, 1, tempFile);
	fwrite(&tempHeader.palette_bpp, 1, 1, tempFile);

	fwrite(&tempHeader.origin_x, 2, 1, tempFile);
	fwrite(&tempHeader.origin_y, 2, 1, tempFile);
	fwrite(&tempHeader.image_width, 2, 1, tempFile);
	fwrite(&tempHeader.image_height, 2, 1, tempFile);
	fwrite(&tempHeader.image_bpp, 1, 1, tempFile);
	fwrite(&tempHeader.image_descriptor, 1, 1, tempFile);

	uintptr_t i;
	uint8_t tempColor[3];
	for(i = 0; i < (uint32_t)(inTexture->width * inTexture->height); i++) {
		tempColor[2] = fgl_color_red(inTexture->data[i]);
		tempColor[1] = fgl_color_green(inTexture->data[i]);
		tempColor[0] = fgl_color_blue(inTexture->data[i]);
		tempColor[2] = ((tempColor[2] << 3) | (tempColor[2] >> 2));
		tempColor[1] = ((tempColor[1] << 2) | (tempColor[1] >> 4));
		tempColor[0] = ((tempColor[0] << 3) | (tempColor[0] >> 2));
		fwrite(tempColor, 1, 3, tempFile);
	}

	fclose(tempFile);
	return true;
}



bool fgl_texture_mipmap(fgl_texture* inTexture) {
	if(inTexture->width == 0) {
		if(inTexture->height == 0)
			return false;
		// TODO - Halve height.
		return false;
	}
	if(inTexture->height == 0) {
		fgl_texture* tempMip = fgl_texture_create((inTexture->width - 1), (inTexture->height - 1));
		if(tempMip == NULL)
			return false;
		fgl_color_t* tempTexPtr = inTexture->data;
		fgl_color_t* tempMipPtr = tempMip->data;
		fgl_color_t* tempTexEnd = &inTexture->data[1 << inTexture->width << inTexture->height];
		for(; tempTexPtr < tempTexEnd; tempTexPtr += 2)
			*(tempMipPtr++) = fgl_color_halfblend(tempTexPtr[0], tempTexPtr[1]);
		inTexture->mipmap = (void*)tempMip;
		if(inTexture->width > 0)
			return fgl_texture_mipmap(tempMip);
		return true;
	}

	fgl_texture* tempMip = fgl_texture_create((inTexture->width - 1), (inTexture->height - 1));
	if(tempMip == NULL)
		return false;

	// TODO - Generate mipmap.

	inTexture->mipmap = (void*)tempMip;
	if((inTexture->width > 0) || (inTexture->height > 0))
		return fgl_texture_mipmap(tempMip);
	return true;
}
