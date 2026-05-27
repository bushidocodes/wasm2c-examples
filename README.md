# wasm2c-examples

Demonstrates two ways to use [wasm2c](https://github.com/WebAssembly/wabt/tree/main/wasm2c) — the tool that converts WebAssembly bytecode into portable C code — so the result can be compiled and run as a native executable without a WASM runtime.

Both examples implement the same two functions (`add` and `sub`) and call them from a C host program.

| Example | Source language | Toolchain path |
|---------|----------------|----------------|
| `add-sub-c` | C | `C → WASM → C → native` |
| `add-sub-wat` | WAT (WebAssembly Text) | `WAT → WASM → C → native` |

## How it works

```
add.c ──┐                        add.wat ──┐
sub.c ──┤→ .wasm → wasm2c → .w2c.c        ├→ .wasm → wasm2c → .w2c.c
        │                        sub.wat ──┘
        └─ main.c ──────────────────────────────┐
                                                ↓
                                    clang → add-sub (native exe)
```

`wasm2c` translates a binary WebAssembly module into C code. The generated C file, together with the small runtime in `common/`, links into an ordinary native binary. No WASM runtime is needed at execution time.

The `common/` directory contains the wasm-rt runtime support files from [wabt](https://github.com/WebAssembly/wabt), which provide memory management, trap handling, and table operations for the generated code.

## Prerequisites

### 1. LLVM with WebAssembly target support

The standard [LLVM release](https://releases.llvm.org/) (version 9+) includes the `wasm32` target. The LLVM bundled with Visual Studio does **not** include the WebAssembly target and cannot be used for the `C → WASM` step.

On Windows, install via winget:
```
winget install LLVM.LLVM
```

On macOS:
```
brew install llvm
```

On Linux (Debian/Ubuntu):
```
apt install clang lld
```

### 2. wabt (WebAssembly Binary Toolkit)

Provides `wasm2c` and `wat2wasm`. Download the latest release for your platform from the [wabt releases page](https://github.com/WebAssembly/wabt/releases) and place it in `tools/`:

```
mkdir tools
# Download and extract to tools/wabt-<version>/
```

The Makefiles default to `tools/wabt-1.0.41`. Override with `WABT_DIR=path/to/wabt`.

### 3. GNU Make

- **macOS/Linux**: typically pre-installed (`make --version`)
- **Windows**: install [GnuWin32 make](https://gnuwin32.sourceforge.net/packages/make.htm) or use WSL

## Building

```sh
# add-sub-c: C source → WASM → C → native
cd add-sub-c
make WASM_CLANG=/path/to/llvm/clang CC=/path/to/clang

# add-sub-wat: WAT source → WASM → C → native
cd add-sub-wat
make WASM_CLANG=/path/to/llvm/clang CC=/path/to/clang
```

### Makefile variables

| Variable | Default | Purpose |
|----------|---------|---------|
| `WASM_CLANG` | `clang` | Clang with wasm32 target (for C→WASM step) |
| `CC` | `clang` | Native C compiler (for final binary) |
| `WASM2C` | `$(WABT_DIR)/bin/wasm2c` | wasm2c tool |
| `WAT2WASM` | `$(WABT_DIR)/bin/wat2wasm` | wat2wasm tool (`add-sub-wat` only) |
| `WABT_DIR` | `../tools/wabt-1.0.41` | Root of wabt installation |

## Running

```
./add-sub <a> <b>
```

```
$ ./add-sub 10 3
add(10, 3) -> 13
sub(10, 3) -> 7
```

## Project layout

```
wasm2c-examples/
├── common/               # wasm-rt runtime (from wabt)
│   ├── wasm-rt.h
│   ├── wasm-rt-impl.h
│   ├── wasm-rt-impl.c
│   ├── wasm-rt-impl-tableops.inc
│   ├── wasm-rt-mem-impl.c
│   └── wasm-rt-mem-impl-helper.inc
├── add-sub-c/            # C → WASM → C example
│   ├── Makefile
│   ├── add.c
│   ├── sub.c
│   └── main.c
├── add-sub-wat/          # WAT → WASM → C example
│   ├── Makefile
│   ├── add.wat
│   ├── sub.wat
│   └── main.c
└── tools/                # downloaded wabt binaries (gitignored)
```

Generated files (`*.wasm`, `*.w2c.c`, `*.w2c.h`, `*.o`, `add-sub`) are gitignored.

## Generated API

`wasm2c --module-name <name>` generates a clean, instance-based C API:

```c
// Declare an instance
w2c_add add_inst;

// Initialize the runtime (once per process)
wasm_rt_init();

// Instantiate the module
wasm2c_add_instantiate(&add_inst);

// Call an exported function
u32 result = w2c_add_add(&add_inst, 10, 3);

// Clean up
wasm2c_add_free(&add_inst);
wasm_rt_free();
```

Multiple module instances can coexist since all state is in the instance struct.
