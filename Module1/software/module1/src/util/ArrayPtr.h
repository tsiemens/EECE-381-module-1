/*
 * array_ptr.h
 *
 */

#ifndef ARRAYPTR_H_
#define ARRAYPTR_H_

typedef struct ShortIntPtr {
	short int* data;
	int size;
} ShortIntPtr;

void ShortIntPtr_free(ShortIntPtr* this);

void ShortToIntCPY(short int* sibuf, unsigned int* uibuf, int size);

#endif
