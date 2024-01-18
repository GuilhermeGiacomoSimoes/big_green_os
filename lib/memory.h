#ifndef __MEMORY_H__
#define __MEMORY_H__

inline void memcpy(char* source, char*dest, int nbytes)
{
	for(
		int i = 0; 
		i < nbytes; 
		i++, *(dest + i) = *(source + i)
	);
}


#endif
