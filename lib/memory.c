#include "memory.h"

void k_memcpy(const char* source, char*dest, const int nbytes)
{
	for(
		int i = 0; 
		i < nbytes; 
		i++, *(dest + i) = *(source + i)
	);
}

