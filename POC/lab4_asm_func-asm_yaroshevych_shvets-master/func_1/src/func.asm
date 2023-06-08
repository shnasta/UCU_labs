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
    ; Since function is defined as <func(int_64_t* input_array, size_t size)>
    ; RDI - Pointer to the input array
    ; RSI - Size of input array

    ; RAX - Return value


    ; Sorting will be done using insertion sort algorithm.
    ; Pseudo code:
    ;
    ; for i = 1 to size
    ;   key = input_array[i]
    ;   j = i - 1
    ;   while j >= 0 and input_array[j] > key
    ;     input_array[j + 1] = input_array[j]
    ;     j = j - 1
    ;   input_array[j + 1] = key
    ;
    ; We will need two counters, one for the outer loop and one for the inner loop.
    ; We will use R8 and R9 for this purpose.
    ; We will also need a temporary variable to store the value of the key.
    ; We will use R10 for this purpose.

    ; Initialize R8 to 1 (i = 1)
    mov R8, 1
    ForLoop:
        ; Check if i < size
        cmp R8, RSI
        jge EndOfForLoop

        ; key = input_array[i]. Address of input_array[i] is RDI + 4*i (4 bytes per element)
        mov R10D, [RDI + 4*R8]

        ; j = i - 1
        mov R9, R8
        dec R9
        WhileLoop:
            ; Check if j >= 0
            test R9, R9
            jl EndOfWhileLoop

            ; Check if input_array[j] > key
            mov R11D, [RDI+4*R9]
            cmp R11D, R10D
            jl EndOfWhileLoop

            ; input_array[j + 1] = input_array[j]
            mov [RDI + 4*R9 + 4], R11D

            ; j = j - 1
            dec R9
            jmp WhileLoop
        EndOfWhileLoop:
            ; input_array[j + 1] = key
            mov [RDI + 4*R9 + 4], R10D

            ; i = i + 1
            inc R8 ;
            jmp ForLoop
    EndOfForLoop:
        ret