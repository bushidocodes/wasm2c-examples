#include <stdio.h>
#include <stdlib.h>

/* Uncomment this to define fac_init and fac_Z_facZ_ii instead. */
#define WASM_RT_MODULE_PREFIX add_
#include "add.w2c.h"

#define WASM_RT_MODULE_PREFIX sub_
#include "sub.w2c.h"

int main(int argc, char **argv)
{
    /* Make sure there are two command-line arguments. */
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <u32> <u32>\n", argv[0]);
        return 1;
    }

    u32 a = atoi(argv[1]);
    u32 b = atoi(argv[2]);

    /* Initialize the add module. Since we didn't define WASM_RT_MODULE_PREFIX,
    the initialization function is called `init`. */
    add_init();
    sub_init();

    /* Call `add`, using the mangled name. */
    u32 result = add_Z_addZ_iii(a, b);
    u32 result2 = sub_Z_subZ_iii(a, b);

    /* Print the result. */
    printf("add(%u, %u) -> %u\n", a, b, result);
    printf("sub(%u, %u) -> %u\n", a, b, result2);

    return 0;
}