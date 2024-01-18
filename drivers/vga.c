#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f

unsigned char port_byte_in(unsigned short port)
{
	unsigned char result;
	__asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
	return result;
}

void port_byte_out(unsigned short port, unsigned char data)
{
	__asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

void set_cursor(int offset)
{
	offset /= 2;
	port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
	port_byte_out(VGA_DATA_REGISTER, (unsigned char) (offset >> 8));
	port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
	port_byte_out(VGA_DATA_REGISTER, (unsigned char) (offset & 0xff));
}

int get_cursor()
{
	port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
	int offset = port_byte_in(VGA_DATA_REGISTER) << 8;
	port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
	offset += port_byte_in(VGA_DATA_REGISTER);
	return offset * 2;
}

void set_char_at_video_memory(char character, int offset)
{
	unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
	vidmem[offset] = character;
	vidmem[offset + 1] = WHITE_ON_BLACK;
}

int get_row_from_offset(int offset)
{
	return offset / (2 * MAX_COLS);
}

int get_offset(int col, int row)
{
	return 2 * (row * MAX_COLS + col);
}

int move_offset_to_new_line(int offset)
{
	return get_offset(0, get_row_from_offset(offset) + 1);
}

void print_string(char *str)
{
	int offset = get_cursor();
	for(int i =0; str[i] != 0; i++) {
		if(str[i] == '\n') 
			offset = move_offset_to_new_line(offset);
		else {
			offset += 2;
			set_char_at_video_memory(str[i], offset);
		}
	}

	set_cursor(offset);
}
