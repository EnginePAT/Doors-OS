#include "../include/multiboot2.h"

static uint32_t module_start = 0;
static uint32_t module_end = 0;

void multiboot2_init(uint32_t addr) {
    uint32_t total_size = *(uint32_t*)addr;
    uint32_t offset = addr + 8; // skip total_size and reserved fields

    while (offset < addr + total_size) {
        multiboot_tag_t* tag = (multiboot_tag_t*)offset;

        if (tag->type == MULTIBOOT_TAG_TYPE_END) {
            break;
        }

        if (tag->type == MULTIBOOT_TAG_TYPE_MODULE) {
            multiboot_tag_module_t* mod = (multiboot_tag_module_t*)tag;

            // We assume the first module is ext2.img
            module_start = mod->mod_start;
            module_end = mod->mod_end;
            break; // stop after first module
        }

        offset = ALIGN(offset + tag->size, MULTIBOOT_TAG_ALIGN);
    }
}

uint32_t multiboot2_get_module_start() {
    return module_start;
}

uint32_t multiboot2_get_module_end() {
    return module_end;
}
