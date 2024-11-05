#include "asm.h"
#include <string.h>
#include<stdio.h>

int64_t asm_add(int64_t a, int64_t b) {
  asm(
        "addq %[b], %[a]"
        : [a] "+r"(a)
        : [b] "r"(b)
        : 
    );
  return a;
}

int asm_popcnt(uint64_t x) {
int s = 0;
asm (
"xorl %[s], %[s];"
"movq %[x], %[c];"
"1:;"
"testq %[c], %[c];"
"jz 2f;"
"bt $0, %[c];"
"adc $0, %[s];"
"shrq $1, %[c];"
"jmp 1b;"
"2:;"
: [s] "+r"(s), [c] "=&r"(x)
: [x] "r"(x)
);
  return s;
}

void *asm_memcpy(void *dest, const void *src, size_t n) {
  asm (
"rep movsb;"
: "+D"(dest), "+S"(src), "+c"(n)
:
: "memory"
);
  return dest;
}

int asm_setjmp(asm_jmp_buf env) {
    int s = 0;
  uint64_t x=99;
asm (
"xorl %[s], %[s];"
"movq %[x], %[c];"
"1:;"
"testq %[c], %[c];"
"jz 2f;"
"bt $0, %[c];"
"adc $0, %[s];"
"shrq $1, %[c];"
"jmp 1b;"
"2:;"
: [s] "+r"(s), [c] "=&r"(x)
: [x] "r"(x)
);
  return s;
}

void asm_longjmp(asm_jmp_buf env, int val) {
  return;
}