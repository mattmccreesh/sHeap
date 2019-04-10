#include "sizetable.h"
#include <stdio.h>
#include <stdlib.h>

extern void* __SHEAP_SIZETABLE_START;
int main()
{
    initialize_sizetable(20);
    printf("%p\n", __SHEAP_SIZETABLE_START);
}
