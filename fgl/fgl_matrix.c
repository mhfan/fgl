#include "fgl_matrix.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>



#define _fgl_matrix_identity { 0x00010000, 0, 0, 0, 0, 0x00010000, 0, 0, 0, 0, 0x00010000, 0, 0, 0, 0, 0x00010000 }

fix16_t   _fgl_matrix_current[6][16] = { _fgl_matrix_identity, _fgl_matrix_identity, _fgl_matrix_identity, _fgl_matrix_identity, _fgl_matrix_identity, _fgl_matrix_identity };
fix16_t*  _fgl_matrix_stack[6]       = { NULL, NULL, NULL, NULL, NULL, NULL };
uintptr_t _fgl_matrix_stack_depth[6] = { 0, 0, 0, 0, 0, 0 };
uintptr_t _fgl_matrix_stack_ptr[6]   = { 0, 0, 0, 0, 0, 0 };
bool      _fgl_matrix_change[6]      = { false, false, false, false, false, false };
uint8_t   _fgl_matrix_mode           = FGL_MATRIX_PROJECTION;

fix16_t _fgl_matrix_modelview[16]  = _fgl_matrix_identity;
fix16_t _fgl_matrix_projviewp[16]  = _fgl_matrix_identity;
fix16_t _fgl_matrix_transform[16]  = _fgl_matrix_identity;



void fgl_matrix_print(fix16_t* inMatrix) {
	uintptr_t i;
	printf("{ ");
	for(i = 0; i < 16; i++) {
		if(!(i & 3) && i)
			printf("\n  ");
		printf("%+.4f ", fix16_to_dbl(inMatrix[i]));
	}
	printf(" }\n");
}

void fgl_matrix_copy(fix16_t* outMatrix, fix16_t* inMatrix) {
	uintptr_t i;
	for(i = 0; i < 16; i++)
		outMatrix[i] = inMatrix[i];
}

void fgl_matrix_transpose(fix16_t* outMatrix, fix16_t* inMatrix) {
	if(outMatrix == inMatrix) {
		fix16_t tempMatrix[16];
		fgl_matrix_transpose(tempMatrix, inMatrix);
		fgl_matrix_copy(outMatrix, tempMatrix);
		return;
	}
	uintptr_t i, j;
	for(j = 0; j < 4; j++) {
		for(i = 0; i < 4; i++)
			outMatrix[(j << 2) + i] = inMatrix[(i << 2) + j];
	}
}

void fgl_matrix_multiply(fix16_t* outMatrix, fix16_t* inMatrix0, fix16_t* inMatrix1) {
	if((outMatrix == inMatrix0) || (outMatrix == inMatrix1)) {
		fix16_t tempMatrix[16];
		fgl_matrix_multiply(tempMatrix, inMatrix0, inMatrix1);
		fgl_matrix_copy(outMatrix, tempMatrix);
		return;
	}
	uint8_t i, j, k;
	for(j = 0; j < 4; j++) {
		for(i = 0; i < 4; i++) {
			outMatrix[(j << 2) + i] = 0;
			for(k = 0; k < 4; k++)
				outMatrix[(j << 2) + i] += fix16_mul(inMatrix0[(k << 2) + i], inMatrix1[(j << 2) + k]);
		}
	}
}



void _fgl_matrix_transform_update() {
	bool tempModelView = (_fgl_matrix_change[FGL_MATRIX_MODEL] || _fgl_matrix_change[FGL_MATRIX_VIEW]);
	bool tempProjViewP = (_fgl_matrix_change[FGL_MATRIX_PROJECTION] || _fgl_matrix_change[FGL_MATRIX_VIEWPORT]);
	if(tempModelView || tempProjViewP) {
		if(tempModelView) {
			fgl_matrix_multiply(_fgl_matrix_modelview, _fgl_matrix_current[FGL_MATRIX_MODEL], _fgl_matrix_current[FGL_MATRIX_VIEW]);
			_fgl_matrix_change[FGL_MATRIX_MODEL] = false;
			_fgl_matrix_change[FGL_MATRIX_VIEW] = false;
		}
		if(tempProjViewP) {
			fgl_matrix_multiply(_fgl_matrix_projviewp, _fgl_matrix_current[FGL_MATRIX_PROJECTION], _fgl_matrix_current[FGL_MATRIX_VIEWPORT]);
			_fgl_matrix_change[FGL_MATRIX_PROJECTION] = false;
			_fgl_matrix_change[FGL_MATRIX_VIEWPORT] = false;
		}
		fgl_matrix_multiply(_fgl_matrix_transform, _fgl_matrix_modelview, _fgl_matrix_projviewp);
	} else
		return;

	/*printf("model\n");
	fgl_matrix_print(_fgl_matrix_current[FGL_MATRIX_MODEL]);
	printf("\n");
	printf("view\n");
	fgl_matrix_print(_fgl_matrix_current[FGL_MATRIX_VIEW]);
	printf("\n");
	printf("projection\n");
	fgl_matrix_print(_fgl_matrix_current[FGL_MATRIX_PROJECTION]);
	printf("\n");
	printf("transform\n");
	fgl_matrix_print(_fgl_matrix_transform);
	printf("\n");*/
}



void fgl_matrix_mode_set(uint8_t inMode) {
	if(inMode > FGL_MATRIX_LAST)
		return;
	_fgl_matrix_mode = inMode;
}

uint8_t fgl_matrix_mode_get() {
	return _fgl_matrix_mode;
}



void fgl_matrix_identity() {
	_fgl_matrix_current[_fgl_matrix_mode][ 0] = fix16_one;
	_fgl_matrix_current[_fgl_matrix_mode][ 1] = 0;
	_fgl_matrix_current[_fgl_matrix_mode][ 2] = 0;
	_fgl_matrix_current[_fgl_matrix_mode][ 3] = 0;

	_fgl_matrix_current[_fgl_matrix_mode][ 4] = 0;
	_fgl_matrix_current[_fgl_matrix_mode][ 5] = fix16_one;
	_fgl_matrix_current[_fgl_matrix_mode][ 6] = 0;
	_fgl_matrix_current[_fgl_matrix_mode][ 7] = 0;

	_fgl_matrix_current[_fgl_matrix_mode][ 8] = 0;
	_fgl_matrix_current[_fgl_matrix_mode][ 9] = 0;
	_fgl_matrix_current[_fgl_matrix_mode][10] = fix16_one;
	_fgl_matrix_current[_fgl_matrix_mode][11] = 0;

	_fgl_matrix_current[_fgl_matrix_mode][12] = 0;
	_fgl_matrix_current[_fgl_matrix_mode][13] = 0;
	_fgl_matrix_current[_fgl_matrix_mode][14] = 0;
	_fgl_matrix_current[_fgl_matrix_mode][15] = fix16_one;

	_fgl_matrix_change[_fgl_matrix_mode] = true;
}

void fgl_matrix_push() {
	if(_fgl_matrix_stack_depth[_fgl_matrix_mode] == 0) {
		_fgl_matrix_stack[_fgl_matrix_mode] = (fix16_t*)malloc(64 * sizeof(fix16_t));
		if(_fgl_matrix_stack[_fgl_matrix_mode] == NULL) {
			printf("FGL_ERROR: Can't allocate matrix stack, out of memory."); // TODO - Proper error.
			return;
		}
		_fgl_matrix_stack_depth[_fgl_matrix_mode] = 64;
	} else if(_fgl_matrix_stack_depth[_fgl_matrix_mode] <= _fgl_matrix_stack_ptr[_fgl_matrix_mode]) {
		fix16_t* tempRealloc = (fix16_t*)realloc(_fgl_matrix_stack[_fgl_matrix_mode], (_fgl_matrix_stack_depth[_fgl_matrix_mode] << 1) * sizeof(fix16_t));
		if(tempRealloc == NULL) {
			printf("FGL_ERROR: Can't resize matrix stack, out of memory."); // TODO - Proper error.
			return;
		}
		_fgl_matrix_stack[_fgl_matrix_mode] = tempRealloc;
		_fgl_matrix_stack_depth[_fgl_matrix_mode] <<= 1;
	}
	fgl_matrix_copy(&_fgl_matrix_stack[_fgl_matrix_mode][_fgl_matrix_stack_ptr[_fgl_matrix_mode]], _fgl_matrix_current[_fgl_matrix_mode]);
	_fgl_matrix_stack_ptr[_fgl_matrix_mode] += 16;
}

void fgl_matrix_pop() {
	if(_fgl_matrix_stack_ptr[_fgl_matrix_mode] == 0) {
		fgl_matrix_identity();
		printf("FGL_ERROR: Matrix stack underflow, loading identity."); // TODO - Proper warning.
		return;
	}
	_fgl_matrix_stack_ptr[_fgl_matrix_mode] -= 16;
	fgl_matrix_copy(_fgl_matrix_current[_fgl_matrix_mode], &_fgl_matrix_stack[_fgl_matrix_mode][_fgl_matrix_stack_ptr[_fgl_matrix_mode]]);
	_fgl_matrix_change[_fgl_matrix_mode] = true;
}



void fgl_matrix_get(fix16_t* outMatrix) {
	fgl_matrix_copy(outMatrix, _fgl_matrix_current[_fgl_matrix_mode]);
}

void fgl_matrix_set(fix16_t* inMatrix) {
	fgl_matrix_copy(_fgl_matrix_current[_fgl_matrix_mode], inMatrix);
	_fgl_matrix_change[_fgl_matrix_mode] = true;
}

void fgl_matrix_mult(fix16_t* inMatrix) {
	fgl_matrix_multiply(_fgl_matrix_current[_fgl_matrix_mode], _fgl_matrix_current[_fgl_matrix_mode], inMatrix);
	_fgl_matrix_change[_fgl_matrix_mode] = true;
}
