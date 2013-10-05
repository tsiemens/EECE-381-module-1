/*
 * ArrayPtr.c
 *
 */

#include <stdlib.h>
#include "ArrayPtr.h"

void ShortIntPtr_free(ShortIntPtr* this) {
	free(this->data);
	free(this);
}

void ShortToIntCPY(short int* sibuf, unsigned int* uibuf, int size) {
	int i = 0;
	for (i = 0; i < size; i++) {
		uibuf[i] = sibuf[i];
	}
}
