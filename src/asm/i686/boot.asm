;Multiboot header constants
MBALIGN equ 1 << 0
MEMINFO equ 1 << 1
MULTIBOOT_HEADER_FLAGS equ MBALIGN | MEMINFO
MULTIBOOT_HEADER_MAGIC equ 0x1BADB002
CHECKSUM equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

;Mark this program as a kernel
section .multiboot
align 4
    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd CHECKSUM

;Declare memory stack
section .bss
align 16
global stack_bottom
stack_bottom:
    resb 16384
global stack_top
stack_top:

;Start is entry point, so define it
section .text
global _start:function (_start.end - _start)
_start:
    ;Loaded into protected mode on x86 machine
    mov esp, stack_top
    ;Load GDT and enable paging here.
    ;Enter high level kernel
    extern kernelMain
    call kernelMain

    ;Put the computer in a loop
    cli
.hang: hlt
    jmp .hang
.end: