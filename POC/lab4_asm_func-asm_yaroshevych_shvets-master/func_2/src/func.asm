include 'format/format.inc'

; Specify ELF64 as executable format.
format ELF64

; This is the code section of the program.
section '.text'

; Define public function
public func

func:
    ; According to Linux Calling Convention arguments are passed in registers as follows:
    ; RDI, RSI, RDX, RCX, R8, R9

    ; RDI - Array of a's
    ; RSI - Array of b's
    ; RDX - Answer array or x's
    ; RCX - Size of the input arrays

    ; R8 - counter (i)
    ; XMM0 - a[i]
    ; XMM1 - b[i]
    ; XMM2 - -1

    ; Initialize R8 to 0 (i = 0)
    xor R8, R8
    ; Initialize XMM2 to -1
    movss XMM2, [minus]
    ForLoop:
        ; Check if i < size
        cmp R8, RCX
        jge EndOfForLoop

        ; Move a[i] to XMM0 as 32-bit floating point
        movss XMM0, [RDI + 4*R8]
        ; Move b[i] to XMM1 as 32-bit floating point
        movss XMM1, [RSI + 4*R8]

        ; Perform a/b division
        divss XMM1, XMM0
        ; Negate the x value (to follow -b/a equation solution)
        mulss XMM1, XMM2

        ; Write x to RDX
        movss [RDX + 4*R8], XMM1

        ; i = i + 1
        inc R8
        jmp ForLoop
    EndOfForLoop:
        ret


; This is the data section of the program.
section '.data' writeable

minus dd -1.0f
