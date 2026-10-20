# arena_allocator

A tiny arena allocator in C, in a single header. Allocate as much as you want, never free anything one by one, then release everything with one call.

## Why an arena

+ malloc and free per object are slow, and forgetting one free leaks memory.
+ An arena hands out memory from big blocks it owns. Freeing is one call that releases every block at once.
+ It fits programs whose data lives until the end: compilers, parsers, games built around a frame, command line tools.

## API

+ arena_alloc(count, size): returns zeroed memory for count items of size bytes, like calloc. Never returns NULL: it exits with a message when the system is out of memory.
+ arena_reserve(size): makes sure the next size bytes fit in the current block, so a big batch of allocations doesn't start a new block halfway.
+ arena_free(): releases every block, and returns how many bytes were used, block headers included.

## Usage

+ Copy header.h into your project and include it.
+ Allocate with arena_alloc(10, sizeof(int)) and so on, as often as you like.
+ At the end of the program, call arena_free() once.
+ Build: gcc your_file.c (header.h holds the implementation, there is nothing else to compile).
+ The example in main.c counts the words of a sentence. Every word copy and every list node comes from the arena, and one arena_free() at the end releases them all. Run it with: gcc main.c && ./a.out

## How it works

+ Memory comes from blocks chained in a linked list. Each block knows its size and how much of it is used.
+ An allocation takes the next free bytes of the current block. Every size is rounded up to 16 bytes, so every pointer is 16-byte aligned.
+ When a request doesn't fit, a new block is chained after the current one. The new block is twice as big as the previous one (256 bytes for the first), and bigger if the request needs it.
+ Blocks come from calloc, so all memory starts at zero.

## Limits

+ No single free: memory is only released all at once.
+ Not thread safe: the arena is one global list.
+ header.h defines its functions, so include it in only one .c file of a program.

## History

This repository started as Garbage_collector, a list of every allocation freed at the end. It is now an arena: fewer calls to malloc, and no list to walk per allocation.

## Author

Mohammed Hrima
