#ifndef __STRING_H__
#define __STRING_H__

inline void strcpy(char *dest, const char *source)
{
	for(
		int i = 0; 
		*(dest + i) != '\0'; 
		i++, *(dest + i) = *(source + i)
	);
}

#endif
