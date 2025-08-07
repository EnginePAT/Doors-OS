#ifndef STDINT_H
#define STDINT_H

#define VGA_MEM ((unsigned short*)0xB8000)
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT)

// Int types
typedef signed char        int8_t;
typedef signed short       int16_t;
typedef signed int         int32_t;
typedef signed long long   int64_t;

// Unsigned Int types
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

// Boolean
#define bool
#define true 1
#define false 0

#endif // STDINT_H
