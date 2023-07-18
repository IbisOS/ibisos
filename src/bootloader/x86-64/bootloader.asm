global boot
boot:
    mov ah, 0x0e ;request text only mode

    ;Try to print the secret character
    ;First attempt...
    mov al, my_secret
    int 0x10 ;Does this print the secret?

    ;Second attempt
    mov al, [my_secret]
    int 0x10 ;Does this print the secret?

    ;Third and final attempt
    mov bx, my_secret ;Copy secret's address into bx register
    add bx, 0x7c00 ;Add secret's address to 0x7c00 (bottom of boot sector)
    mov al, [bx]
    int 0x10 ;Does this final attempt print the secret?
    jmp $ ;jump to current address (basically loop)

global my_secret
my_secret:
    db "X" ;The secret!

times 510-($-$$) db 0 ; Fill with 510 zeros, accounting for size of loop
dw 0xaa55 ; Magic number