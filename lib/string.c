#include "string.h"

int strlen(const char*str) 
{
	int count;
	for(count = 0; *(str + count) != '\0'; ++count);
	return count;
}

///
///static int count_c(const char* str, const char c) 
///{
///	int count = 0;
///	int str_len = strlen(str);
///	for(int i = 0; i < str_len; i++)
///		if(*(str + i) == c)
///			count ++;
///
///	return count;
///}
///
///static int bigger_str(const char* str, const char c)
///{
///	int bigger = 1;
///	int current_str = 0;
///	for(int i = 0; i < (int) strlen(str); i++) {
///		current_str ++;
///		if( *(str + i) == c
///			&& current_str > bigger 
///		) {
///			bigger = current_str;
///			current_str = 0;
///		}
///	}
///
///	return bigger;
///}
///
///void k_strok(const char* str, const char c, char** result)
///{
///	const int count = count_c(str, c);
///
///	int count_current_str = 0;
///	int count_current_c = 0;
///	char *current_str;
///	int bigger_string = bigger_str(str, c);
///	current_str = malloc(bigger_string);
///
///	const int str_len = strlen(str);
///	for(int i = 0 ; i < str_len; i ++) 
///	{
///		if(*(str + i) == c
///			|| i == str_len - 1 
///		) 
///		{
///			if(i == str_len - 1) 
///				*(current_str + count_current_str++) = *(str + i);
///
///			result[count_current_c] 
///				= (char *) malloc(count_current_str);
///
///			for(int j = 0; j < count_current_str; j++)
///				*(result[count_current_c] + j) 
///					= *(current_str + j);
///
///			count_current_c ++;
///			count_current_str = 0;
///		}
///		else
///			*(current_str + count_current_str++) = *(str + i);
///	}
///
///	free(current_str);
///}


void append(char s[], char n)
{
	int len = strlen(s);
	s[len] = n;
	s[len + 1] = '\0';
}


void strcpy(char *dest, const char *source)
{
	for(
		int i = 0; 
		*(dest + i) != '\0'; 
		i++, *(dest + i) = *(source + i)
	);
}

int strcmp(char s1[], char s2[])
{
	int i = 0;
	for(; s1[i] == s2[i]; i++)
		if(s1[i] == '\0') 
			return 0;

	return s1[i] - s2[i];
}
