; start.asm

    global KernelEntry
    global __AAA_MULTIBOOT
    extern KernelMain

    bits 32
    segment .text

F_PAGE_ALIGN equ 1<<0
F_MEM_FIELDS equ 1<<1

MAGIC        equ 0x1BADB002
FLAGS        equ F_PAGE_ALIGN | F_MEM_FIELDS
CHECKSUM     equ -(MAGIC + FLAGS)

KernelEntry:
    mov esp, 0x00101000
    push ebx
    call KernelMain
KernelEntry__loop1:
    hlt ; just in case we return for some reason.
    jmp KernelEntry__loop1

segment .multiboot
; The Multiboot header
    align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM
