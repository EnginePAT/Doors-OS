#include "../include/ext2.h"
#include "../include/console.h"  // your print(), etc
#include "../include/ports.h"

typedef struct {
    ext2_superblock_t* superblock;
    ext2_group_desc_t* group_desc;
    uint32_t block_size;
    uint8_t* base_addr;  // base of EXT2 image in memory
} ext2_fs_t;

ext2_fs_t ext2_fs;
ext2_group_desc_t gdt;

static inline void insw(uint16_t port, void* addr, int count) {
    asm volatile("rep insw" : "+D"(addr), "+c"(count) : "d"(port) : "memory");
}

// Temporary buffer to avoid stack overflows
uint8_t inode_buffer[EXT2_BLOCK_SIZE];

void* memcpy(void* dest, const void* src, unsigned int n) {
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;

    for (unsigned int i = 0; i < n; i++) {
        d[i] = s[i];
    }

    return dest;
}

// Wait until disk is ready
void ata_wait() {
    while (inb(ATA_COMMAND_PORT) & ATA_STATUS_BSY);
    while (!(inb(ATA_COMMAND_PORT) & ATA_STATUS_DRQ));
}

int fs_init(uint8_t* ext2_image_base) {
    ext2_fs.base_addr = ext2_image_base;

    // Superblock is at offset 1024 bytes
    ext2_fs.superblock = (ext2_superblock_t*)(ext2_image_base + 1024);

    if (ext2_fs.superblock->s_magic != 0xEF53) {
        print("EXT2 fs_init: Invalid magic number, not an EXT2 filesystem.\n");
        return -1;
    }

    ext2_fs.block_size = 1024 << ext2_fs.superblock->s_log_block_size;

    print("EXT2 filesystem detected.\n");
    print("Block size: ");
    print_uint(ext2_fs.block_size);
    print("\n");

    // Group descriptor table is immediately after superblock
    // For 1KB blocks, group desc start at block 2 (offset 2048)
    // For >1KB blocks, group desc start at block 1 (block_size)
    uint32_t group_desc_offset;
    if (ext2_fs.block_size == 1024) {
        group_desc_offset = 2048;
    } else {
        group_desc_offset = ext2_fs.block_size;
    }

    ext2_fs.group_desc = (ext2_group_desc_t*)(ext2_image_base + group_desc_offset);

    print("EXT2 fs_init: Initialization complete.\n");

    return 0;
}

// Read 512 bytes from disk at LBA `lba` into `buffer`
void read_disk(uint32_t lba, uint8_t* buffer) {
    outb(ATA_PRIMARY_IO + 6, 0xE0 | ((lba >> 24) & 0x0F));       // Drive + LBA bits
    outb(ATA_PRIMARY_IO + 2, 1);                                 // Sector count
    outb(ATA_PRIMARY_IO + 3, (uint8_t)(lba & 0xFF));             // LBA low
    outb(ATA_PRIMARY_IO + 4, (uint8_t)((lba >> 8) & 0xFF));      // LBA mid
    outb(ATA_PRIMARY_IO + 5, (uint8_t)((lba >> 16) & 0xFF));     // LBA high
    outb(ATA_COMMAND_PORT, ATA_CMD_READ);                        // Read command

    ata_wait();                                                  // Wait for drive

    insw(ATA_PRIMARY_IO, buffer, SECTOR_SIZE / 2);               // Read 512 bytes
}

ext2_inode_t* ext2_read_inode(uint32_t inode_num) {
    if (inode_num < 1) return NULL;

    // Index is zero-based
    uint32_t index = inode_num - 1;

    uint32_t inode_table_block = gdt.bg_inode_table;
    uint32_t inodes_per_block = EXT2_BLOCK_SIZE / EXT2_INODE_SIZE;

    uint32_t block = inode_table_block + (index / inodes_per_block);
    uint32_t offset = (index % inodes_per_block) * EXT2_INODE_SIZE;

    // Read the block into memory
    read_disk(block, inode_buffer);

    // Copy the inode into a static struct
    static ext2_inode_t inode;
    memcpy(&inode, inode_buffer + offset, sizeof(ext2_inode_t));

    return &inode;
}
