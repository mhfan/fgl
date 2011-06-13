// Defines:
// _depth_write         - Enables depth writes.
// _depth_test          - Enables depth testing.
// _polygon_color       - Enables polygon colors.
// _vertex_color        - Enables vertex colors.
// _texture             - Enables texture mapping.
// _texture_perspective - Enables texture perspective correction. (TODO)
// _clip                - Enables clipping.
//   _clip_x            - Enables x clipping.
//     _clip_left       - Enables left  clipping.
//     _clip_right      - Enables right clipping.
//   _clip_y            - Enables y clipping.
//     _clip_top        - Enables top    clipping.
//     _clip_bottom     - Enables bottom clipping.
//   _clip_z            - Enables z clipping. (TODO)

// TODO - Fix texturing glitches.


#ifndef min
	#define min(x, y) ((x) < (y) ? (x) : (y))
#endif

#ifndef max
	#define max(x, y) ((x) > (y) ? (x) : (y))
#endif

#ifndef swap
	#define swap(x, y) { (x) ^= (y); (y) ^= (x); (x) ^= (y); }
#endif



#ifdef _clip
	#ifndef _clip_x
		#define _clip_x
	#endif
	#ifndef _clip_y
		#define _clip_y
	#endif
#endif

#ifdef _clip_x
	#ifndef _clip_left
		#define _clip_left
	#endif
	#ifndef _clip_right
		#define _clip_right
	#endif
#endif

#ifdef _clip_y
	#ifndef _clip_top
		#define _clip_top
	#endif
	#ifndef _clip_bottom
		#define _clip_bottom
	#endif
#endif

#if (defined(_clip_left) || defined(_clip_right) || defined(_clip_top) || defined(_clip_bottom))
	#define _clip_partial
#endif



#if (defined(_vertex_color) && defined(_polygon_color))
	#warning "Can't have vertex and polygon colors in rasterizer, multiply first."
	#undef _polygon_color
#endif

#if (defined(_depth_write) || defined(_depth_test))
	#define _depth_interp
	#define _depth_func(x, y) ((x) <= (y))
#endif






/*void _func_name_raster(fgl_color_t* inPtr, uint16_t inLen, fgl_color_t inColor) {
	asm(
		"sll  $a1, $a1, 1\n\t"
		"add  $a1, $a1, $a0\n\t"
		"; Insert shaders here\n\t"
		"sh   $a2, 0($a0)\n\t"
		"bne  $a0, $a1, -3\n\t"
		"addi $a0, $a0, 2\n\t"
	);
}*/

void _func_name_raster(fgl_color_t* inPtr, fgl_color_t* inEnd
	#ifdef _clip_left
		, uint32_t inClipLeft
	#endif
	#ifdef _clip_right
		, uint32_t inClipRight
	#endif
	#ifdef _depth_interp
		, uint16_t* inDepthPtr, int32_t inZ0, int32_t inZ1
	#endif
	#ifdef _texture
		, fgl_texture* inTexture, int32_t inU0, int32_t inV0, int32_t inU1, int32_t inV1
	#endif
	#if defined(_vertex_color)
		, int32_t inR0, int32_t inG0, int32_t inB0, int32_t inR1, int32_t inG1, int32_t inB1
	#elif (!defined(_texture) || defined(_polygon_color))
		, fgl_color_t inColor
	#endif
	) {
	// Assumes scanline already clipped.
	// Assumes inPtr & inEnd are aligned to 16-bits.
	// TODO - Make inEnd always greater than inPtr
	if(inEnd <= inPtr)
		return;

	#ifdef _texture
	/*if(inU0 < 0)
		inU0 = 0;
	if(inU0 >= (1 << (inTexture->width + 16)))
		inU0 =  ((1 << (inTexture->width + 16)) - 1);
	if(inU1 < 0)
		inU0 = 0;
	if(inU1 >= (1 << (inTexture->width + 16)))
		inU1 =  ((1 << (inTexture->width + 16)) - 1);
	if(inV0 < 0)
		inV0 = 0;
	if(inV0 >= (1 << (inTexture->height + 16)))
		inV0 =  ((1 << (inTexture->height + 16)) - 1);
	if(inV1 < 0)
		inV1 = 0;
	if(inV1 >= (1 << (inTexture->height + 16)))
		inV1 =  ((1 << (inTexture->height + 16)) - 1);*/
	#endif

	// TODO - Speed up fgl_texture lookup.
	#if (!defined(_depth_interp) && !defined(_texture) && !defined(_vertex_color))
		#ifdef _clip_left
			inPtr += inClipLeft;
		#endif
		#ifdef _clip_right
			inEnd -= inClipRight;
		#endif
		if((uintptr_t)inPtr & 1) {
			*(inPtr++) = inColor;
			if(inPtr >= inEnd)
				return;
		}
		uint32_t* tempEnd = (uint32_t*)((uintptr_t)inEnd & 0xFFFFFFFC);
		uint32_t* tempPtr = (uint32_t*)inPtr;
		fgl_color_pair_t tempColor = (inColor | (inColor << 16));
		while(tempPtr < tempEnd)
			*(tempPtr++) = tempColor;
		if((uintptr_t)inEnd & 3)
			*((fgl_color_t*)tempPtr) = inColor;
	#else
		#if (defined(_depth_interp) || defined(_texture) || defined(_vertex_color))
			uint16_t tempDx = (((uintptr_t)inEnd - (uintptr_t)inPtr) >> 1);
			#ifdef _depth_interp
				int32_t tempDz = (inZ1 - inZ0) / tempDx;
			#endif
			#ifdef _texture
				int32_t tempDu = (inU1 - inU0) / tempDx;
				int32_t tempDv = (inV1 - inV0) / tempDx;
				fgl_color_t tempTexel;
			#endif
			#ifdef _vertex_color
				int32_t tempDr = (inR1 - inR0) / tempDx;
				int32_t tempDg = (inG1 - inG0) / tempDx;
				int32_t tempDb = (inB1 - inB0) / tempDx;
				fgl_color_t inColor;
			#endif
		#endif
		#ifdef _clip_left
			inPtr += inClipLeft;
			#ifdef _depth_interp
				inDepthPtr += inClipLeft;
				inZ0 += (tempDz * inClipLeft);
			#endif
			#ifdef _texture
				inU0 += (tempDu * inClipLeft);
				inV0 += (tempDv * inClipLeft);
			#endif
			#ifdef _vertex_color
				inR0 += (tempDr * inClipLeft);
				inG0 += (tempDg * inClipLeft);
				inB0 += (tempDb * inClipLeft);
			#endif
		#endif
		#ifdef _clip_right
			inEnd -= inClipRight;
		#endif
		while(inPtr < inEnd) {
			#ifdef _depth_test
				if(_depth_func((inZ0 >> 15), *inDepthPtr)) {
			#else
				{
			#endif
				#ifdef _vertex_color
					inColor = (((inR0 >> 5) & 0xF800) | ((inG0 >> 11) & 0x07E0) | (inB0 >> 16));
				#endif
				#ifdef _texture
					tempTexel = inTexture->data[((inV0 >> 16) * inTexture->width) + (inU0 >> 16)];
					#if (defined(_vertex_color) || defined(_polygon_color))
					tempTexel = fgl_color_multiply(tempTexel, inColor);
					#endif
					*inPtr = tempTexel;
				#else
					*inPtr = inColor;
				#endif
				#ifdef _depth_write
					*inDepthPtr = (inZ0 >> 15);
				#endif
			}
			inPtr++;
			#ifdef _depth_interp
				inDepthPtr++;
				inZ0 += tempDz;
			#endif
			#ifdef _texture
				inU0 += tempDu;
				inV0 += tempDv;
			#endif
			#ifdef _vertex_color
				inR0 += tempDr;
				inG0 += tempDg;
				inB0 += tempDb;
			#endif
		}
	#endif
}



void _func_name(fgl_vertex_t inV0, fgl_vertex_t inV1, fgl_vertex_t inV2
	#ifdef _texture
	, fgl_texture* inTexture
	#endif
	#ifdef _polygon_color
		, fgl_color_t inColor
	#elif (!defined(_vertex_color) && !defined(_texture))
		#define inColor FGL_COLOR_WHITE
	#endif
) {
	// Assumes verteces are in height order.

	#ifdef _clip_top
		if(inV2.y < 0)
			return;
	#endif
	#ifdef _clip_bottom
		if(inV0.y >= _fgl_clip_height)
			return;
	#endif

	inV0.y >>= 16;
	inV1.y >>= 16;
	inV2.y >>= 16;

	#ifdef _clip_left
		if((inV0.x < 0) && (inV1.x < 0) && (inV2.x < 0))
			return;
	#endif
	#ifdef _clip_right
		if((inV0.x >= _fgl_clip_width) && (inV1.x >= _fgl_clip_width) && (inV2.x >= _fgl_clip_width))
			return;
	#endif

	// Setup interpolation variables.
	int32_t tempX[2] = { inV0.x, inV0.x };
	#ifdef _depth_interp
	int32_t tempZ[2] = { (inV0.z << 15), (inV0.z << 15) };
	#endif
	#ifdef _texture
	int32_t tempU[2] = { (inV0.u << 16), (inV0.u << 16) };
	int32_t tempV[2] = { (inV0.v << 16), (inV0.v << 16) };
	#endif
	#ifdef _vertex_color
	int32_t tempR[2] = { (fgl_color_red(inV0.c) << 16), (fgl_color_red(inV0.c) << 16) };
	int32_t tempG[2] = { (fgl_color_green(inV0.c) << 16), (fgl_color_green(inV0.c) << 16) };
	int32_t tempB[2] = { (fgl_color_blue(inV0.c) << 16), (fgl_color_blue(inV0.c) << 16) };
	#endif

	// Setup buffer pointer.
	fgl_color_t* tempPtr = &_fgl_draw_buffer->data[inV0.y * _fgl_draw_buffer->width];
	#ifdef _depth_interp
	uint16_t* tempDepthPtr = &_fgl_depth_buffer[inV0.y * _fgl_draw_buffer->width];
	#endif
	fgl_color_t* tempEnd[2];
	#ifdef _clip_bottom
		tempEnd[0] = &_fgl_draw_buffer->data[min((_fgl_clip_height >> 16), inV1.y) * _fgl_draw_buffer->width];
		tempEnd[1] = &_fgl_draw_buffer->data[min((_fgl_clip_height >> 16), inV2.y) * _fgl_draw_buffer->width];
	#else
		tempEnd[0] = &_fgl_draw_buffer->data[inV1.y * _fgl_draw_buffer->width];
		tempEnd[1] = &_fgl_draw_buffer->data[inV2.y * _fgl_draw_buffer->width];
	#endif
	uint16_t tempWidth = _fgl_draw_buffer->width;

	// Calculate interpolation deltas.
	int32_t tempDx[3];
	uint16_t tempDy;
	#ifdef _depth_interp
	int32_t tempDz[3];
	#endif
	#ifdef _texture
	int32_t tempDu[3];
	int32_t tempDv[3];
	#endif
	#ifdef _vertex_color
	int32_t tempDr[3];
	int32_t tempDg[3];
	int32_t tempDb[3];
	#endif
	if((inV1.y - inV0.y) > 0) {
		tempDy = (inV1.y - inV0.y);
		tempDx[0] = (inV1.x - inV0.x) / tempDy;
		#ifdef _depth_interp
		tempDz[0] = ((inV1.z - inV0.z) << 15) / tempDy;
		#endif
		#ifdef _texture
		tempDu[0] = ((inV1.u - inV0.u) << 16) / tempDy;
		tempDv[0] = ((inV1.v - inV0.v) << 16) / tempDy;
		#endif
		#ifdef _vertex_color
		tempDr[0] = ((fgl_color_red(inV1.c) - fgl_color_red(inV0.c)) << 16) / tempDy;
		tempDg[0] = ((fgl_color_green(inV1.c) - fgl_color_green(inV0.c)) << 16) / tempDy;
		tempDb[0] = ((fgl_color_blue(inV1.c) - fgl_color_blue(inV0.c)) << 16) / tempDy;
		#endif
	} else {
		if(inV1.x > inV0.x) {
			tempX[1] = inV1.x;
			#ifdef _depth_interp
			tempZ[1] = (inV1.z << 15);
			#endif
			#ifdef _texture
			tempU[1] = (inV1.u << 16);
			tempV[1] = (inV1.v << 16);
			#endif
			#ifdef _vertex_color
			tempR[1] = (fgl_color_red(inV1.c) << 16);
			tempG[1] = (fgl_color_green(inV1.c) << 16);
			tempB[1] = (fgl_color_blue(inV1.c) << 16);
			#endif
		} else {
			tempX[0] = inV1.x;
			#ifdef _depth_interp
			tempZ[0] = (inV1.z << 15);
			#endif
			#ifdef _texture
			tempU[0] = (inV1.u << 16);
			tempV[0] = (inV1.v << 16);
			#endif
			#ifdef _vertex_color
			tempR[0] = (fgl_color_red(inV1.c) << 16);
			tempG[0] = (fgl_color_green(inV1.c) << 16);
			tempB[0] = (fgl_color_blue(inV1.c) << 16);
			#endif
		}
		tempDx[0] = (inV1.x - inV0.x);
	}
	if((inV2.y - inV0.y) > 0) {
		tempDy = (inV2.y - inV0.y);
		tempDx[1] = (inV2.x - inV0.x) / tempDy;
		#ifdef _depth_interp
		tempDz[1] = ((inV2.z - inV0.z) << 15) / tempDy;
		#endif
		#ifdef _texture
		tempDu[1] = ((inV2.u - inV0.u) << 16) / tempDy;
		tempDv[1] = ((inV2.v - inV0.v) << 16) / tempDy;
		#endif
		#ifdef _vertex_color
		tempDr[1] = ((fgl_color_red(inV2.c) - fgl_color_red(inV0.c)) << 16) / tempDy;
		tempDg[1] = ((fgl_color_green(inV2.c) - fgl_color_green(inV0.c)) << 16) / tempDy;
		tempDb[1] = ((fgl_color_blue(inV2.c) - fgl_color_blue(inV0.c)) << 16) / tempDy;
		#endif
	} else
		tempDx[1] = 0;
	if((inV2.y - inV1.y) > 0) {
		tempDy = (inV2.y - inV1.y);
		tempDx[2] = (inV2.x - inV1.x) / tempDy;
		#ifdef _depth_interp
		tempDz[2] = ((inV2.z - inV1.z) << 15) / tempDy;
		#endif
		#ifdef _texture
		tempDu[2] = ((inV2.u - inV1.u) << 16) / tempDy;
		tempDv[2] = ((inV2.v - inV1.v) << 16) / tempDy;
		#endif
		#ifdef _vertex_color
		tempDr[2] = ((fgl_color_red(inV2.c) - fgl_color_red(inV1.c)) << 16) / tempDy;
		tempDg[2] = ((fgl_color_green(inV2.c) - fgl_color_green(inV1.c)) << 16) / tempDy;
		tempDb[2] = ((fgl_color_blue(inV2.c) - fgl_color_blue(inV1.c)) << 16) / tempDy;
		#endif
	}

	// Rasterize triangle.
	if(tempDx[0] > tempDx[1]) {
		#ifdef _clip_top
		if(inV0.y < 0) {
			if(inV1.y < 0) {
				tempDy = (inV1.y - inV0.y);
				tempPtr = tempEnd[0];
			} else {
				tempDy = -(inV0.y);
				tempPtr += (tempDy * _fgl_draw_buffer->width);
			}
			tempX[0] += (tempDx[1] * tempDy);
			tempX[1] += (tempDx[0] * tempDy);
			#ifdef _depth_interp
			tempZ[0] += (tempDz[1] * tempDy);
			tempZ[1] += (tempDz[0] * tempDy);
			tempDepthPtr += (tempDy * _fgl_draw_buffer->width);
			#endif
			#ifdef _texture
			tempU[0] += (tempDu[1] * tempDy);
			tempU[1] += (tempDu[0] * tempDy);
			tempV[0] += (tempDv[1] * tempDy);
			tempV[1] += (tempDv[0] * tempDy);
			#endif
			#ifdef _vertex_color
			tempR[0] += (tempDr[1] * tempDy);
			tempR[1] += (tempDr[0] * tempDy);
			tempG[0] += (tempDg[1] * tempDy);
			tempG[1] += (tempDg[0] * tempDy);
			tempB[0] += (tempDb[1] * tempDy);
			tempB[1] += (tempDb[0] * tempDy);
			#endif
		}
		#endif
		for(; tempPtr < tempEnd[0]; tempPtr += tempWidth) {
			_func_name_raster(&tempPtr[tempX[0] >> 16], &tempPtr[tempX[1] >> 16]
			#ifdef _clip_left
			, (tempX[0] < 0 ? ((-tempX[0]) >> 16) : 0)
			#endif
			#ifdef _clip_right
			, (tempX[1] > _fgl_clip_width ? ((tempX[1] - _fgl_clip_width) >> 16) : 0)
			#endif
			#ifdef _depth_interp
			, &tempDepthPtr[tempX[0] >> 16], tempZ[0], tempZ[1]
			#endif
			#ifdef _texture
			, inTexture, tempU[0], tempV[0], tempU[1], tempV[1]
			#endif
			#if defined(_vertex_color)
			, tempR[0], tempG[0], tempB[0], tempR[1], tempG[1], tempB[1]
			#elif (!defined(_texture) || defined(_polygon_color))
			, inColor
			#endif
			);
			tempX[0] += tempDx[1];
			tempX[1] += tempDx[0];
			#ifdef _depth_interp
			tempZ[0] += tempDz[1];
			tempZ[1] += tempDz[0];
			tempDepthPtr += tempWidth;
			#endif
			#ifdef _texture
			tempU[0] += tempDu[1];
			tempU[1] += tempDu[0];
			tempV[0] += tempDv[1];
			tempV[1] += tempDv[0];
			#endif
			#ifdef _vertex_color
			tempR[0] += tempDr[1];
			tempR[1] += tempDr[0];
			tempG[0] += tempDg[1];
			tempG[1] += tempDg[0];
			tempB[0] += tempDb[1];
			tempB[1] += tempDb[0];
			#endif
		}
		#ifdef _clip_top
		if(inV1.y < 0) {
			tempDy = -(inV1.y);
			tempPtr += (tempDy * _fgl_draw_buffer->width);
			tempX[0] += (tempDx[1] * tempDy);
			tempX[1] += (tempDx[2] * tempDy);
			#ifdef _depth_interp
			tempZ[0] += (tempDz[1] * tempDy);
			tempZ[1] += (tempDz[2] * tempDy);
			tempDepthPtr += tempDy * _fgl_draw_buffer->width;
			#endif
			#ifdef _texture
			tempU[0] += (tempDu[1] * tempDy);
			tempU[1] += (tempDu[2] * tempDy);
			tempV[0] += (tempDv[1] * tempDy);
			tempV[1] += (tempDv[2] * tempDy);
			#endif
			#ifdef _vertex_color
			tempR[0] += (tempDr[1] * tempDy);
			tempR[1] += (tempDr[2] * tempDy);
			tempG[0] += (tempDg[1] * tempDy);
			tempG[1] += (tempDg[2] * tempDy);
			tempB[0] += (tempDb[1] * tempDy);
			tempB[1] += (tempDb[2] * tempDy);
			#endif
		}
		#endif
		for(; tempPtr < tempEnd[1]; tempPtr += tempWidth) {
			_func_name_raster(&tempPtr[tempX[0] >> 16], &tempPtr[tempX[1] >> 16]
			#ifdef _clip_left
			, (tempX[0] < 0 ? ((-tempX[0]) >> 16) : 0)
			#endif
			#ifdef _clip_right
			, (tempX[1] > _fgl_clip_width ? ((tempX[1] - _fgl_clip_width) >> 16) : 0)
			#endif
			#ifdef _depth_interp
			, &tempDepthPtr[tempX[0] >> 16], tempZ[0], tempZ[1]
			#endif
			#ifdef _texture
			, inTexture, tempU[0], tempV[0], tempU[1], tempV[1]
			#endif
			#if defined(_vertex_color)
			, tempR[0], tempG[0], tempB[0], tempR[1], tempG[1], tempB[1]
			#elif (!defined(_texture) || defined(_polygon_color))
			, inColor
			#endif
			);
			tempX[0] += tempDx[1];
			tempX[1] += tempDx[2];
			#ifdef _depth_interp
			tempZ[0] += tempDz[1];
			tempZ[1] += tempDz[2];
			tempDepthPtr += tempWidth;
			#endif
			#ifdef _texture
			tempU[0] += tempDu[1];
			tempU[1] += tempDu[2];
			tempV[0] += tempDv[1];
			tempV[1] += tempDv[2];
			#endif
			#ifdef _vertex_color
			tempR[0] += tempDr[1];
			tempR[1] += tempDr[2];
			tempG[0] += tempDg[1];
			tempG[1] += tempDg[2];
			tempB[0] += tempDb[1];
			tempB[1] += tempDb[2];
			#endif
		}
	} else {
		#ifdef _clip_top
		if(inV0.y < 0) {
			if(inV1.y < 0) {
				tempDy = (inV1.y - inV0.y);
				tempPtr = tempEnd[0];
			} else {
				tempDy = -(inV0.y);
				tempPtr += (tempDy * _fgl_draw_buffer->width);
			}
			tempX[0] += (tempDx[0] * tempDy);
			tempX[1] += (tempDx[1] * tempDy);
			#ifdef _depth_interp
			tempZ[0] += (tempDz[0] * tempDy);
			tempZ[1] += (tempDz[1] * tempDy);
			tempDepthPtr += tempDy * _fgl_draw_buffer->width;
			#endif
			#ifdef _texture
			tempU[0] += (tempDu[0] * tempDy);
			tempU[1] += (tempDu[1] * tempDy);
			tempV[0] += (tempDv[0] * tempDy);
			tempV[1] += (tempDv[1] * tempDy);
			#endif
			#ifdef _vertex_color
			tempR[0] += (tempDr[0] * tempDy);
			tempR[1] += (tempDr[1] * tempDy);
			tempG[0] += (tempDg[0] * tempDy);
			tempG[1] += (tempDg[1] * tempDy);
			tempB[0] += (tempDb[0] * tempDy);
			tempB[1] += (tempDb[1] * tempDy);
			#endif
		}
		#endif
		for(; tempPtr < tempEnd[0]; tempPtr += tempWidth) {
			_func_name_raster(&tempPtr[tempX[0] >> 16], &tempPtr[tempX[1] >> 16]
			#ifdef _clip_left
			, (tempX[0] < 0 ? ((-tempX[0]) >> 16) : 0)
			#endif
			#ifdef _clip_right
			, (tempX[1] > _fgl_clip_width ? ((tempX[1] - _fgl_clip_width) >> 16) : 0)
			#endif
			#ifdef _depth_interp
			, &tempDepthPtr[tempX[0] >> 16], tempZ[0], tempZ[1]
			#endif
			#ifdef _texture
			, inTexture, tempU[0], tempV[0], tempU[1], tempV[1]
			#endif
			#if defined(_vertex_color)
			, tempR[0], tempG[0], tempB[0], tempR[1], tempG[1], tempB[1]
			#elif (!defined(_texture) || defined(_polygon_color))
			, inColor
			#endif
			);
			tempX[0] += tempDx[0];
			tempX[1] += tempDx[1];
			#ifdef _depth_interp
			tempZ[0] += tempDz[0];
			tempZ[1] += tempDz[1];
			tempDepthPtr += tempWidth;
			#endif
			#ifdef _texture
			tempU[0] += tempDu[0];
			tempU[1] += tempDu[1];
			tempV[0] += tempDv[0];
			tempV[1] += tempDv[1];
			#endif
			#ifdef _vertex_color
			tempR[0] += tempDr[0];
			tempR[1] += tempDr[1];
			tempG[0] += tempDg[0];
			tempG[1] += tempDg[1];
			tempB[0] += tempDb[0];
			tempB[1] += tempDb[1];
			#endif
		}
		#ifdef _clip_top
		if(inV1.y < 0) {
			tempDy = -(inV1.y);
			tempPtr += (tempDy * _fgl_draw_buffer->width);
			tempX[0] += (tempDx[2] * tempDy);
			tempX[1] += (tempDx[1] * tempDy);
			#ifdef _depth_interp
			tempZ[0] += (tempDz[2] * tempDy);
			tempZ[1] += (tempDz[1] * tempDy);
			tempDepthPtr += tempDy * _fgl_draw_buffer->width;
			#endif
			#ifdef _texture
			tempU[0] += (tempDu[2] * tempDy);
			tempU[1] += (tempDu[1] * tempDy);
			tempV[0] += (tempDv[2] * tempDy);
			tempV[1] += (tempDv[1] * tempDy);
			#endif
			#ifdef _vertex_color
			tempR[0] += (tempDr[2] * tempDy);
			tempR[1] += (tempDr[1] * tempDy);
			tempG[0] += (tempDg[2] * tempDy);
			tempG[1] += (tempDg[1] * tempDy);
			tempB[0] += (tempDb[2] * tempDy);
			tempB[1] += (tempDb[1] * tempDy);
			#endif
		}
		#endif
		for(; tempPtr < tempEnd[1]; tempPtr += tempWidth) {
			_func_name_raster(&tempPtr[tempX[0] >> 16], &tempPtr[tempX[1] >> 16]
			#ifdef _clip_left
			, (tempX[0] < 0 ? ((-tempX[0]) >> 16) : 0)
			#endif
			#ifdef _clip_right
			, (tempX[1] > _fgl_clip_width ? ((tempX[1] - _fgl_clip_width) >> 16) : 0)
			#endif
			#ifdef _depth_interp
			, &tempDepthPtr[tempX[0] >> 16], tempZ[0], tempZ[1]
			#endif
			#ifdef _texture
			, inTexture, tempU[0], tempV[0], tempU[1], tempV[1]
			#endif
			#if defined(_vertex_color)
			, tempR[0], tempG[0], tempB[0], tempR[1], tempG[1], tempB[1]
			#elif (!defined(_texture) || defined(_polygon_color))
			, inColor
			#endif
			);
			tempX[0] += tempDx[2];
			tempX[1] += tempDx[1];
			#ifdef _depth_interp
			tempZ[0] += tempDz[2];
			tempZ[1] += tempDz[1];
			tempDepthPtr += tempWidth;
			#endif
			#ifdef _texture
			tempU[0] += tempDu[2];
			tempU[1] += tempDu[1];
			tempV[0] += tempDv[2];
			tempV[1] += tempDv[1];
			#endif
			#ifdef _vertex_color
			tempR[0] += tempDr[2];
			tempR[1] += tempDr[1];
			tempG[0] += tempDg[2];
			tempG[1] += tempDg[1];
			tempB[0] += tempDb[2];
			tempB[1] += tempDb[1];
			#endif
		}
	}
}



#ifdef _func_name_sprite
void _func_name_sprite(fgl_vertex_t inV0, fgl_vertex_t inV1
	#ifdef _texture
	, fgl_texture* inTexture
	#endif
	#ifdef _polygon_color
		, fgl_color_t inColor
	#elif (!defined(_vertex_color) && !defined(_texture))
		#define inColor FGL_COLOR_WHITE
	#endif
) {
	// Assumes verteces are in height order.

	#ifdef _clip_top
		if(inV1.y < 0)
			return;
	#endif
	#ifdef _clip_bottom
		if(inV0.y >= _fgl_clip_height)
			return;
	#endif

	inV0.y >>= 16;
	inV1.y >>= 16;

	if(inV0.x > inV1.x) {
		swap(inV0.x, inV1.x);
		swap(inV0.u, inV1.u);
	}

	#ifdef _clip_left
		if(inV0.x < 0)
			return;
	#endif
	#ifdef _clip_right
		if(inV1.x >= _fgl_clip_width)
			return;
	#endif

	// Setup interpolation variables.
	#ifdef _depth_interp
	int32_t tempZ = ((inV0.z + inV1.z) << 14);
	#endif
	#ifdef _texture
	int32_t tempV = (inV0.v << 16);
	#endif
	#ifdef _vertex_color
	int32_t tempR = ((fgl_color_red(inV0.c) + fgl_color_red(inV1.c)) << 15);
	int32_t tempG = ((fgl_color_green(inV0.c) + fgl_color_green(inV1.c)) << 15);
	int32_t tempB = ((fgl_color_blue(inV0.c) + fgl_color_blue(inV1.c)) << 15);
	#endif

	// Setup buffer pointer.
	fgl_color_t* tempPtr = &_fgl_draw_buffer->data[inV0.y * _fgl_draw_buffer->width];
	#ifdef _depth_interp
	uint16_t* tempDepthPtr = &_fgl_depth_buffer[inV0.y * _fgl_draw_buffer->width];
	#endif
	#ifdef _clip_bottom
		fgl_color_t* tempEnd = &_fgl_draw_buffer->data[min((_fgl_clip_height >> 16), inV1.y) * _fgl_draw_buffer->width];
	#else
		fgl_color_t* tempEnd = &_fgl_draw_buffer->data[inV1.y * _fgl_draw_buffer->width];
	#endif
	uint16_t tempWidth = _fgl_draw_buffer->width;

	// Calculate interpolation deltas.
	uint16_t tempDy = (inV1.y - inV0.y);
	#ifdef _texture
	int32_t tempDv = ((inV1.v - inV0.v) << 16) / tempDy;
	#endif

	// Clip sprite.
	#ifdef _clip_top
	if(inV0.y < 0) {
		tempDy = -(inV0.y);
		tempPtr += (tempDy * _fgl_draw_buffer->width);
		#ifdef _depth_interp
		tempDepthPtr += (tempDy * _fgl_draw_buffer->width);
		#endif
		#ifdef _texture
		tempV += (tempDv * tempDy);
		#endif
	}
	#endif

	// Rasterize sprite.
	for(; tempPtr < tempEnd; tempPtr += tempWidth) {
		_func_name_raster(&tempPtr[inV0.x >> 16], &tempPtr[inV1.x >> 16]
		#ifdef _clip_left
		, (inV0.x < 0 ? ((-inV0.x) >> 16) : 0)
		#endif
		#ifdef _clip_right
		, (inV1.x > _fgl_clip_width ? ((inV1.x - _fgl_clip_width) >> 16) : 0)
		#endif
		#ifdef _depth_interp
		, &tempDepthPtr[inV0.x >> 16], tempZ, tempZ
		#endif
		#ifdef _texture
		, inTexture, (inV0.u << 16) , tempV, (inV1.u << 16), tempV
		#endif
		#if defined(_vertex_color)
		, tempR, tempG, tempB, tempR, tempG, tempB
		#elif (!defined(_texture) || defined(_polygon_color))
		, inColor
		#endif
		);
		#ifdef _depth_interp
		tempDepthPtr += tempWidth;
		#endif
		#ifdef _texture
		tempV += tempDv;
		#endif
	}
}
#endif


#undef _depth_interp
#undef _depth_func
#undef inColor

#undef _func_name
#undef _func_name_raster
#undef _func_name_sprite
#undef _depth_write
#undef _depth_test
#undef _polygon_color
#undef _vertex_color
#undef _texture
#undef _texture_perspective
#undef _clip
#undef _clip_x
#undef _clip_left
#undef _clip_right
#undef _clip_y
#undef _clip_top
#undef _clip_bottom
#undef _clip_partial
