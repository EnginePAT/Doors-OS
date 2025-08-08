[bits 32]

section .multiboot_header
align 8
    dd 0xE85250D6              ; Multiboot2 magic
    dd 0                       ; Architecture (0 = i386)
    dd header_end - header_start  ; Header length
    dd -(0xE85250D6 + 0 + (header_end - header_start)) ; Checksum

header_start:
    ; Tag: End
    dw 0                       ; Type = 0 (end tag)
    dw 0                       ; Flags
    dd 8                       ; Size
header_end:

section .text
global _start
_start:
    cli
    mov esp, 0x9FB00           ; ✅ Set up stack

    push ebx                   ; Push Multiboot2 info
    push eax                   ; Push magic number
    extern kernel
    call kernel                ; Call C kernel function

.hang:
    cli
    hlt
    jmp .hang
