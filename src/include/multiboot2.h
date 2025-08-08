#ifndef MULTIBOOT2_H
#define MULTIBOOT2_H

#include "stdint.h"

#define MULTIBOOT2_MAGIC 0x36d76289

// Tag types
#define MULTIBOOT_TAG_TYPE_END        0
#define MULTIBOOT_TAG_TYPE_MODULE     3

// All tags are 8-byte aligned
#define MULTIBOOT_TAG_ALIGN 8
#define ALIGN(x, a) (((x) + (a - 1)) & ~(a - 1))

// Generic tag header
typedef struct {
    uint32_t type;
    uint32_t size;
} multiboot_tag_t;

// Module tag (ext2.img will be one of these)
typedef struct {
    uint32_t type;
    uint32_t size;
    uint32_t mod_start;
    uint32_t mod_end;
    char cmdline[]; // Null-terminated string follows
} multiboot_tag_module_t;

// Function declarations
void multiboot2_init(uint32_t addr);
uint32_t multiboot2_get_module_start();
uint32_t multiboot2_get_module_end();

#endif // MULTIBOOT2_H
