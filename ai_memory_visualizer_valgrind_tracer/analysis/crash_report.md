# Crash Report — 4-crash.c

## The crash

```c
size_t size = 5 * 1024 * 1024;
int *big = malloc(size);
...
big[0] = 42;
printf("big[0] before free: %d\n", big[0]);

free(big);

big[0] = 1337;
printf("big[0] after free: %d\n", big[0]);
```

Running `./bin/4-crash` directly:

```
big[0] before free: 42
Segmentation fault (core dumped)
```

Confirmed the exit status too, not just the printed message (a wrapper script that checks
`$?` after the run):

```
exit status was: 139
killed by signal: 11
```

139 = 128 + 11, signal 11 is `SIGSEGV`. So this is a real, OS-level segmentation fault, not
something Valgrind invented (see the note at the end of `valgrind_analysis.md` about how
this same program behaves differently when it *is* run under Valgrind — worth reading, but
this report is about the native crash).

## Root cause, step by step

1. `malloc(5 * 1024 * 1024)` requests 5 MiB. glibc's malloc has a threshold (`M_MMAP_THRESHOLD`,
   128 KiB by default) above which it does not carve the block out of the normal heap
   arena (the `brk`-managed region used for small allocations). Instead it asks the kernel
   directly for a private anonymous mapping via `mmap()`. This request is well above that
   threshold, so `big` points into its own dedicated mapping, not the regular heap.
   (This also explains why `big`'s address, `0x7ffff76ff010` from gdb below, looks nothing
   like the small-malloc addresses in `memory_maps.md`, e.g. `0x555555559010` — different
   region entirely.)
2. `big[0] = 42;` writes into that mapping. Totally valid, the page is mapped and
   writable.
3. `free(big);` — for a normal small chunk, glibc's `free()` just marks the chunk free and
   links it into a freelist for reuse; the underlying pages usually stay mapped. But for an
   `mmap`-backed allocation like this one, `free()` calls `munmap()` on the whole region
   instead, immediately, unconditionally. The mapping is gone the moment `free()` returns.
4. `big[0] = 1337;` dereferences `big`, which still holds the old virtual address, but that
   address no longer has *any* page table entry backing it. This is not "the value might
   be stale" the way a small-chunk use-after-free often is. There is nothing there at all.
   The CPU's MMU raises a page fault on the write, the kernel checks the process's memory
   map, finds no mapping for that address, and delivers `SIGSEGV` instead of resolving the
   fault. That's why the crash happens exactly here and not one line earlier.

## Confirming it isn't guesswork

Ran it under gdb instead of just trusting the reasoning above:

```
Program received signal SIGSEGV, Segmentation fault.
0x00005555555551f1 in main () at programs/4-crash.c:17
17          big[0] = 1337;
#0  0x00005555555551f1 in main () at programs/4-crash.c:17
        size = 5242880
        big = 0x7ffff76ff010
$1 = (int *) 0x7ffff76ff010
x/4xg big
0x7ffff76ff010: Cannot access memory at address 0x7ffff76ff010
```

Two things this confirms directly, not just in theory:

- The fault is at line 17 exactly, `big[0] = 1337;`, matching step 4 above.
- `x/4xg big` (asking gdb to just read 4 giant-words starting at `big`) fails with
  `Cannot access memory at address 0x7ffff76ff010`. That is gdb, running with full
  debugger privileges, also unable to read that address. It's not a permissions thing
  or an alignment thing, the address genuinely has no backing page anymore, confirming
  step 3 (the region really was unmapped, not just marked invalid by some higher-level
  check).

## Category of undefined behavior

This is a **use-after-free**: an access to heap memory through a pointer whose object's
lifetime already ended (`free()` ends the lifetime of the block `big` points to). What
makes this particular use-after-free crash **every single time**, deterministically,
rather than sometimes-corrupt-sometimes-fine like a lot of use-after-free bugs, is the
allocation size crossing the mmap threshold. Root cause in one sentence: the code writes
to memory after the specific `free()` call that, for this allocation size, unmaps the page
backing that memory, so the write always faults.

## Where an AI explanation was right, and where it wasn't

Before writing the two sections above I asked an AI for a general take on this code. Two
different claims came back, one useful, one that needed correcting:

**Correct and useful:** it identified the bug as a use-after-free and pointed at
`free(big)` on line 15 as the point after which line 17 becomes invalid. That's right and
matches the analysis above.

**Wrong / incomplete:** it then said, roughly, "this is undefined behavior so the crash is
not guaranteed, whether it segfaults depends on whether the freed memory has been reused
by another allocation in the meantime." That is the correct general mental model for a
*typical small* use-after-free (like if this had been `malloc(16)` instead of 5 MiB), but
it's the wrong explanation for *this specific program*, and I only caught that by actually
running it, repeatedly, and it segfaulted every time, plus checking with gdb that the page
is really gone rather than "maybe still there, maybe not." The AI's answer treated "heap
memory" as one uniform thing, but glibc doesn't; small and large allocations go through
different code paths with different consequences when freed, and the size chosen here
(5 MiB, well past the mmap threshold) was specifically picked to land in the deterministic
case rather than the "depends on reuse" case. An explanation that's only correct for the
general case isn't automatically correct for the specific 12 lines of code being asked
about, that's the gap.

## Suggested fix (labeled as such — not required by the task, just noting it)

Don't touch `big` after `free(big)`. If the value at `big[0]` is still needed after
freeing, read it before the `free()` call and store it in a local, e.g.:

```c
int last_value = big[0];
free(big);
printf("big[0] after free: %d\n", last_value);
```

Setting `big = NULL;` right after `free(big);` would not by itself prevent this exact bug
(the code would then be dereferencing `NULL`, a different, also-invalid access), but it is
still good practice in general since it turns a silent/inconsistent use-after-free into an
immediate, obvious null-pointer crash on the very next use, which is easier to debug than
an intermittent one.
