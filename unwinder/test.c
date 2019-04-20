#define UNW_LOCAL_ONLY
#include <libunwind.h>
#include <stdio.h>
#include <stdlib.h>

int count_valid = 0;
int count_nptr = 0;

void* unwinder()
{
    unw_cursor_t cursor;
    unw_context_t uc;
    unw_word_t ip;
    unw_getcontext(&uc);
    unw_init_local(&cursor, &uc);
    unw_step(&cursor);
    unw_step(&cursor);
    unw_get_reg(&cursor, UNW_REG_IP, &ip);
    printf("instruction pointer at %lx\n", (long)ip);
    return (void*) 0;
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
    printf("hello, %p\n", q);
    return 0;
}
