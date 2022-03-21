%.wasm: %.c 
	clang --target=wasm32 -nostdlib -Wl,--no-entry $^ -o $@

%.w2c.c: %.wasm
	 wasm2c $< -o $@

add.o: add.w2c.c
	clang -DWASM_RT_MODULE_PREFIX=add_ -c $^ -o $@

sub.o: sub.w2c.c
	clang -DWASM_RT_MODULE_PREFIX=sub_ -c $^ -o $@

a.out: ../common/wasm-rt-impl.c main.c add.o sub.o
	clang -I../common $^ -o $@

clean:
	rm -f *.w2c.c
	rm -f *.w2c.h
	rm -f *.o
	rm -f *.out