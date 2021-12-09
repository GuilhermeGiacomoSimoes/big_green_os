void main() {
	char* letter_g = (char*) 0xb8000;
	*letter_g = 'G';

	char* letter_u = (char*) 0xb8002;
	*letter_u = 'U';

	char* letter_i = (char*) 0xb8004;
	*letter_i = 'I';

	char* letter_s = (char*) 0xb8006;
	*letter_s = ' ';

}
