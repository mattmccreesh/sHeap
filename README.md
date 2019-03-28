# sHeap
Creating a secure heap allocator resistant to use after free vulnerabilities. Inspired by the Cling allocator paper

## Usage

The first step is to run `make` at the top level in order to compile the shared object file and the test file.

Once we have everything compiled, we need to set up the environment to run with our own implementations.

`LD_PRELOAD` is an environment variable that should be used to point to the `sheap.so` shared object file.

For example, if you are at the top of the repo you can run with: 
```
LD_PRELOAD=$PWD/sheap.so ./runner
```

## Files

`main.c` - This is a test file that we can (should) use to make sure things are working

`sheap.c` - This is the main file that houses our implementations of `malloc`, `calloc`, `realloc`, and `free`. This should be compiled as a shared object file and preloaded to be used at runtime.

