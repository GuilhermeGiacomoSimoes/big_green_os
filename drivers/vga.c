#include "vga.h"
#include "../lib/memory.h"

//This register contains a value of offset cursor
#define VGA_OFFSET_REGISTER 0x3c4

//This register contains a data value of 
//current address cursor
#define VGA_DATA_REGISTER 0x3c5

#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e

// this address is address that the video memory start
#define VIDEO_ADDRESS 0xb8000

#define MAX_ROWS 25
#define MAX_COLS 80

//cursor color
#define WHITE_ON_BLACK 0x0f

static unsigned char port_byte_in(unsigned short port)
{
    unsigned char result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

static void port_byte_out(unsigned short port, unsigned char data)
{
	__asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

static void set_cursor(int offset)
{
	offset /= 2;
	port_byte_out(VGA_OFFSET_REGISTER, (unsigned char) (offset));
}

static int get_cursor()
{
    int offset = port_byte_in(VGA_OFFSET_REGISTER);
    return offset * 2;
}

static void set_char_at_video_memory(
		const char character, const int offset)
{
	unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
	vidmem[offset] = character;
	vidmem[offset + 1] = WHITE_ON_BLACK;
}

static int get_row_from_offset(int offset)
{
	return offset / (2 * MAX_COLS);
}

static int get_offset(int col, int row)
{
	return 2 * (row * MAX_COLS + col);
}

static int move_offset_to_new_line(int offset)
{
	return get_offset(0, get_row_from_offset(offset) + 1);
}

static int scroll_ln(int offset)
{
	k_memcpy(
			(char *) (get_offset(0, 1) + VIDEO_ADDRESS),
			(char *) (get_offset(0, 0) + VIDEO_ADDRESS),
			(int) MAX_COLS * (MAX_ROWS - 1) * 2
		);

	for(int col = 0; col < MAX_COLS; col++) 
		set_char_at_video_memory(' ', get_offset(col, MAX_ROWS - 1));

	return offset - 2 * MAX_COLS;
}


void clear_screen() 
{
	for(int i = 0; i < MAX_COLS * MAX_ROWS; ++i)
		set_char_at_video_memory(' ', i * 2);

	set_cursor(get_offset(0, 0));
}

void print_string(char *str)
{
	int offset = get_cursor();
	for(int i =0; str[i] != 0; i++) {
		if(offset >= MAX_ROWS * MAX_COLS * 2) 
			offset = scroll_ln(offset);
		if(str[i] == '\n') 
			offset = move_offset_to_new_line(offset);
		else {
			set_char_at_video_memory(str[i], offset);
			offset += 2;
		}
	}

	set_cursor(offset);
}

