#include "sizetable.h"
#include <stdio.h>
#include <stdlib.h>

extern void* __SHEAP_SIZETABLE_START;
extern void* __SHEAP_SIZETABLE_END;
int main()
{
    initialize_sizetable(20);
    printf("%ld\n", sizeof(struct size_table_elem));
    printf("%p\n", __SHEAP_SIZETABLE_START);
    printf("%p\n\n\n", __SHEAP_SIZETABLE_END);
    /*
    expand_sizetable(10);
    printf("%p\n", __SHEAP_SIZETABLE_START);
    printf("%p\n", __SHEAP_SIZETABLE_END);*/
    struct size_table_elem* x[22];
    //should resize and then fill with 2 more after resize
    for(int i = 0; i < 22; i++){
        x[i] = create_sizetable_elem(15000*i);
        printf("%p\n", x[i]);
    }
    printf("\n\n%p\n", __SHEAP_SIZETABLE_START);
    printf("%p\n\n\n", __SHEAP_SIZETABLE_END);
    printf("Ensuring correct allocations and stuff\n");
    for(int i = 0; i < 22; i++){
        void* v = get_sizetable_freeptr(x[i], 15000*i);
        printf("%p\n", v);
        v = get_sizetable_freeptr(x[i], 0);//should be null pointer besides first 2 iterations of loop
        printf("%p\n", v);
    }
    printf("\n\n");
    int class = get_sizeclass_index(131073);
    printf("%d", class);
}
