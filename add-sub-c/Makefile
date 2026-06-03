# Tool paths — override these if your tools are installed elsewhere.
# WASM_CLANG: a clang that can target wasm32 (standard LLVM release)
# CC:         the native C compiler for the final binary
# WASM2C:     wasm2c from the wabt toolkit
WABT_DIR  ?= ../tools/wabt-1.0.41
WASM_CLANG ?= clang
CC         ?= clang
WASM2C     ?= "$(WABT_DIR)/bin/wasm2c"

UNITY_DIR    = ../common/unity
RUNTIME_SRCS = ../common/wasm-rt-impl.c ../common/wasm-rt-mem-impl.c
CFLAGS       = -I../common -I$(UNITY_DIR)

.PHONY: all test clean

all: test_add_sub

%.wasm: %.c
	"$(WASM_CLANG)" --target=wasm32 -nostdlib -Wl,--no-entry $^ -o $@

%.w2c.c: %.wasm
	$(WASM2C) $< --module-name $(basename $<) -o $@

add.o: add.w2c.c
	$(CC) $(CFLAGS) -c $^ -o $@

sub.o: sub.w2c.c
	$(CC) $(CFLAGS) -c $^ -o $@

test_add_sub: test_add_sub.c add.o sub.o
	$(CC) $(CFLAGS) $^ $(RUNTIME_SRCS) $(UNITY_DIR)/unity.c -o $@

test: test_add_sub
	./test_add_sub

clean:
	rm -f *.wasm *.w2c.c *.w2c.h *.o test_add_sub
