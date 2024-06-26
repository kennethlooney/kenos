#include <stdio.h>
#include "efi/efi.h"

int main(int argc, char** argv)
{
    int c = add_test(10,10);
    printf("Answer = %d\n", c);
    return 0;
}