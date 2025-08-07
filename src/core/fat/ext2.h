#ifndef EXT2_H
#define EXT2_H

#include "../../include/stdint.h"

/// -----------------------------
/// Superblock
/// -----------------------------
typedef struct {
    uint32_t s_inodes_count;         // 0–3
    uint32_t s_blocks_count;         // 4–7
    uint32_t s_r_blocks_count;       // 8–11
    uint32_t s_free_blocks_count;    // 12–15
    uint32_t s_free_inodes_count;    // 16–19
    uint32_t s_first_data_block;     // 20–23
    uint32_t s_log_block_size;       // 24–27
    uint32_t s_log_frag_size;        // 28–31
    uint32_t s_blocks_per_group;     // 32–35
    uint32_t s_frags_per_group;      // 36–39
    uint32_t s_inodes_per_group;     // 40–43
    uint32_t s_mtime;                // 44–47
    uint32_t s_wtime;                // 48–51
    uint16_t s_mnt_count;            // 52–53
    uint16_t s_max_mnt_count;        // 54–55
    uint16_t s_magic;                // 56–57 (should be 0xEF53)
    uint16_t s_state;                // 58–59
    uint16_t s_errors;               // 60–61
    uint16_t s_minor_rev_level;      // 62–63
    uint32_t s_lastcheck;            // 64–67
    uint32_t s_checkinterval;        // 68–71
    uint32_t s_creator_os;           // 72–75
    uint32_t s_rev_level;            // 76–79
    uint16_t s_def_resuid;           // 80–81
    uint16_t s_def_resgid;           // 82–83
    uint32_t s_first_ino;            // 84–87
    uint16_t s_inode_size;           // 88–89
    uint16_t s_block_group_nr;       // 90–91
    uint32_t s_feature_compat;       // 92–95
    uint32_t s_feature_incompat;     // 96–99
    uint32_t s_feature_ro_compat;    // 100–103
    uint8_t  s_uuid[16];             // 104–119
    char     s_volume_name[16];      // 120–135
    char     s_last_mounted[64];     // 136–199
    uint32_t s_algo_bitmap;          // 200–203
    uint8_t  s_prealloc_blocks;      // 204
    uint8_t  s_prealloc_dir_blocks;  // 205
    uint16_t s_padding1;             // 206–207
    uint8_t  s_reserved[940];        // 208–1147 (end at 1024 bytes)
} __attribute__((packed)) ext2_superblock;

/// -----------------------------
/// Group Descriptor
/// -----------------------------
typedef struct {
    uint32_t bg_block_bitmap;        // 0–3
    uint32_t bg_inode_bitmap;        // 4–7
    uint32_t bg_inode_table;         // 8–11
    uint16_t bg_free_blocks_count;   // 12–13
    uint16_t bg_free_inodes_count;   // 14–15
    uint16_t bg_used_dirs_count;     // 16–17
    uint16_t bg_pad;                 // 18–19
    uint8_t  bg_reserved[12];        // 20–31
} __attribute__((packed)) ext2_group_desc;

/// -----------------------------
/// Inode Structure (128 bytes)
/// -----------------------------
typedef struct {
    uint16_t i_mode;         // 0–1
    uint16_t i_uid;          // 2–3
    uint32_t i_size;         // 4–7
    uint32_t i_atime;        // 8–11
    uint32_t i_ctime;        // 12–15
    uint32_t i_mtime;        // 16–19
    uint32_t i_dtime;        // 20–23
    uint16_t i_gid;          // 24–25
    uint16_t i_links_count;  // 26–27
    uint32_t i_blocks;       // 28–31
    uint32_t i_flags;        // 32–35
    uint32_t i_osd1;         // 36–39
    uint32_t i_block[15];    // 40–99
    uint32_t i_generation;   // 100–103
    uint32_t i_file_acl;     // 104–107
    uint32_t i_dir_acl;      // 108–111
    uint32_t i_faddr;        // 112–115
    uint8_t  i_osd2[12];     // 116–127
} __attribute__((packed)) ext2_inode;

/// -----------------------------
/// Directory Entry (Variable-Length)
/// -----------------------------
typedef struct {
    uint32_t inode;          // 0–3
    uint16_t rec_len;        // 4–5
    uint8_t  name_len;       // 6
    uint8_t  file_type;      // 7
    char     name[];         // 8+ (not null-terminated)
} __attribute__((packed)) ext2_dir_entry;

/// -----------------------------
/// Useful Constants
/// -----------------------------
#define EXT2_BLOCK_SIZE(sb) (1024 << (sb).s_log_block_size)         // Calculate the Block Size
#define EXT2_SUPER_MAGIC     0xEF53
#define EXT2_GOOD_OLD_INODE_SIZE 128
#define EXT2_N_BLOCKS        15

// File types for directory entries (ext2_dir_entry.file_type)
#define EXT2_FT_UNKNOWN  0
#define EXT2_FT_REG_FILE 1
#define EXT2_FT_DIR      2
#define EXT2_FT_CHRDEV   3
#define EXT2_FT_BLKDEV   4
#define EXT2_FT_FIFO     5
#define EXT2_FT_SOCK     6
#define EXT2_FT_SYMLINK  7

/// -----------------------------
/// Function Declarations
/// -----------------------------

#endif // EXT2_H
