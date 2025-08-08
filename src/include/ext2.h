#ifndef EXT2_H
#define EXT2_H

#include "stdint.h"

#define NULL ((void*)0)

#define EXT2_SUPERBLOCK_OFFSET 1024
#define EXT2_INODE_SIZE 128  // Classic ext2 inode size
#define EXT2_BLOCK_SIZE 1024 // Simplest ext2, 1KB blocks

#define SECTOR_SIZE 512
#define ATA_PRIMARY_IO 0x1F0
#define ATA_COMMAND_PORT 0x1F7
#define ATA_STATUS_BSY 0x80
#define ATA_STATUS_DRQ 0x08
#define ATA_CMD_READ 0x20

// EXT2 Superblock Structure (1024 bytes at offset 1024)
typedef struct __attribute__((packed)) {
    uint32_t s_inodes_count;         // Total number of inodes
    uint32_t s_blocks_count;         // Total number of blocks
    uint32_t s_r_blocks_count;       // Reserved blocks count
    uint32_t s_free_blocks_count;    // Free blocks count
    uint32_t s_free_inodes_count;    // Free inodes count
    uint32_t s_first_data_block;     // First data block number
    uint32_t s_log_block_size;       // Block size = 1024 << s_log_block_size
    uint32_t s_log_frag_size;        // Fragment size
    uint32_t s_blocks_per_group;     // Blocks per group
    uint32_t s_frags_per_group;      // Fragments per group
    uint32_t s_inodes_per_group;     // Inodes per group
    uint32_t s_mtime;                // Mount time
    uint32_t s_wtime;                // Write time
    uint16_t s_mnt_count;            // Mount count
    uint16_t s_max_mnt_count;        // Max mount count before fsck
    uint16_t s_magic;                // Magic signature (0xEF53)
    uint16_t s_state;                // File system state
    uint16_t s_errors;               // Behaviour when detecting errors
    uint16_t s_minor_rev_level;      // Minor revision level
    uint32_t s_lastcheck;            // Time of last check
    uint32_t s_checkinterval;        // Max time between checks
    uint32_t s_creator_os;           // OS
    uint32_t s_rev_level;            // Revision level
    uint16_t s_def_resuid;           // Default uid for reserved blocks
    uint16_t s_def_resgid;           // Default gid for reserved blocks
    uint32_t s_first_ino;            // First non-reserved inode
    uint16_t s_inode_size;           // Size of inode structure
    uint16_t s_block_group_nr;       // Block group number of this superblock
    uint32_t s_feature_compat;       // Compatible features
    uint32_t s_feature_incompat;     // Incompatible features
    uint32_t s_feature_ro_compat;    // Readonly-compatible features
    uint8_t  s_uuid[16];             // 128-bit UUID
    char     s_volume_name[16];      // Volume name
    char     s_last_mounted[64];     // Directory where last mounted
    uint32_t s_algorithm_usage_bitmap; // For compression
    uint8_t  s_prealloc_blocks;      // Number of blocks to preallocate for files
    uint8_t  s_prealloc_dir_blocks;  // Number of blocks to preallocate for directories
    uint16_t s_padding1;             // Padding
    uint8_t  s_journal_uuid[16];     // Journal UUID
    uint32_t s_journal_inum;         // Journal inode number
    uint32_t s_journal_dev;          // Journal device
    uint32_t s_last_orphan;          // Start of list of inodes to delete
    uint32_t s_hash_seed[4];         // Hash seed for directories
    uint8_t  s_def_hash_version;     // Default hash version
    uint8_t  s_reserved_char_pad;    // Reserved char padding
    uint16_t s_reserved_word_pad;    // Reserved word padding
    uint32_t s_default_mount_opts;   // Default mount options
    uint32_t s_first_meta_bg;        // First metablock block group
    uint32_t s_reserved[190];        // Padding to 1024 bytes
} ext2_superblock_t;

// EXT2 Group Descriptor Structure (32 bytes per group)
typedef struct __attribute__((packed)) {
    uint32_t bg_block_bitmap;         // Block ID of block bitmap
    uint32_t bg_inode_bitmap;         // Block ID of inode bitmap
    uint32_t bg_inode_table;          // Block ID of inode table start
    uint16_t bg_free_blocks_count;    // Free blocks count in group
    uint16_t bg_free_inodes_count;    // Free inodes count in group
    uint16_t bg_used_dirs_count;      // Directories count in group
    uint16_t bg_pad;                  // Padding
    uint8_t  bg_reserved[12];         // Reserved
} ext2_group_desc_t;

// EXT2 Inode Structure (variable size, typically 128 bytes)
typedef struct __attribute__((packed)) {
    uint32_t   i_mode;
    uint32_t   i_uid;
    uint32_t   i_size;
    uint32_t   i_atime;
    uint32_t   i_ctime;
    uint32_t   i_mtime;
    uint32_t   i_dtime;
    uint32_t   i_gid;
    uint32_t   i_links_count;
    uint32_t   i_blocks;
    uint32_t   i_flags;
    uint32_t   i_osd1;
    uint32_t   i_block[15]; // pointers to blocks
    uint32_t   i_generation;
    uint32_t   i_file_acl;
    uint32_t   i_dir_acl;
    uint32_t   i_faddr;
    uint8_t    i_osd2[12];
} ext2_inode_t;

// EXT2 Directory Entry Structure (variable length)
typedef struct __attribute__((packed)) {
    uint32_t inode;             // Inode number
    uint16_t rec_len;           // Directory entry length
    uint8_t  name_len;          // Name length
    uint8_t  file_type;         // File type
    char     name[];            // File name (not null-terminated)
} ext2_dir_entry_t;

int fs_init(uint8_t* ext2_image_base);
ext2_inode_t* ext2_read_inode(uint32_t inode_num);

#endif // EXT2_H
