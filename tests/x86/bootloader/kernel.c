void main()
{
	char msg[] = {'B', 'O', 'O', 'T', ' ', 'I', 'S', ' ', 'W', 'O', 'R', 'K', 0};

	int size = 0;
	for( ; *(msg + size) != '\0' ; ++ size );

	unsigned char * vid_mem = (unsigned char *) 0xb8000;

	for (int i = 0 ; i < size ; i ++ ) {
		*(vid_mem + i * 2) = msg[i];
	}
}
