global irq1_handler
extern irq_handler

section .text
irq1_handler:
    cli
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push dword 33         ; Interrupt number (IRQ1 + 32)
    push dword 0          ; Error code (none for IRQs)

    call irq_handler

    add esp, 8            ; Pop int_no and err_code

    pop gs
    pop fs
    pop es
    pop ds
    popa
    sti
    iretd
