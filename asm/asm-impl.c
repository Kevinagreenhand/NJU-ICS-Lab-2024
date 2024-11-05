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
"movq %[x], %[a];"
"1:;"
"testq %[a], %[a];"
"jz 2f;"
"bt 0x0, %[a];"
"adc 0x0, %[s];"
"shrq 0x1, %[a];"
"jmp 1b;"
"2:;"
: [s] "+r"(s), [a] "=&r"(x)
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
    uint64_t tmp1=0;
    uint64_t tmp2=0;
    asm(
        "push %%rbp;"
        "mov %%rsp, %%rbp;"
        "movq (%%rbp), %[tmp1];"
        "movq %[tmp1], %[a];"
        "movq %%rsp, %[tmp1];"
        "add 0x10, %[tmp1]"
        "movq %[tmp1], %[b];"
        "movq %%rbx, %[c];"
        "movq %%rbp, %[tmp2]"
        "add 0x8, %[tmp2]"
        "movq (%[tmp2]), %[tmp1];"
        "movq %[tmp1], %[d];"
        "movq %%r12, %[e];"
        "movq %%r13, %[f];"
        "movq %%r14, %[g];"
        "movq %%r15, %[h];"
        "pop %%rbp;"
        : [a] "=m"(env[0]), [b] "=m"(env[1]), [c] "=m"(env[2]),
          [d] "=m"(env[3]), [e] "=m"(env[4]), [f] "=m"(env[5]),
          [g] "=m"(env[6]), [h] "=m"(env[7])，[tmp1] "=&r"(tmp1), [tmp2] "=&r"(tmp2)
        :
        : "memory","rbp"
    );
    return 0;
}

void asm_longjmp(asm_jmp_buf env, int val) {
  return;
}