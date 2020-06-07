# sHeap
Creating a secure heap allocator resistant to use after free vulnerabilities. Inspired by the Cling allocator paper

## Usage
The first step is to run `make` at the top level (the sHeap folder) in order to compile the shared object file and the test file.

Once we have everything compiled, we need to set up the environment to run with our own implementations.

`LD_PRELOAD` is an environment variable that should be used to pokkint to the `sheap.so` shared object file.

For example, if you are at the top of the repo you can run with: 
```
LD_PRELOAD=$PWD/sheap.so ./runner
```

## Dependencies
Libpthread was used, and generally exists on most systems
Libunwind, was used, and can be installed on Ubuntu with sudo apt-get install libunwind-dev

## Files

`main.c` - This is a test file that we can (should) use to make sure things are working. It compiles to its own exececutable unrelated to sheap.

`sheap.c` - This is the main file that houses our implementations of `malloc`, `calloc`, `realloc`, and `free`. This should be compiled as a shared object file and preloaded to be used at runtime.

`sizetable.c` - Implements data structure to track size classes for each allocation site, having pointer to head freelist pointer for each size class. Also tracks if callsite is malloc wrapper

`flist.c` - Maintains and updates free lists of blocks per size class per allocation site. Resues free blocks or calls system call to allocate more space

`pool_hash_table.c` - This is the hash table to map allocation sites to pools. Pools are defined by the sizetable for the pool, so this maps alloc site to a sizetable element which in turn stores head pointers for size classes for that call site.

`util.c` - This defines util functions for allocations, system calls, and debugging utilities.  For example, it allocates memory with sys call. It also provides printing utilities as printf uses malloc and should not be used for debugging within malloc

## Results
This resulted in a very well received papare and presentation (adding this a year later as I'm testing some git permissions to diagnose an issue I"m having)
