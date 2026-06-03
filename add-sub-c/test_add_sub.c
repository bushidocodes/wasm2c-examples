#include <stdint.h>

/* wasm-rt.h pulls in windows.h on Windows; it must come before unity.h
   because Unity (C11 mode) includes <stdnoreturn.h> which redefines
   `noreturn` as `_Noreturn`, breaking __declspec(noreturn) in SDK headers. */
#include "wasm-rt.h"
#include "add.w2c.h"
#include "sub.w2c.h"

#include "unity.h"

static w2c_add add_inst;
static w2c_sub sub_inst;

void setUp(void)
{
    wasm2c_add_instantiate(&add_inst);
    wasm2c_sub_instantiate(&sub_inst);
}

void tearDown(void)
{
    wasm2c_add_free(&add_inst);
    wasm2c_sub_free(&sub_inst);
}

void test_add_basic(void)
{
    TEST_ASSERT_EQUAL_UINT32(13, w2c_add_add(&add_inst, 10, 3));
}

void test_add_identity(void)
{
    TEST_ASSERT_EQUAL_UINT32(42, w2c_add_add(&add_inst, 42, 0));
    TEST_ASSERT_EQUAL_UINT32(0,  w2c_add_add(&add_inst, 0,  0));
}

void test_add_overflow_wraps(void)
{
    TEST_ASSERT_EQUAL_UINT32(0,           w2c_add_add(&add_inst, UINT32_MAX, 1));
    TEST_ASSERT_EQUAL_UINT32(2200000000U, w2c_add_add(&add_inst, 2000000000U, 200000000U));
}

void test_sub_basic(void)
{
    TEST_ASSERT_EQUAL_UINT32(7, w2c_sub_sub(&sub_inst, 10, 3));
}

void test_sub_identity(void)
{
    TEST_ASSERT_EQUAL_UINT32(42, w2c_sub_sub(&sub_inst, 42, 0));
    TEST_ASSERT_EQUAL_UINT32(0,  w2c_sub_sub(&sub_inst, 5,  5));
}

void test_sub_underflow_wraps(void)
{
    TEST_ASSERT_EQUAL_UINT32(UINT32_MAX, w2c_sub_sub(&sub_inst, 0, 1));
}

int main(void)
{
    wasm_rt_init();
    UNITY_BEGIN();
    RUN_TEST(test_add_basic);
    RUN_TEST(test_add_identity);
    RUN_TEST(test_add_overflow_wraps);
    RUN_TEST(test_sub_basic);
    RUN_TEST(test_sub_identity);
    RUN_TEST(test_sub_underflow_wraps);
    int result = UNITY_END();
    wasm_rt_free();
    return result;
}
