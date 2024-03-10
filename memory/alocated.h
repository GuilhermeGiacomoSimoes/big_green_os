#ifndef __ALOCATED_H__
#define __ALOCATED_H__

#include<stdint.h>
#include<stdio.h>

void *k_malloc(size_t size);
void k_free(void *p);

#endif
