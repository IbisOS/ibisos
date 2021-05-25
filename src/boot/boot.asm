;Align modules on page boundaries
MBALIGN equ 1 << 0
MEMINFO equ 1 << 1
MULTIBOOT_FLAGS equ MBALIGN | MEMINFO
MAGIC equ 0x1BADB002
CHECKSUM equ -(MAGIC + MULTIBOOT_FLAGS)

;Multiboot header that bootloaders need
section .Multiboot
    align 4
    dd MAGIC
    dd MULTIBOOT_FLAGS
    dd CHECKSUM

;Section defining stack bottom and top
section .bss
    align 16
global stack_bottom:function
stack_bottom:
    resb 16384
global stack_top:function
stack_top:

;Section where initiation of kernel is done
section .text
global load_kernel:function (load_kernel.end - load_kernel)
load_kernel:
    ;Set esp to top of stack register
    mov esp, stack_top
    ;Get the kernel start function
    extern StartKernel
    ;Start the kernel
    call StartKernel
    ;Disable interrupts
    cli
    hlt
.end: