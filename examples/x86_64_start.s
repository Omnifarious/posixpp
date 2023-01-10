.globl _start
_start:
        endbr64
        xor %ebp, %ebp
        pop %rdi         # Pop argc into C++ ABI first argument
        mov %rsp, %rsi   # Push argv into C++ ABI second argument
        and $~15, %rsp   # 16-byte align stack
        pushq %rax       # Garbage, need two 8-byte pushes to maintain alignment.
        pushq %rsp       # end of stack
        call main
        hlt              # Hopefully crash if you get here.

# TODO - Set up environ, see SVR4/i386 ABI (pages 3-31, 3-32), handle
#        atexit registration for shared library finish function in
#        %rdx
