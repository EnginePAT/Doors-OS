#include "include/console.h"
#include "include/colors.h"
#include "include/ports.h"  // for outb, inb
#include "include/keyboard.h"
#include "include/multiboot2.h"
#include "include/ext2.h"

void print_hex(uint32_t num);
void print_inode_info(ext2_inode_t* inode);

void kernel(uint32_t multiboot_magic, uint32_t multiboot_info) {
    // Setup Multiboot2
    if (multiboot_magic != MULTIBOOT2_MAGIC) {
        // Invalid boot, halt or error
        print("Invalid Multiboot2 Magic Number\n");
        while (1);
    }

    multiboot2_init(multiboot_info);

    uint32_t ext2_start = multiboot2_get_module_start();
    uint32_t ext2_end = multiboot2_get_module_end();

    // Now you can read from ext2_start memory as your EXT2 disk image
    uint8_t first_byte = *((uint8_t*)ext2_start);

    // Setup the screen
    set_background_color(VGA.BLACK, VGA.LIGHT_GRAY);
    clear_screen();

    // EXT2 Init Logic
    print("Initializing EXT2 filesystem...\n");

    if (fs_init((uint8_t*)ext2_start) != 0) {
        print("EXT2 init failed.\n");
        while (1) { __asm__ volatile("hlt"); }
    }

    print("EXT2 filesystem ready!\n");
    print("\n");

    print("Yoooooo Wassuppp Welcome to Doors 11\n");

    login();

    while (1) {
        __asm__ volatile("hlt");
    }
}

void print_hex(uint32_t num) {
    char hex_chars[] = "0123456789ABCDEF";
    char buffer[9];  // 8 hex digits + null terminator
    buffer[8] = '\0';

    for (int i = 7; i >= 0; i--) {
        buffer[i] = hex_chars[num & 0xF];
        num >>= 4;
    }

    print(buffer);
}

void print_inode_info(ext2_inode_t* inode) {
    print("Inode info:\n");

    print("Size: ");
    print_uint(inode->i_size);
    print("\n");

    print("Mode: 0x");
    print_hex(inode->i_mode);
    print("\n");

    print("Blocks: ");
    print_uint(inode->i_blocks);
    print("\n");

    print("Direct block pointers:\n");
    for (int i = 0; i < 12; i++) {  // first 12 direct pointers
        print("  ");
        print_uint(inode->i_block[i]);
        print("\n");
    }
}
