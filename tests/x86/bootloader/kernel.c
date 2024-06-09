int str_size(char *str) {
	int size = 0;
	for( ; *(str + size) != '\0' ; ++ size );
	return size;
}

void print_str(char *str)
{
	int size = str_size(str);

	unsigned char * vid_mem = (unsigned char *) 0xb8000;

	for (int i = 0 ; i < size ; i ++ ) {
		*(vid_mem + i * 2) = str[i];
	}
}

void main()
{
	char msg[] = {'B', 'O', 'O', 'T', ' ', 'I', 'S', ' ', 'W', 'O', 'R', 'K', 0};
	print_str(msg);
}
