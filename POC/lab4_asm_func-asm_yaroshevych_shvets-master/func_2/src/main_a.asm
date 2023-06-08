include 'format/format.inc'

; Specify ELF64 as executable format.
format ELF64

; This is the code section of the program.
section '.text' executable

; Define public function
public main
; Include the external sorting function and printf
extrn func
extrn printf

main:
    ; Define the stack frame
    push rbp
    ; Set the base pointer to the stack pointer
    mov rbp, rsp

    ; Call the solving function satisfying the calling convention
    lea RDI, [a_coef] ; Set the first argument to the address of a_coef
    lea RSI, [b_coef] ; Set the second argument to the address of b_coef
    lea RDX, [result] ; Set the third argument to the address of result
    mov RCX, [len] ; Set the fourth argument to the length of the arrays
    call func

    ; Print the result
    call print

    ; Restore the stack frame
    leave
    ret

; Print the result
print:
    push rbp
    mov rbp, rsp

    ; Align the stack to 16 bytes for the printf function
    sub rsp, 16
    ; Save R12
    mov [rbp-16], R12
    ; Loop through the array and print each element. Set counter to 0 (using xor).
    ; R12 is used since it is a callee-saved register. printf allowed to change all scratch registers.
    xor R12, R12
    ForLoop:
        ; If the counter is greater than the length of the array, break
        cmp R12, [len]
        jge EndOfForLoop

        ; Call printf to print the element satisfying the calling convention
        lea RDI, [msg]
        ; Set number of XMMx registers to print
        mov RAX, 3
        movss XMM0, [a_coef + 4*R12]
        movss XMM1, [b_coef + 4*R12]
        movss XMM2, [result + 4*R12]

        ; Convert single precision floats to double precision floats aka double
        cvtss2sd XMM0, XMM0
        cvtss2sd XMM1, XMM1
        cvtss2sd XMM2, XMM2

        call printf

        ; Increment the counter and loop
        inc R12
        jmp ForLoop
    EndOfForLoop:
        ; Clear RDI
        xor RDI, RDI
        ; Restore R12
        add rsp, 16
        mov R12, [rbp-16]
        ; Restore the stack pointer to the base pointer and return
        leave
        ret


; This is the data section of the program.
section '.data' writeable

; Define the array of a coefficients
a_coef dd -20.0f, 10.333f, -15.75f, 5.0f, 37.5004f
; Define the array of b coefficients
b_coef dd 10.0f, 0.0f, 5.5f, 20.0f, -75.0008f
; Define the array of the results
result dd 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
; Define the length of arrays
len dq 5

; Define the message to be printed.
; Arguments are passed in the following order: a coefficient, b coefficient, result
msg db "%f*x + %f = 0,  x = %f", 0xA, 0
