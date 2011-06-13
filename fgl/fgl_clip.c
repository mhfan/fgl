#include "fgl_clip.h"
#include "fgl_color.h"

// TODO - Fix triangle clipping to separate x, y, and z clipping
// TODO - Interpolate verteces with respect to x, y & z not just x, y or z.
// TODO - Fix triangle clipping situations where V0.x > V1.x or V0.z > V1.z.

fix16_t _fgl_clip_width    = (320 << 16);
fix16_t _fgl_clip_height   = (240 << 16);
fix16_t _fgl_clip_depth    = (1 << 16);

extern bool _fgl_cull_face_enable;



bool _fgl_triangle_winding (fgl_vertex_t* inV) {
	fix16_t tempUX = inV[0].x - inV[2].x;
	fix16_t tempUY = inV[0].y - inV[2].y;
	fix16_t tempVX = inV[1].x - inV[2].x;
	fix16_t tempVY = inV[1].y - inV[2].y;
	int64_t tempWind = ((int64_t)tempUX * (int64_t)tempVY) - ((int64_t)tempUY * (int64_t)tempVX);
	return (tempWind >= 0);
}



int fgl_clip_point(fgl_vertex_t* inV, fgl_vertex_t* outV) {
	if((inV[0].x < 0) || (inV[0].x >= _fgl_clip_width))
		return 0;
	if((inV[0].y < 0) || (inV[0].y >= _fgl_clip_height))
		return 0;
	if((inV[0].z < 0) || (inV[0].z >= _fgl_clip_depth))
		return 0;
	*outV = *inV;
	return 1;
}

int fgl_clip_line(fgl_vertex_t* inV0, fgl_vertex_t* inV1, fgl_vertex_t* outV) {
	// Assumes inV[n].x,y,z are transformed to screen coords.
	// Assumes inV0->y <= inV1->y

	fgl_vertex_t tempV[2] = { *inV0, *inV1 };

	if(tempV[0].x > tempV[1].x) {
		if(tempV[0].x < 0) {
			if(tempV[1].x < 0)
				return 0;
			fgl_clip_line_x(&tempV[0], &tempV[1], outV);
			tempV[0] = outV[0];
			tempV[1] = outV[1];
		} else if(tempV[1].x >= _fgl_clip_width) {
			if(tempV[0].x >= _fgl_clip_width)
				return 0;
			fgl_clip_line_x(&tempV[0], &tempV[1], outV);
			tempV[0] = outV[0];
			tempV[1] = outV[1];
		}
	} else {
		if(tempV[1].x < 0) {
			if(tempV[0].x < 0)
				return 0;
			fgl_clip_line_x(&tempV[1], &tempV[0], outV);
			tempV[0] = outV[0];
			tempV[1] = outV[1];
		} else if(tempV[0].x >= _fgl_clip_width) {
			if(tempV[1].x >= _fgl_clip_width)
				return 0;
			fgl_clip_line_x(&tempV[1], &tempV[0], outV);
			tempV[0] = outV[0];
			tempV[1] = outV[1];
		}
	}

	return 2;

	if(inV0->y < 0) {
		if(inV1->y < 0)
			return 0;
		fgl_clip_line_y(inV0, inV1, outV);
	} else if(inV1->y >= _fgl_clip_height) {
		if(inV0->y >= _fgl_clip_height)
			return 0;
		fgl_clip_line_y(inV0, inV1, outV);
	}

	if(inV0->z > inV1->z) {
		if(inV0->z < 0) {
			if(inV1->z < 0)
				return 0;
			fgl_clip_line_z(inV0, inV1, outV);
		} else if(inV1->z >= _fgl_clip_depth) {
			if(inV0->z >= _fgl_clip_depth)
				return 0;
			fgl_clip_line_z(inV0, inV1, outV);
		}
	} else {
		if(inV1->z < 0) {
			if(inV0->z < 0)
				return 0;
			fgl_clip_line_z(inV1, inV0, outV);
		} else if(inV0->z >= _fgl_clip_depth) {
			if(inV1->z >= _fgl_clip_depth)
				return 0;
			fgl_clip_line_z(inV1, inV0, outV);
		}
	}

	return 2;
}

int fgl_clip_triangle(fgl_vertex_t* inV, fgl_vertex_t* outV) {
	// Fast Off-screen clipping.
	if((inV[0].x < 0) && (inV[1].x < 0) && (inV[2].x < 0))
		return 0;
	if((inV[0].x >= _fgl_clip_width) && (inV[1].x >= _fgl_clip_width) && (inV[2].x >= _fgl_clip_width))
		return 0;
	if((inV[0].y < 0) && (inV[1].y < 0) && (inV[2].y < 0))
		return 0;
	if((inV[0].y >= _fgl_clip_height) && (inV[1].y >= _fgl_clip_height) && (inV[2].y >= _fgl_clip_height))
		return 0;
	if((inV[0].z < 0) && (inV[1].z < 0) && (inV[2].z < 0))
		return 0;
	if((inV[0].z >= _fgl_clip_depth) && (inV[1].z >= _fgl_clip_depth) && (inV[2].z >= _fgl_clip_depth))
		return 0;

	// Fast Degenerate clipping.
	if((inV[0].x == inV[1].x) && (inV[0].y == inV[1].y))
			return 0;
	if((inV[0].x == inV[2].x) && (inV[0].y == inV[2].y))
			return 0;
	if((inV[2].x == inV[1].x) && (inV[2].y == inV[1].y))
			return 0;

	// Backface culling.
	if(_fgl_cull_face_enable && _fgl_triangle_winding(inV))
		return 0;

	// Vertex output and re-ordering.
	if(inV[1].y < inV[0].y) {
		if(inV[2].y < inV[1].y) {
			outV[0] = inV[2];
			outV[1] = inV[1];
			outV[2] = inV[0];
		} else {
			outV[0] = inV[1];
			if(inV[2].y < inV[0].y) {
				outV[1] = inV[2];
				outV[2] = inV[0];
			} else {
				outV[1] = inV[0];
				outV[2] = inV[2];
			}
		}
	} else if(inV[2].y < inV[0].y) {
		outV[0] = inV[2];
		if(inV[1].y < inV[0].y) {
			outV[1] = inV[1];
			outV[2] = inV[0];
		} else {
			outV[1] = inV[0];
			outV[2] = inV[1];
		}
	} else {
		outV[0] = inV[0];
		if(inV[1].y < inV[2].y) {
			outV[1] = inV[1];
			outV[2] = inV[2];
		} else {
			outV[1] = inV[2];
			outV[2] = inV[1];
		}
	}

	// Thorough degenerate clipping.
	fix16_t tempDx[2];
	tempDx[0] = fix16_sdiv((inV[1].x - inV[0].x), (inV[1].y - inV[0].y));
	tempDx[1] = fix16_sdiv((inV[2].x - inV[0].x), (inV[2].y - inV[0].y));
	if(((tempDx[0] - tempDx[1]) >> 12) == 0)
		return 0;

	return 3;
}

int fgl_clip_sprite(fgl_vertex_t* inV, fgl_vertex_t* outV) {
	// Fast Off-screen clipping.
	if((inV[0].x < 0) && (inV[1].x < 0))
		return 0;
	if((inV[0].x >= _fgl_clip_width) && (inV[1].x >= _fgl_clip_width))
		return 0;
	if((inV[0].y < 0) && (inV[1].y < 0))
		return 0;
	if((inV[0].y >= _fgl_clip_height) && (inV[1].y >= _fgl_clip_height))
		return 0;

	// Degenerate clipping.
	if((inV[0].x == inV[1].x) || (inV[0].y == inV[1].y))
			return 0;

	// Z-Clipping
	if(inV[0].z != inV[1].z) {
		inV[0].z = (((int64_t)inV[0].z + (int64_t)inV[1].z) >> 1);
		inV[1].z = inV[0].z;
	}
	if(inV[0].z < 0)
		return 0;
	if(inV[0].z >= _fgl_clip_depth)
		return 0;

	// Vertex output and re-ordering.
	if((inV[0].y > inV[1].y)) {
		outV[0] = inV[1];
		outV[1] = inV[0];
	} else {
		outV[0] = inV[0];
		outV[1] = inV[1];
	}

	// Backface culling.
	if(_fgl_cull_face_enable && (outV[0].x > outV[1].x))
		return 0;

	// Horizontal clipping
	if((outV[0].x < 0) || (outV[1].x >= _fgl_clip_width)) {
		fix16_t tempDx, tempDu;
		tempDx = (outV[1].x - outV[0].x);
		tempDu = (outV[1].u - outV[0].u) / tempDx;
		if(outV[0].x < 0) {
			outV[0].u += (-outV[0].x * tempDu);
			outV[0].x  = 0;
		}
		if(outV[1].x >= _fgl_clip_width) {
			outV[1].u -= ((outV[1].x - (_fgl_clip_width - 1)) * tempDu);
			outV[1].x  = (_fgl_clip_width - 1);
		}
	}

	// Vertical clipping
	if((outV[0].y < 0) || (outV[1].y >= _fgl_clip_height)) {
		fix16_t tempDy, tempDv;
		tempDy = (outV[1].y - outV[0].y);
		tempDv = (outV[1].v - outV[0].v) / tempDy;
		if(outV[0].x < 0) {
			outV[0].v += (-outV[0].y * tempDv);
			outV[0].y  = 0;
		}
		if(outV[1].y >= _fgl_clip_height) {
			outV[1].v -= ((outV[1].y - (_fgl_clip_height - 1)) * tempDv);
			outV[1].y  = (_fgl_clip_height - 1);
		}
	}

	return 2;
}



void fgl_clip_line_x(fgl_vertex_t* inV0, fgl_vertex_t* inV1, fgl_vertex_t* outV) {
	// Assumes inV0->x < inV1->x

	fix16_t tempDx, tempDy, tempDz;
	tempDx = (inV1->x - inV0->x);
	tempDy = fix16_div((inV1->y - inV0->y), tempDx);
	tempDz = fix16_div((inV1->z - inV0->z), tempDx);

	// TODO - Calculate tempDx, tempDy[, tempDz] magnitude for perspective correct interpolation.

	fix16_t tempDu, tempDv;
	tempDu = fix16_div((inV1->u - inV0->u), tempDx);
	tempDv = fix16_div((inV1->v - inV0->v), tempDx);

	fix16_t tempR0, tempG0, tempB0;
	fix16_t tempR1, tempG1, tempB1;
	fix16_t tempDr, tempDg, tempDb;

	bool tempCDiff = (inV0->c != inV1->c);
	if(tempCDiff) {
		tempR0 = fgl_color_red(inV0->c)   << 16;
		tempG0 = fgl_color_green(inV0->c) << 16;
		tempB0 = fgl_color_blue(inV0->c)  << 16;
		tempR1 = fgl_color_red(inV1->c)   << 16;
		tempG1 = fgl_color_green(inV1->c) << 16;
		tempB1 = fgl_color_blue(inV1->c)  << 16;
		tempDr = fix16_div((tempR1 - tempR0), tempDx);
		tempDg = fix16_div((tempG1 - tempG0), tempDx);
		tempDb = fix16_div((tempB1 - tempB0), tempDx);
	}

	if(inV0->x < 0) {
		outV[0].x  = 0;
		tempDx = (outV[0].x - inV0->x);

		outV[0].y  = (inV0->y + fix16_mul(tempDy, tempDx));
		outV[0].z  = (inV0->z + fix16_mul(tempDz, tempDx));
		outV[0].u  = (inV0->z + fix16_mul(tempDu, tempDx));
		outV[0].v  = (inV0->z + fix16_mul(tempDv, tempDx));

		if(tempCDiff) {
			tempR0 += fix16_mul(tempDr, tempDx);
			tempG0 += fix16_mul(tempDg, tempDx);
			tempB0 += fix16_mul(tempDb, tempDx);
			outV[0].c = fgl_color_rgb((tempR0 >> 16), (tempG0 >> 16), (tempB0 >> 16));
		}
	}

	if(inV1->x >= _fgl_clip_width) {
		outV[1].x  = (_fgl_clip_width - 1);
		tempDx = (inV1->x - outV[1].x);

		outV[1].y  = (inV1->y - fix16_mul(tempDy, tempDx));
		outV[1].z  = (inV1->z - fix16_mul(tempDz, tempDx));
		outV[1].u  = (inV1->z - fix16_mul(tempDu, tempDx));
		outV[1].v  = (inV1->z - fix16_mul(tempDv, tempDx));

		if(tempCDiff) {
			tempR1 -= fix16_mul(tempDr, tempDx);
			tempG1 -= fix16_mul(tempDg, tempDx);
			tempB1 -= fix16_mul(tempDb, tempDx);
			outV[1].c = fgl_color_rgb((tempR1 >> 16), (tempG1 >> 16), (tempB1 >> 16));
		}
	}
}

void fgl_clip_line_y(fgl_vertex_t* inV0, fgl_vertex_t* inV1, fgl_vertex_t* outV) {
	// Assumes inV0->y < inV1->y

	outV[0] = *inV0;
	outV[1] = *inV1;
	return;

	fix16_t tempDx, tempDy, tempDz;
	tempDy = (inV1->y - inV0->y);
	tempDx = ((inV1->x - inV0->x) << 16) / tempDy;
	tempDz = ((inV1->z - inV0->z) << 16) / tempDy;

	fix16_t tempDu, tempDv;
	tempDu = (inV1->u - inV0->u) / tempDy;
	tempDv = (inV1->v - inV0->v) / tempDy;

	fix16_t tempR0, tempG0, tempB0;
	fix16_t tempR1, tempG1, tempB1;
	fix16_t tempDr, tempDg, tempDb;
	if(inV0->c != inV1->c) {
		tempR0 = fgl_color_red(inV0->c);
		tempG0 = fgl_color_green(inV0->c);
		tempB0 = fgl_color_blue(inV0->c);
		tempR1 = fgl_color_red(inV1->c);
		tempG1 = fgl_color_green(inV1->c);
		tempB1 = fgl_color_blue(inV1->c);
		tempDr = ((tempR1 - tempR0) << 16) / tempDy;
		tempDg = ((tempG1 - tempG0) << 16) / tempDy;
		tempDb = ((tempB1 - tempB0) << 16) / tempDy;
	}

	if(outV[0].y < 0) {
		outV[0].y  = -inV0->y;
		outV[0].x  = ((outV[0].x << 16) + (tempDx * outV[0].y)) >> 16;
		outV[0].z  = ((outV[0].z << 16) + (tempDz * outV[0].y)) >> 16;
		outV[0].u += (tempDu * outV[0].y);
		outV[0].v += (tempDv * outV[0].y);

		if(outV[0].c != inV1->c) {
			tempR0 = ((tempR0 << 16) + (tempDr * outV[0].y)) >> 16;
			tempG0 = ((tempG0 << 16) + (tempDg * outV[0].y)) >> 16;
			tempB0 = ((tempB0 << 16) + (tempDb * outV[0].y)) >> 16;
			outV[0].c = fgl_color_rgb(tempR0, tempG0, tempB0);
		}

		outV[0].y = 0;
	}

	if(outV[1].y >= _fgl_clip_height) {
		outV[1].y  = (_fgl_clip_height - inV1->y) + 1;
		outV[1].x  = ((outV[1].x << 16) - (tempDx * outV[1].y)) >> 16;
		outV[1].z  = ((outV[1].z << 16) - (tempDz * outV[1].y)) >> 16;
		outV[1].u -= (tempDu * outV[1].y);
		outV[1].v -= (tempDv * outV[1].y);

		if(inV0->c != outV[1].c) {
			tempR1 = ((tempR1 << 16) - (tempDr * outV[1].y)) >> 16;
			tempG1 = ((tempG1 << 16) - (tempDg * outV[1].y)) >> 16;
			tempB1 = ((tempB1 << 16) - (tempDb * outV[1].y)) >> 16;
			outV[1].c = fgl_color_rgb(tempR1, tempG1, tempB1);
		}

		outV[1].y = (_fgl_clip_height - 1);
	}
}


void fgl_clip_line_z(fgl_vertex_t* inV0, fgl_vertex_t* inV1, fgl_vertex_t* outV) {
	// Assumes inV0->z < inV1->z

	outV[0] = *inV0;
	outV[1] = *inV1;
	return;

	fix16_t tempDx, tempDy, tempDz;
	tempDz = (inV1->z - inV0->z);
	tempDx = ((inV1->x - inV0->x) << 16) / tempDz;
	tempDy = ((inV1->y - inV0->y) << 16) / tempDz;

	fix16_t tempDu, tempDv;
	tempDu = (inV1->u - inV0->u) / tempDz;
	tempDv = (inV1->v - inV0->v) / tempDz;

	fix16_t tempR0, tempG0, tempB0;
	fix16_t tempR1, tempG1, tempB1;
	fix16_t tempDr, tempDg, tempDb;
	if(inV0->c != inV1->c) {
		tempR0 = fgl_color_red(inV0->c);
		tempG0 = fgl_color_green(inV0->c);
		tempB0 = fgl_color_blue(inV0->c);
		tempR1 = fgl_color_red(inV1->c);
		tempG1 = fgl_color_green(inV1->c);
		tempB1 = fgl_color_blue(inV1->c);
		tempDr = ((tempR1 - tempR0) << 16) / tempDz;
		tempDg = ((tempG1 - tempG0) << 16) / tempDz;
		tempDb = ((tempB1 - tempB0) << 16) / tempDz;
	}

	if(outV[0].z < 0) {
		outV[0].z  = -inV0->z;
		outV[0].x  = ((outV[0].x << 16) + (tempDx * outV[0].z)) >> 16;
		outV[0].y  = ((outV[0].y << 16) + (tempDy * outV[0].z)) >> 16;
		outV[0].u += (tempDu * outV[0].z);
		outV[0].v += (tempDv * outV[0].z);

		if(outV[0].c != inV1->c) {
			tempR0 = ((tempR0 << 16) + (tempDr * outV[0].z)) >> 16;
			tempG0 = ((tempG0 << 16) + (tempDg * outV[0].z)) >> 16;
			tempB0 = ((tempB0 << 16) + (tempDb * outV[0].z)) >> 16;
			outV[0].c = fgl_color_rgb(tempR0, tempG0, tempB0);
		}

		outV[0].z = 0;
	}

	if(outV[1].z >= _fgl_clip_depth) {
		outV[1].z  = (_fgl_clip_depth - inV1->z) + 1;
		outV[1].x  = ((outV[1].x << 16) - (tempDx * outV[1].z)) >> 16;
		outV[1].y  = ((outV[1].y << 16) - (tempDz * outV[1].z)) >> 16;
		outV[1].u -= (tempDu * outV[1].z);
		outV[1].v -= (tempDv * outV[1].z);

		if(inV0->c != outV[1].c) {
			tempR1 = ((tempR1 << 16) - (tempDr * outV[1].z)) >> 16;
			tempG1 = ((tempG1 << 16) - (tempDg * outV[1].z)) >> 16;
			tempB1 = ((tempB1 << 16) - (tempDb * outV[1].z)) >> 16;
			outV[1].c = fgl_color_rgb(tempR1, tempG1, tempB1);
		}

		outV[1].z = (_fgl_clip_depth - 1);
	}
}
