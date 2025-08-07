ENTRY(kernel)

SECTIONS
{
    . = 1M;

    .multiboot_header :
    {
        KEEP(*(.multiboot_header))
    }

    .text : {
        *(.multiboot)
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    .bss : {
        *(.bss*)
        *(COMMON)
    }
}
