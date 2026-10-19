global _start

section .text
    ; Allocate a block of memory and put its address in RAX.
    alloc:
        mov rbx, rax

        mov rax, 9
        mov rdi, 0
        mov rsi, rbx
        mov rdx, (1 | 2 | 4)
        mov r10, (2 | 32)
        mov r8, -1
        mov r9, 0
        syscall

        ret
    
    ; Initialize a buffer and put it in RAX.
    init_buffer:
        mov r11, rax
        mov r12, rbx

        imul rax, rbx
        call alloc

        mov [rax], r11
        mov [rax+1], r12

        ret
    
    ; Put pixel (RBX, RCX) of color RDX in buffer RAX.
    put_pixel:
        mov rsi, [rax+1]
        imul rsi, rcx
        add rsi, rax
        add rsi, 2
        add rsi, rbx

        mov [rsi], rdx
    
    ; Draw buffer RAX to STDOUT.
    draw_buffer:
        mov r8, rax
        mov r9, [rax+1]

        mov rbx, 0
        .xlp:
            mov rcx, 0
            .ylp:
                mov rsi, colcd_strt

                mov r10, rcx
                imul r10, r9
                add r10, r8
                add r10, 2
                add r10, rbx

                

                mov rax, 1
                mov rdi, 1

    _start:

section .data
    colcd_strt: db "\033[48;2;"
    colcd_mid: db "38;2;"
    colcd_end: db "m▄\033[0m"