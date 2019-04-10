#include "sizetable.h"
#include <stdio.h>
#include <stdlib.h>

extern void* __SHEAP_SIZETABLE_START;
extern void* __SHEAP_SIZETABLE_END;
int main()
{
    initialize_sizetable(20);
    printf("%d\n", sizeof(struct size_table_elem));
    printf("%p\n", __SHEAP_SIZETABLE_START);
    printf("%p\n", __SHEAP_SIZETABLE_END);
    expand_sizetable(10);
    printf("%p\n", __SHEAP_SIZETABLE_START);
    printf("%p\n", __SHEAP_SIZETABLE_END);
}
