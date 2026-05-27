#include <stdio.h>
#include <stdlib.h>

#include "wasm-rt.h"
#include "add.w2c.h"
#include "sub.w2c.h"

int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <u32> <u32>\n", argv[0]);
        return 1;
    }

    u32 a = (u32)atoi(argv[1]);
    u32 b = (u32)atoi(argv[2]);

    wasm_rt_init();

    w2c_add add_inst;
    w2c_sub sub_inst;
    wasm2c_add_instantiate(&add_inst);
    wasm2c_sub_instantiate(&sub_inst);

    u32 result_add = w2c_add_add(&add_inst, a, b);
    u32 result_sub = w2c_sub_sub(&sub_inst, a, b);

    printf("add(%u, %u) -> %u\n", a, b, result_add);
    printf("sub(%u, %u) -> %u\n", a, b, result_sub);

    wasm2c_add_free(&add_inst);
    wasm2c_sub_free(&sub_inst);
    wasm_rt_free();

    return 0;
}
