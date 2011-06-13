// Defines:
// _depth_write         - Enables depth writes.
// _depth_test          - Enables depth testing.
// _polygon_color       - Enables polygon colors.
// _vertex_color        - Enables vertex colors.
// _texture             - Enables texture mapping.
// _texture_perspective - Enables texture perspective correction. (TODO)



#ifndef min
	#define min(x, y) ((x) < (y) ? (x) : (y))
#endif

#ifndef max
	#define max(x, y) ((x) > (y) ? (x) : (y))
#endif

#ifndef swap
	#define swap(x, y) do { typeof(x) z = (x); (x) = (y); (y) = z; } while(0)
#endif



#if (defined(_vertex_color) && defined(_polygon_color))
	#warning "Can't have vertex and polygon colors in rasterizer, multiply first."
	#undef _polygon_color
#endif

#if (defined(_depth_write) || defined(_depth_test))
	#define _depth_interp
	#define _depth_func(x, y) ((x) <= (y))
#endif



void _fgl_draw_line(fgl_vertex_t inV0, fgl_vertex_t inV1
	#ifdef _texture
		, fgl_texture* inTexture
	#endif
	#ifdef _polygon_color
		, fgl_color_t inColor
	#elif (!defined(_vertex_color) && !defined(_texture))
		#define inColor FGL_COLOR_WHITE
	#endif
) {

	fgl_color_t* tempBuffer = _fgl_draw_buffer->data;
	#ifdef _depth_interp
	uint16_t*    tempDepthPtr = _fgl_depth_buffer;
	#endif

	int32_t tempX, tempY;
	#ifdef _depth_interp
	int32_t tempZ = (inV0.z << 15);
	#endif
	#ifdef _texture
	int32_t tempU = (inV0.u << 16);
	int32_t tempV = (inV0.v << 16);
	fgl_color_t tempTexel;
	#endif
	#ifdef _vertex_color
	int32_t tempR = (fgl_color_red(inV0.c) << 16);
	int32_t tempG = (fgl_color_green(inV0.c) << 16);
	int32_t tempB = (fgl_color_blue(inV0.c) << 16);
	fgl_color_t inColor;
	#endif

	if(abs(inV1.y - inV0.y) >= abs(inV1.x - inV0.x)) {
		inV0.y >>= 16;
		inV1.y >>= 16;

		if(inV0.y > inV1.y)
			swap(inV0, inV1);
		else if(inV0.y == inV1.y)
			return;

		int32_t tempDy = (inV1.y - inV0.y);
		int32_t tempDx = (inV1.x - inV0.x) / tempDy;
		#ifdef _depth_interp
		int32_t tempDz = ((inV1.z - inV0.z) << 15) / tempDy;
		#endif
		#ifdef _texture
		int32_t tempDu = ((inV1.u - inV0.u) << 16) / tempDy;
		int32_t tempDv = ((inV1.v - inV0.v) << 16) / tempDy;
		#endif
		#ifdef _vertex_color
		int32_t tempDr = ((fgl_color_red(inV1.c) - fgl_color_red(inV0.c)) << 16) / tempDy;
		int32_t tempDg = ((fgl_color_green(inV1.c) - fgl_color_green(inV0.c)) << 16) / tempDy;
		int32_t tempDb = ((fgl_color_blue(inV1.c) - fgl_color_blue(inV0.c)) << 16) / tempDy;
		#endif

		tempY = inV0.y;
		tempX = inV0.x;
		tempBuffer = &tempBuffer[tempY * _fgl_draw_buffer->width];
		#ifdef _depth_interp
		tempDepthPtr = &tempDepthPtr[tempY * _fgl_draw_buffer->width];
		#endif

		for(; tempY < inV1.y; tempY++, tempX += tempDx) {
			#ifdef _depth_test
			if(_depth_func((tempZ >> 15), tempDepthPtr[tempX >> 16]))
			#endif
			{
				#ifdef _vertex_color
					inColor = (((tempR >> 5) & 0xF800) | ((tempG >> 11) & 0x07E0) | (tempB >> 16));
				#endif
				#ifdef _texture
					tempTexel = inTexture->data[((tempV >> 16) * inTexture->width) + (tempU >> 16)];
					#if (defined(_vertex_color) || defined(_polygon_color))
						tempTexel = fgl_color_multiply(tempTexel, inColor);
					#endif
					tempBuffer[tempX >> 16] = tempTexel;
				#else
					tempBuffer[tempX >> 16] = inColor;
				#endif
				#ifdef _depth_write
					tempDepthPtr[tempX >> 16] = (tempZ >> 15);
				#endif
			}

			tempBuffer = &tempBuffer[_fgl_draw_buffer->width];
			#ifdef _depth_interp
			tempDepthPtr = &tempDepthPtr[_fgl_draw_buffer->width];
			tempZ += tempDz;
			#endif
			#ifdef _texture
			tempU += tempDu;
			tempV += tempDv;
			#endif
			#ifdef _vertex_color
			tempR += tempDr;
			tempG += tempDg;
			tempB += tempDb;
			#endif
		}
	} else {
		inV0.x >>= 16;
		inV1.x >>= 16;

		if(inV0.x > inV1.x)
			swap(inV0, inV1);
		else if(inV0.x == inV1.x)
			return;

		int32_t tempDx = (inV1.x - inV0.x);
		int32_t tempDy = (inV1.y - inV0.y) / tempDx;
		#ifdef _depth_interp
		int32_t tempDz = ((inV1.z - inV0.z) << 15) / tempDx;
		#endif
		#ifdef _texture
		int32_t tempDu = ((inV1.u - inV0.u) << 16) / tempDx;
		int32_t tempDv = ((inV1.v - inV0.v) << 16) / tempDx;
		#endif
		#ifdef _vertex_color
		int32_t tempDr = ((fgl_color_red(inV1.c) - fgl_color_red(inV0.c)) << 16) / tempDx;
		int32_t tempDg = ((fgl_color_green(inV1.c) - fgl_color_green(inV0.c)) << 16) / tempDx;
		int32_t tempDb = ((fgl_color_blue(inV1.c) - fgl_color_blue(inV0.c)) << 16) / tempDx;
		#endif

		tempX = inV0.x;
		tempY = inV0.y;
		for(; tempX < inV1.x; tempX++, tempY += tempDy) {
			if(tempX >= 0) {
				#ifdef _depth_test
				if(_depth_func((tempZ >> 15), tempDepthPtr[((tempY >> 16) * _fgl_draw_buffer->width) + tempX]))
				#endif
				{
					#ifdef _vertex_color
						inColor = (((tempR >> 5) & 0xF800) | ((tempG >> 11) & 0x07E0) | (tempB >> 16));
					#endif
					#ifdef _texture
						tempTexel = inTexture->data[((tempV >> 16) * inTexture->width) + (tempU >> 16)];
						#if (defined(_vertex_color) || defined(_polygon_color))
							tempTexel = fgl_color_multiply(tempTexel, inColor);
						#endif
						tempBuffer[((tempY >> 16) * _fgl_draw_buffer->width) + tempX] = tempTexel;
					#else
						tempBuffer[((tempY >> 16) * _fgl_draw_buffer->width) + tempX] = inColor;
					#endif
					#ifdef _depth_write
						tempDepthPtr[((tempY >> 16) * _fgl_draw_buffer->width) + tempX] = (tempZ >> 15);
					#endif
				}
			}

			#ifdef _depth_interp
			tempZ += tempDz;
			#endif
			#ifdef _texture
			tempU += tempDu;
			tempV += tempDv;
			#endif
			#ifdef _vertex_color
			tempR += tempDr;
			tempG += tempDg;
			tempB += tempDb;
			#endif
		}
	}
}



#undef _depth_interp
#undef _depth_func
#undef inColor

#undef _fgl_draw_line
#undef _depth_write
#undef _depth_test
#undef _polygon_color
#undef _vertex_color
#undef _texture
#undef _texture_perspective
