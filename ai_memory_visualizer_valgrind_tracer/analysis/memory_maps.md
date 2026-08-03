# Memory Maps

For this task I wrote 5 small C programs instead of reusing something already written,
because I wanted to control exactly which memory behaviors show up (aliasing, recursion,
a leak, an overflow, an uninitialized read, a real crash). They live in `programs/` and
build with the `Makefile` (`make re` builds everything into `bin/`).

To build the maps below I didn't just eyeball the source and guess. For each program I:

1. Compiled it with `make re` and ran it once normally (no valgrind), just to see what it
   prints.
2. Asked an AI (I used it the same way the task says: as a first draft generator) to
   describe the stack/heap state at a few points in the program.
3. Then actually stepped through the same points with `gdb` (batch mode, breakpoints with
   `printf` commands attached) and compared the real addresses/values against what the AI
   said. Where they didn't match, I went with gdb, because gdb is looking at the actual
   running process and the AI is just pattern-matching on the C code.

The main program for this task is `0-clean_demo.c` since it's the one that isn't buggy and
is meant to show the "normal" model clearly. The other 4 are covered briefly here and in
much more depth in `valgrind_analysis.md` and `crash_report.md`.

## 0-clean_demo.c

```c
char *duplicate(const char *src)
{
    char *copy = malloc(strlen(src) + 1);
    ...
    strcpy(copy, src);
    return (copy);
}

int sum_range(int n)
{
    if (n <= 0)
        return (0);
    return (n + sum_range(n - 1));
}

int main(void)
{
    char *original = duplicate("holberton");
    char *alias = original;
    ...
    alias[0] = 'H';
    total = sum_range(4);
    ...
    free(original);
    original = NULL;
    ...
}
```

### Stack vs heap, concretely

There is exactly **one** heap allocation in this program: the `malloc(strlen(src) + 1)`
inside `duplicate`. Everything else (`original`, `alias`, `total`, `n` in every recursive
call, `src` and `copy` inside `duplicate`) is a stack variable that lives inside some
function's frame.

Real addresses from gdb (breakpoints right after the interesting lines, `run`):

```
[duplicate] src="holberton" &src=0x7fffffffe1c8 copy(heap)=0x555555559010
[main after assign] original=0x555555559010 alias=0x555555559010 &original=0x7fffffffe200 &alias=0x7fffffffe208
```

Notice the two different kinds of addresses:

- `0x7fffffffe1c8`, `0x7fffffffe200`, `0x7fffffffe208` — these are all in the `0x7ffff...`
  range, which on this machine/build is the **stack**. `&src` (a parameter of `duplicate`)
  and `&original`/`&alias` (locals of `main`) are stack slots, and they are 8 bytes apart
  from each other exactly because they're two adjacent `char *` (8 bytes each on x86-64)
  in `main`'s frame.
- `0x555555559010` is the **heap**. It's the value `malloc` returned, and it's the same
  numeric value stored in both `original` and `alias`.

So `original` and `alias` are two *different stack variables* (different addresses,
`0x7fffffffe200` vs `0x7fffffffe208`) that happen to *hold the same heap address*. That's
what "aliasing" means here: two names, one object. `alias[0] = 'H'` doesn't touch
`alias`'s own stack slot, it dereferences the heap address stored in it and writes to the
heap. So the change shows up through `original` too, since `original` points at the exact
same byte:

```
after mutation through alias, original: Holberton
```

### Lifetime of the heap block

The block `duplicate` allocates does not belong to `duplicate`'s stack frame. It's created
with `malloc`, so its lifetime is controlled entirely by `malloc`/`free` calls, not by
which function is currently running. `duplicate` returns and its frame is gone, but the
10-byte block (`strlen("holberton") + 1`) it allocated is still alive on the heap,
referenced first by `copy` (while `duplicate` was running) and then by `original` and
`alias` in `main` (after `duplicate` returns and copies the pointer value out). The block's
life ends only at `free(original)` on line 54, when `main` explicitly frees it. Nothing
about returning from `duplicate` deallocates it, that's the whole point of using the heap
instead of a local array.

Ownership here is simple because there's only one allocation: `main` is the owner (it's
the one that eventually calls `free`), even though `duplicate` is the one that created the
block. Whoever calls `free` is the owner at the time of freeing, and in this program that's
`main`.

### Recursion and stack growth

`sum_range` calls itself 5 times (`n = 4, 3, 2, 1, 0`). Each call is a brand new stack
frame with its own copy of `n`. Real addresses:

```
[sum_range] n=4 &n(stack)=0x7fffffffe1dc
[sum_range] n=3 &n(stack)=0x7fffffffe1bc
[sum_range] n=2 &n(stack)=0x7fffffffe19c
[sum_range] n=1 &n(stack)=0x7fffffffe17c
[sum_range] n=0 &n(stack)=0x7fffffffe15c
```

The address of `n` decreases by exactly `0x20` (32 bytes) every call. That's the stack
growing downward (toward lower addresses) as each new frame is pushed, which matches how
the x86-64 stack works on Linux. Each `n=4`'s frame is not destroyed until `sum_range(3)`
returns, so at the deepest point (`n=0`) there are 5 separate `n` variables alive at once,
stacked on top of each other, each with a different address. As the calls return (from
`n=0` back up to `n=4`), the frames pop in reverse order and those addresses become invalid
again — `sum_range(4)`'s `n` is the last one still valid, until the whole call returns
`total = 10` to `main`.

### What happens after `free`

```
[main before free] original=0x555555559010 alias=0x555555559010
[main after free, before NULL] original=0x555555559010 (dangling) alias=0x555555559010 (dangling)
[main after original=NULL] original=(nil) alias=0x555555559010 (alias NOT nulled, still dangling)
```

`free(original)` releases the heap block back to the allocator. At that exact instant,
*both* `original` and `alias` become dangling pointers, because dangling-ness is a property
of the value they hold (an address whose backing memory is no longer valid), not something
attached to one variable. The very next line, `original = NULL;`, only touches the
`original` variable. It has zero effect on `alias`, which still holds
`0x555555559010` and is still just as dangling as before. This is confirmed directly in
the gdb trace above.

This is exactly the kind of thing that's easy to get wrong if you reason about it without
checking, so it's the AI mistake I documented below.

One more small thing worth being precise about: the final `printf` in the program prints
`alias`'s value with `%p` but never dereferences it (`(void *)alias`, not `*alias`). Printing
a dangling pointer's numeric value doesn't touch the freed memory, so it does not crash and
Valgrind does not flag it (see `valgrind_analysis.md`, the `0-clean_demo` section is clean).
Strictly speaking the C standard treats even *using the value* of a pointer whose target has
been freed as undefined behavior, not just dereferencing it, so this line is technically not
100% portable-safe either, but in practice on every real compiler this is harmless. I'm
flagging it so I'm not accidentally implying "only dereferencing a dangling pointer is
dangerous."

### Where the AI explanation was wrong

I asked an AI to explain what happens to `original` and `alias` after `free(original);
original = NULL;`. First answer I got (paraphrased, this is the kind of thing these tools
say):

> After `free(original)` the heap memory holding "holberton" is deallocated. Then
> `original = NULL;` clears the original pointer. At this point both `original` and
> `alias` no longer reference the freed memory, since they were pointing to the same
> block and the block has been marked invalid, so using either pointer afterward is safe
> since the block is gone.

This is wrong in a way that matters. Two separate problems:

1. It says "both `original` and `alias` no longer reference the freed memory" — false.
   Only `original` was reassigned. `alias` still holds `0x555555559010`, the exact same
   address, forever (or until something else overwrites that stack slot). The gdb trace
   above proves this directly.
2. It says using either pointer afterward is "safe since the block is gone" — this is
   backwards. The block being "gone" (freed) is exactly *why* using `alias` afterward is
   unsafe, not why it's safe. Dereferencing `alias` after this point would be a
   use-after-free, not a protected operation. `free()` doesn't unmap or zero out small
   heap blocks like this one (see the `4-crash.c` discussion in `crash_report.md` for a
   case where a much bigger allocation actually does get unmapped) — the bytes are very
   likely still sitting there unchanged, which is what makes use-after-free bugs so sneaky:
   they often don't crash, they just quietly hand you stale data.

The corrected version is what's written in the "What happens after free" section above:
`free` invalidates the *block*, both pointers are dangling as of that call, and the later
`original = NULL` only ever changes `original`. `alias` was never touched and is not
automatically cleaned up by anything in this program.

## 1-leak.c, 2-invalid_write.c, 3-uninitialized.c, 4-crash.c

These four are covered in detail in the other two files since their whole point is a
specific bug, but here's the quick stack/heap picture for each, also checked with gdb:

- **1-leak.c**: one heap block (64 bytes from `malloc(64)` in `build_message`), pointer
  returned by value into `main`'s `greeting`. `build_message`'s local `msg` and `main`'s
  `greeting` are different stack slots (`&msg=0x7fffffffe1f8` vs
  `&greeting=0x7fffffffe218`) holding the same heap address `0x555555559010`. `msg`'s
  frame disappears when `build_message` returns, but that's fine, `greeting` already has
  its own copy of the address. The bug isn't there, it's that nothing ever calls
  `free(greeting)`, so `greeting` itself (the last surviving reference) dies when `main`
  returns, and the block becomes permanently unreachable while still marked allocated.
  That's the leak, see `valgrind_analysis.md`.

- **2-invalid_write.c**: one heap block, `malloc(5 * sizeof(int))` = 20 bytes, for indices
  0..4. The loop bound is `i <= 5`, so on the 6th iteration `i` is 5 and the program
  writes to `arr[5]`, 4 bytes past the end of the block. gdb confirms `&arr[5] =
  0x555555559024`, i.e. exactly `0x14` (20) bytes after the base `0x555555559010` — that's
  the first byte past the allocation.

- **3-uninitialized.c**: one heap block, `malloc(sizeof(int))`, never written to before
  `if (*flag)` reads it. In my run gdb showed the leftover value at that address was `0`
  (`*flag(garbage)=0`), which is why the program printed "flag was not set". That's not
  a guarantee, just what happened to be sitting in that particular page this run (see
  `valgrind_analysis.md` for why this matters).

- **4-crash.c**: covered fully in `crash_report.md`. Short version: one 5 MB heap block,
  freed, then written to again. Because of the size, this block is not on the normal
  small-object heap, it's backed by its own `mmap` region, and the two addresses shown
  by gdb (`big = 0x7ffff76ff010`, in the `0x7ffff7...` range) are visibly different from
  the `0x555555559010`-style addresses of the small mallocs above, which is itself a clue
  about the two different allocation strategies glibc uses.
