#include "include/stdint.h"
#include "include/colors.h"
#include "include/console.h"
#include "include/ports.h"

static int cursor_row = 0;
static int cursor_col = 0;

static uint16_t cursor_position = 0; // Tracks current character cell (0 .. SCREEN_WIDTH*SCREEN_HEIGHT-1)
static uint16_t attribute = (0x0 /*blue background*/ << 4) | 0x7 /*white foreground*/;

#define VGA_PORT_COMMAND 0x3D4
#define VGA_PORT_DATA    0x3D5

void set_cursor(uint16_t pos) {
    outb(VGA_PORT_COMMAND, 0x0F);
    outb(VGA_PORT_DATA, (uint8_t)(pos & 0xFF));
    outb(VGA_PORT_COMMAND, 0x0E);
    outb(VGA_PORT_DATA, (uint8_t)((pos >> 8) & 0xFF));
}

void clear_screen() {
    uint16_t blank = (attribute << 8) | ' ';  // space character with current attribute
    
    for (int i = 0; i < SCREEN_SIZE; i++) {
        VGA_MEM[i] = blank;
    }
    
    cursor_position = 0; // Reset cursor position to top-left
    set_cursor(cursor_position);  // Update hardware cursor if you want
}

void move_cursor_to_end() {
    for (int i = SCREEN_WIDTH * SCREEN_HEIGHT - 1; i >= 0; i--) {
        char ch = VGA_MEM[i] & 0xFF;
        if (ch != 0 && ch != ' ') {
            cursor_position = i + 1;
            return;
        }
    }
    cursor_position = 0; // fallback to start if screen is blank
}

void putchar(char c) {
    if (c == '\n') {
        // Move cursor to start of next line
        cursor_position += SCREEN_WIDTH - (cursor_position % SCREEN_WIDTH);
    } else if (c == '\r') {
        // Carriage return: move cursor to start of current line
        cursor_position -= (cursor_position % SCREEN_WIDTH);
    } else if (c == '\b') {
        // Backspace: move cursor back and clear char
        if (cursor_position > 0) {
            cursor_position--;
            VGA_MEM[cursor_position] = (attribute << 8) | ' ';
        }
    } else {
        // Normal printable char
        VGA_MEM[cursor_position++] = (attribute << 8) | c;
    }

    // Scroll screen if cursor goes beyond screen
    if (cursor_position >= SCREEN_WIDTH * SCREEN_HEIGHT) {
        // Scroll everything up by one line
        for (int i = 0; i < SCREEN_WIDTH * (SCREEN_HEIGHT - 1); i++) {
            VGA_MEM[i] = VGA_MEM[i + SCREEN_WIDTH];
        }
        // Clear last line
        for (int i = SCREEN_WIDTH * (SCREEN_HEIGHT - 1); i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
            VGA_MEM[i] = (attribute << 8) | ' ';
        }
        cursor_position -= SCREEN_WIDTH;
    }

    // Update hardware cursor
    set_cursor(cursor_position);
}

void printf(const char* str) {
    while (*str) {
        putchar(*str++);
    }
}

void print(const char* str) {
    while (*str) {
        putchar(*str++);
    }
}

void set_background_color(unsigned char bg_color, unsigned char fg_color) {
    unsigned short attribute = ((bg_color & 0xF) << 4) | (fg_color & 0xF);
    unsigned short blank = (attribute << 8) | 0x20;  // space char with attr
    
    for (int i = 0; i < SCREEN_SIZE; i++) {
        VGA_MEM[i] = blank;
    }

    cursor_position = 0;
    set_cursor(cursor_position);
}

void console_putc(char c) {
    // Your existing code to output a char to screen or serial port.
    // Example (just a stub):
    // write character c to VGA text buffer or serial port
}

void console_write(const char *str) {
    while (*str) {
        console_putc(*str++);
    }
}
