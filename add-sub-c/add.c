#include <stdint.h>

uint32_t add(uint32_t a, uint32_t b) __attribute__((export_name("add")))
{
    return a + b;
}