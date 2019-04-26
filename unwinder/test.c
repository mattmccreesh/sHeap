#define UNW_LOCAL_ONLY
#include <libunwind.h>
#include <stdio.h>
#include <stdlib.h>

int count_valid = 0;
int count_nptr = 0;
void* last_ret = (void*) 100;
void* ret_addr_overwritten;

void* fake_call(){
    return (void*) 9;
}

void detectorAsm(){
  //archive value returned but intercepted
  asm("mov %rax, %r11");
  asm("push %r11");//push as register may change in function call
  asm("mov %0, %%rax" : : "r"(last_ret));
  asm("cmp %rax, %r11");
  asm goto("jne %l0\n" : : : : notEqual);
  printf("Wrapper detected\n");
  asm goto("jmp %l0\n" : : : : jmpBack);
notEqual:
  printf("Not a wrapper\n");
jmpBack:
  asm("mov %0, %%r11" : : "r"(ret_addr_overwritten));
  asm("pop %rax");//restore stack, get rax back to propogate return value properly
  asm("jmp *%r11");
}

//this should actually be an assembly routine
void detector(){
    void* v = fake_call();
    if(v==last_ret){
       printf("Wrapper detected\n");
    }
    else{
        printf("NOT a wrapper\n");
    }
    //asm volatile("mov %0, %r15" : : "r"(ret_addr_overwritten));
    //asm volatile("mov %0, %rax" : : "r"(v));
    asm("mov %0, %%r11" : : "r"(ret_addr_overwritten));
    asm("mov %0, %%rax" : : "r"(v));
    asm("jmp *%r11");
}

void* unwinder()
{
    unw_cursor_t cursor;
    unw_context_t uc;
    unw_word_t ip;
    unw_word_t sp;
    unw_getcontext(&uc);
    unw_init_local(&cursor, &uc);
    unw_step(&cursor);
    unw_step(&cursor);
    unw_get_reg(&cursor, UNW_REG_IP, &ip);
    printf("instruction pointer at %lx\n", (long)ip);
    unw_get_reg(&cursor, UNW_REG_SP, &sp);
    sp -= 8;//minus 8 bytes for return address
    printf("return address at %lx\n", (long)sp);
    last_ret = 0;//value returned by us
    ret_addr_overwritten = (void*) ip;
    void** ret_addr_to_overwrite = (void**) sp;
    *ret_addr_to_overwrite = &detectorAsm + 4;
    return (void*) 0;
}

int non_wrapper()
{
    void* v = unwinder();
    return 4;
}

void* wrapper(){
    void* x = unwinder();
    if (x == 0){
        count_nptr++;
    }else{
	count_valid++;
    }
    return x;
}

int main()
{
    void* q = wrapper();
    printf("FIRST RET %p\n", q);
    int j = non_wrapper();
    printf("SECOND RET %d\n", j);
    return 0;
}
