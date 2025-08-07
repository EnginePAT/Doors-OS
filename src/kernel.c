#include "include/console.h"
#include "include/colors.h"
#include "include/ports.h"  // for outb, inb
#include "include/keyboard.h"

void kernel(void) {
    set_background_color(VGA.BLACK, VGA.LIGHT_GRAY);
    clear_screen();

    // FAT12 Init Logic
    // Implement Later...

    login();

    while (1) {
        __asm__ volatile("hlt");
    }
}
