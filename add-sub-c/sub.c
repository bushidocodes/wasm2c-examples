#include <stdint.h>

uint32_t sub(uint32_t a, uint32_t b) __attribute__((export_name("sub")))
{
    return a - b;
}