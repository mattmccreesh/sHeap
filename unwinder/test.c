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
  asm("cmp %%rax, %0" : : "r"(last_ret));
  //asm("jne $+0x2");
  
  /*
   *0x0000000000000903 <+29>:	cmp    %rax,-0x8(%rbp)
   0x0000000000000907 <+33>:	jne    0x917 <detector+49>
   0x0000000000000909 <+35>:	lea    0x2a4(%rip),%rdi        # 0xbb4
   0x0000000000000910 <+42>:	callq  0x740 <puts@plt>
   0x0000000000000915 <+47>:	jmp    0x923 <detector+61>
   0x0000000000000917 <+49>:	lea    0x2a7(%rip),%rdi        # 0xbc5
   0x000000000000091e <+56>:	callq  0x740 <puts@plt>
   0x0000000000000923 <+61>:	mov    0x2016fe(%rip),%rax        # 0x202028 <ret_addr_overwritten>
   0x000000000000092a <+68>:	mov    %rax,%r11
   0x000000000000092d <+71>:	mov    -0x8(%rbp),%rax
   0x0000000000000931 <+75>:	mov    %rax,%rax
   0x0000000000000934 <+78>:	jmpq   *%r11
  */ 
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
    *ret_addr_to_overwrite = &detector + 18;
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
