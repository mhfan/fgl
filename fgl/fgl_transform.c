#include "fgl_transform.h"
#include <stdio.h>

#include "fgl_matrix.h"



fgl_vertex_t fgl_transform_vertex(fgl_vertex_t inVertex, fix16_t* inMatrix) {
	fgl_vertex_t tempOut = inVertex;

	tempOut.x  = fix16_mul(inVertex.x, inMatrix[0]);
	tempOut.x += fix16_mul(inVertex.y, inMatrix[1]);
	tempOut.x += fix16_mul(inVertex.z, inMatrix[2]);
	tempOut.x += inMatrix[3];

	tempOut.y  = fix16_mul(inVertex.x, inMatrix[4]);
	tempOut.y += fix16_mul(inVertex.y, inMatrix[5]);
	tempOut.y += fix16_mul(inVertex.z, inMatrix[6]);
	tempOut.y += inMatrix[7];

	tempOut.z  = fix16_mul(inVertex.x, inMatrix[8]);
	tempOut.z += fix16_mul(inVertex.y, inMatrix[9]);
	tempOut.z += fix16_mul(inVertex.z, inMatrix[10]);
	tempOut.z += inMatrix[11];

	// Perspective division.
	fix16_t tempW;
	tempW  = fix16_mul(inVertex.x, inMatrix[12]);
	tempW += fix16_mul(inVertex.y, inMatrix[13]);
	tempW += fix16_mul(inVertex.z, inMatrix[14]);
	tempW += inMatrix[15];
	tempW = fix16_sdiv(fix16_one, tempW); // TODO - Check for divide by zero.
	tempOut.x = fix16_mul(tempOut.x, tempW);
	tempOut.y = fix16_mul(tempOut.y, tempW);
	tempOut.z = fix16_mul(tempOut.z, tempW);

	return tempOut;
}



void fgl_translate(fix16_t inX, fix16_t inY, fix16_t inZ) {
	fix16_t tempMatrix[16] = {
		fix16_one, 0, 0, inX,
		0, fix16_one, 0, inY,
		0, 0, fix16_one, inZ,
		0, 0, 0, fix16_one
	};
	fgl_matrix_mult(tempMatrix);
}

void fgl_scale(fix16_t inX, fix16_t inY, fix16_t inZ) {
	fix16_t tempMatrix[16] = {
		inX, 0, 0, 0,
		0, inY, 0, 0,
		0, 0, inZ, 0,
		0, 0, 0, fix16_one
	};
	fgl_matrix_mult(tempMatrix);
}


void fgl_rotate_x(fix16_t inAngle) {
	fix16_t tempSin = fix16_sin(inAngle);
	fix16_t tempCos = fix16_cos(inAngle);
	fix16_t tempMatrix[16] = {
		fix16_one, 0, 0, 0,
		0, tempCos, -tempSin, 0,
		0, tempSin, tempCos, 0,
		0, 0, 0, fix16_one
	};
	fgl_matrix_mult(tempMatrix);
}

void fgl_rotate_y(fix16_t inAngle) {
	fix16_t tempSin = fix16_sin(inAngle);
	fix16_t tempCos = fix16_cos(inAngle);
	fix16_t tempMatrix[16] = {
		tempCos, 0, -tempSin, 0,
		0, fix16_one, 0, 0,
		tempSin, 0, tempCos, 0,
		0, 0, 0, fix16_one
	};
	fgl_matrix_mult(tempMatrix);
}

void fgl_rotate_z(fix16_t inAngle) {
	fix16_t tempSin = fix16_sin(inAngle);
	fix16_t tempCos = fix16_cos(inAngle);
	fix16_t tempMatrix[16] = {
		tempCos, -tempSin, 0, 0,
		tempSin, tempCos, 0, 0,
		0, 0, fix16_one, 0,
		0, 0, 0, fix16_one
	};
	fgl_matrix_mult(tempMatrix);
}

// TODO fgl_rotate



void fgl_ortho(fix16_t inLeft, fix16_t inRight, fix16_t inTop, fix16_t inBottom, fix16_t inNear, fix16_t inFar) {
	fix16_t tempMatrix[16] = {
		fix16_div(fix16_from_int(2), (inRight - inLeft)), 0, 0, -fix16_div((inRight + inLeft), (inRight - inLeft)),
		0, fix16_div(fix16_from_int(2), (inBottom - inTop)), 0, -fix16_div((inBottom + inTop), (inBottom - inTop)),
		0, 0, fix16_div(fix16_from_int(2), (inFar - inNear)), fix16_div((inFar + inNear), (inFar - inNear)),
		0, 0, 0, fix16_one
	};
	fgl_matrix_mult(tempMatrix);
}

void fgl_ortho_2d(fix16_t inLeft, fix16_t inRight, fix16_t inTop, fix16_t inBottom) {
		fgl_ortho(inLeft, inRight, inTop, inBottom, -fix16_one, fix16_one);
}

void fgl_frustum(fix16_t inLeft, fix16_t inRight, fix16_t inTop, fix16_t inBottom, fix16_t inNear, fix16_t inFar) {
	if((inNear <= 0) || (inFar <= 0)) {
		printf("FGL_ERROR: Near/Far plane must be above 0.\n");
		return;
	}
	fix16_t tempMatrix[16] = {
		fix16_div((inNear << 1), (inRight - inLeft)), 0, fix16_div((inRight + inLeft), (inRight - inLeft)), 0,
		0, fix16_div((inNear << 1), (inBottom - inTop)), fix16_div((inBottom + inTop), (inBottom - inTop)), 0,
		0, 0, fix16_div((inFar + inNear), (inFar - inNear)), -fix16_div((fix16_mul(inFar, inNear) << 1), (inFar - inNear)),
		0, 0, fix16_one, 0
	};
	fgl_matrix_mult(tempMatrix);
}

void fgl_perspective(fix16_t inFovY, fix16_t inAspect, fix16_t inNear, fix16_t inFar) {
	fix16_t tempTop    = fix16_mul(fix16_tan(inFovY >> 1), inNear);
	fix16_t tempBottom = -tempTop;
	fix16_t tempLeft   = fix16_mul(inAspect, tempTop);
	fix16_t tempRight  = -tempLeft;
	fgl_frustum(tempLeft, tempRight, tempTop, tempBottom, inNear, inFar);

	// TODO - Update below code to be correct for my top-left origin screen.
	/*fix16_t tempF = fix16_div(fix16_cos(inFovY >> 1), fix16_sin(inFovY >> 1));
	fix16_t tempMatrix[16] = {
		fix16_div(tempF, inAspect), 0, 0, 0,
		0, tempF, 0, 0,
		0, 0, fix16_div((inNear + inFar), (inNear - inFar)), fix16_div((fix16_mul(inNear, inFar) << 1), (inNear - inFar)),
		0, 0, -fix16_one, 0
	};
	fgl_matrix_mult(tempMatrix);*/
}



void fgl_viewport(fix16_t inX, fix16_t inY, fix16_t inWidth, fix16_t inHeight) {
	// TODO - Actually use inX, inY
	fgl_translate(fix16_one, fix16_one, fix16_one);
	fgl_scale((inWidth >> 1), (inHeight >> 1), (fix16_one >> 1));
}
