/*
 x86 REAL MODE MEMORY MAP

0x00000000 - 0x000003FF - Real Mode Interrupt Vector Table
0x00000400 - 0x000004FF - BIOS Data Area
0x00000500 - 0x00007BFF - Unused
0x00007C00 - 0x00007DFF - Our Bootloader
0x00007E00 - 0x0009FFFF - Unused
0x000A0000 - 0x000BFFFF - Video RAM (VRAM) Memory
0x000B0000 - 0x000B7777 - Monochrome Video Memory
0x000B8000 - 0x000BFFFF - Color Video Memory
0x000C0000 - 0x000C7FFF - Video ROM BIOS
0x000C8000 - 0x000EFFFF - BIOS Shadow Area
0x000F0000 - 0x000FFFFF - System BIOS
 */

#include "vga.h"
#include "../lib/memory.h"
#include "helper.h"

//This register contains a value of offset cursor
#define VGA_OFFSET_REGISTER 0x3c4

//This register contains a data value of 
//current address cursor
#define VGA_DATA_REGISTER 0x3c5

//VGA register controllers
#define VGA_CTRL_REG 0x3c4
#define VGA_CURSOR_HIGH 0x0e
#define VGA_CURSOR_LOW 0x0f

#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e

// this address is address that the video memory start
#define VIDEO_ADDRESS 0xb8000


#define MAX_ROWS 25
#define MAX_COLS 80

//cursor color
#define WHITE_ON_BLACK 0x0f

static void set_cursor(int offset)
{
	offset /= 2;
	port_byte_out(VGA_CTRL_REG, VGA_CURSOR_HIGH);
    port_byte_out(VGA_OFFSET_REGISTER, (offset >> 8) & 0xff);
    port_byte_out(VGA_CTRL_REG, VGA_CURSOR_LOW);
    port_byte_out(VGA_OFFSET_REGISTER, offset & 0xFF);
}

static int get_cursor()
{
	port_byte_out(VGA_CTRL_REG, VGA_CURSOR_HIGH);
    int offset = (port_byte_in(VGA_OFFSET_REGISTER) << 8) & 0xff;
	port_byte_out(VGA_CTRL_REG, VGA_CURSOR_LOW);
	offset += port_byte_in(VGA_DATA_REGISTER);

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

	for(int i = 0; str[i] != 0; i++) {
        if (offset >= MAX_ROWS * MAX_COLS * 2) {
            offset = scroll_ln(offset);
        }
        if (str[i] == '\n') {
            offset = move_offset_to_new_line(offset);
        } else {
            set_char_at_video_memory(str[i], offset);
            offset += 2;
        }
    }
    set_cursor(offset);
}


