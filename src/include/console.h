#ifndef CONSOLE_H
#define CONSOLE_H

// Function Definitions
void clear_screen();
void putchar(char c);
void printf(const char* str);
void print(const char* str);
void set_background_color(unsigned char bg_color, unsigned char fg_color);
void console_write(const char *str);
void console_putc(char c);

#endif      // CONSOLE_H