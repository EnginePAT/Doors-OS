# Paths
SRC_DIR := src
BIN_DIR := bin
ISO_DIR := doors

# Tools
CC := x86_64-elf-gcc
LD := x86_64-elf-ld
NASM := nasm

# Flags
CFLAGS := -ffreestanding -m32 -c
LDFLAGS := -T $(SRC_DIR)/linker.ld -m elf_i386

# Files
BOOT_OBJ := $(BIN_DIR)/boot.o
KERNEL_OBJ := $(BIN_DIR)/kernel.o
CONSOLE_OBJ := $(BIN_DIR)/console.o
COLORS_OBJ := $(BIN_DIR)/colors.o
IRQ_OBJ := $(BIN_DIR)/irq.o
ISR_OBJ := $(BIN_DIR)/isr.o
PORTS_OBJ := $(BIN_DIR)/ports.o
KEYBOARD_OBJ := $(BIN_DIR)/keyboard.o
MULTIBOOT2_OBJ := $(BIN_DIR)/multiboot2.o
EXT2_OBJ := $(BIN_DIR)/ext2.o

KERNEL_ELF := $(BIN_DIR)/kernel.elf

# OBJ Files for the linker
OBJS = $(BOOT_OBJ) $(KERNEL_OBJ) $(CONSOLE_OBJ) $(COLORS_OBJ) \
       $(PORTS_OBJ) $(ISR_OBJ) $(IRQ_OBJ) $(KEYBOARD_OBJ) $(MULTIBOOT2_OBJ) \
	   $(EXT2_OBJ)

.PHONY: clean all dirs build

all: dirs $(KERNEL_ELF) copy build

dirs:
	mkdir -p $(BIN_DIR)
	mkdir -p $(ISO_DIR)/boot/grub

# Kernel
$(BOOT_OBJ): $(SRC_DIR)/boot/boot.asm
	$(NASM) -f elf32 $< -o $@

$(KERNEL_OBJ): $(SRC_DIR)/kernel.c
	$(CC) $(CFLAGS) $< -o $@

$(CONSOLE_OBJ): $(SRC_DIR)/console.c
	$(CC) $(CFLAGS) $< -o $@

# Core Kernel Functionality
$(COLORS_OBJ): $(SRC_DIR)/core/colors.c
	$(CC) $(CFLAGS) $< -o $@

$(IRQ_OBJ): $(SRC_DIR)/core/irq.asm
	$(NASM) -f elf32 $< -o $@

$(ISR_OBJ): $(SRC_DIR)/core/isr.c
	$(CC) $(CFLAGS) $< -o $@

$(PORTS_OBJ): $(SRC_DIR)/core/ports.c
	$(CC) $(CFLAGS) $< -o $@

$(KEYBOARD_OBJ): $(SRC_DIR)/keyboard.c
	$(CC) $(CFLAGS) $< -o $@

$(MULTIBOOT2_OBJ): $(SRC_DIR)/core/multiboot2.c
	$(CC) $(CFLAGS) $< -o $@

$(EXT2_OBJ): $(SRC_DIR)/core/ext2.c
	$(CC) $(CFLAGS) $< -o $@

# Linker
$(KERNEL_ELF): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

copy:
	cp $(KERNEL_ELF) $(ISO_DIR)/boot/kernel.elf

clean:
	rm -rf $(BIN_DIR)
	rm -f $(ISO_DIR)/boot/kernel.elf
	rm -f doors-11.iso

build:
	i686-elf-grub-mkrescue -o doors-11.iso doors/

run:
	qemu-system-x86_64 -cdrom doors-11.iso -vga std
