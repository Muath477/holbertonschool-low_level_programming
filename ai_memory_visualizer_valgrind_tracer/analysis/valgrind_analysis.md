# Valgrind Analysis

Ran all 5 programs under:

```
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/<program>
```

`--track-origins=yes` costs some speed but it's the only reason the uninitialized-value
report below actually points at the `malloc` call instead of just saying "somewhere". Full
logs are saved in `analysis/raw/*.valgrind.txt` if needed, this file only quotes the parts
that matter.

## 0-clean_demo.c — clean

```
==458== HEAP SUMMARY:
==458==     in use at exit: 0 bytes in 0 blocks
==458==   total heap usage: 2 allocs, 2 frees, 4,106 bytes allocated
==458== All heap blocks were freed -- no leaks are possible
==458== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

Worth pointing out explicitly: this program still has a dangling pointer sitting around at
the end (`alias`, see `memory_maps.md`), and Valgrind says nothing about it. That's not a
gap in Valgrind, it's correct behavior. Valgrind's memcheck only flags a pointer when it's
actually *used* in a way that touches invalid memory (a read, a write, or a `free`), or
when memory is unreachable at exit (a leak). Just holding a stale address in a variable
that never gets dereferenced or freed again isn't, by itself, something a runtime checker
can catch, because nothing observably wrong happens. It's a landmine, not (yet) an
explosion.

## 1-leak.c — memory leak, definitely lost

```
==459== 64 bytes in 1 blocks are definitely lost in loss record 1 of 1
==459==    at 0x4850858: malloc (vg_replace_malloc.c:447)
==459==    by 0x40011A2: build_message (1-leak.c:12)
==459==    by 0x40011F4: main (1-leak.c:23)
==459== LEAK SUMMARY:
==459==    definitely lost: 64 bytes in 1 blocks
```

**Type:** memory leak, "definitely lost" category specifically.

**What it means:** at program exit, Valgrind walks every pointer still reachable from CPU
registers, the stack, and global variables, and checks whether any of them still point
into a heap block it's tracking. "Definitely lost" is the strongest category, it means it
found *zero* references left anywhere to this 64-byte block. Not "possibly lost" (some
pointer to the middle of the block, ambiguous), not "still reachable" (something global
still points to it, sloppy but not technically lost). Just gone.

**Why it happens here:** `build_message` (line 12) allocates 64 bytes and returns the
pointer. `main` (line 23) stores that pointer in `greeting`. `greeting` is a stack
variable, so its lifetime is `main`'s frame. `main` never calls `free(greeting)`, and when
`main` returns, `greeting` (the only variable anywhere holding that heap address) goes
out of scope. The heap block is still marked "allocated" from the allocator's point of
view, but there is no longer any way for the program to reach it. That's a leak due to
lost ownership: `build_message` handed ownership to `main` by returning the pointer, and
`main` dropped it without ever freeing.

## 2-invalid_write.c — heap buffer overflow (off-by-one)

```
==460== Invalid write of size 4
==460==    at 0x40011D3: main (2-invalid_write.c:13)
==460==  Address 0x4a91054 is 0 bytes after a block of size 20 alloc'd
==460==    at 0x4850858: malloc (vg_replace_malloc.c:447)
==460==    by 0x400119E: main (2-invalid_write.c:6)
```

**Type:** invalid write / heap buffer overflow, caused by an off-by-one loop bound.

**What it means:** `Invalid write of size 4` means a 4-byte store (an `int`) landed
outside memory Valgrind considers valid for this pointer. `0 bytes after a block of size
20` is Valgrind being precise about exactly how far out of bounds: not "somewhere in
garbage memory", but the very first byte past the end of a 20-byte block. That number
matches the code exactly: `malloc(5 * sizeof(int))` is 20 bytes for indices 0..4, and the
loop is `for (i = 0; i <= 5; i++)`, so the last iteration writes `arr[5]`, 20 bytes past
the base, i.e. immediately after the block. Confirmed independently with gdb in
`memory_maps.md`: `&arr[5] = 0x555555559024`, and `0x555555559024 - 0x555555559010 =
0x14 = 20`.

**Why it doesn't crash:** the 4 extra bytes almost always land inside the allocator's
own bookkeeping for the next chunk (or in unused padding), not in unmapped memory, so the
write "succeeds" and the program's own output looks fine (`0 2 4 6 8`, all correct). That's
the actual danger here: this is a bug that corrupts heap metadata silently and would very
likely blow up somewhere completely unrelated later (e.g. a later, unrelated `free()`
aborting with a "corrupted size" message), which is exactly why relying on "did the output
look right" is not enough and Valgrind (or ASan) is needed to catch it at the point where
it actually happens.

## 3-uninitialized.c — use of uninitialized value

```
==461== Conditional jump or move depends on uninitialised value(s)
==461==    at 0x40011B9: main (3-uninitialized.c:11)
==461==  Uninitialised value was created by a heap allocation
==461==    at 0x4850858: malloc (vg_replace_malloc.c:447)
==461==    by 0x400119E: main (3-uninitialized.c:6)
```

**Type:** read of uninitialized heap memory, used in a branch condition.

**What it means:** `malloc` (unlike `calloc`) makes no promise about the contents of the
memory it returns, it can be leftover data from something else that used to occupy those
pages. `if (*flag)` at line 11 branches on that value without `flag` ever having been
written to. `--track-origins=yes` is why the report can say the value "was created by a
heap allocation" at line 6, it's tracking where the uninitialized bytes came from, not just
that they exist.

One thing I actually checked instead of assuming: in my run, gdb showed `*flag` was `0` at
the breakpoint (see `memory_maps.md`), which is why the program printed "flag was not set"
and *looked* correct. That's the point of this bug though, not a counter-argument against
it being one. It happened to be 0 this time because this was the process's first heap
allocation and the underlying page came fresh from the kernel, which zero-fills new pages.
That's a coincidence of this run's allocation pattern, not a guarantee, and Valgrind is
right to flag it regardless of what the actual bit pattern happens to be, because it's
tracking whether a write ever happened, not what value is currently sitting there. A
different allocation history (or a different malloc implementation, or a build without
libc zeroing fresh pages) could just as easily produce a nonzero value here and take the
other branch.

## 4-crash.c — heap use-after-free (write and read), plus a behavior mismatch

```
==465== Invalid write of size 4
==465==    at 0x40011F1: main (4-crash.c:17)
==465==  Address 0x4a91040 is 0 bytes inside a block of size 5,242,880 free'd
==465==    at 0x48538BF: free (vg_replace_malloc.c:990)
==465==    by 0x40011EC: main (4-crash.c:15)
==465==  Block was alloc'd at
==465==    at 0x4850858: malloc (vg_replace_malloc.c:447)
==465==    by 0x40011A8: main (4-crash.c:7)
==465==
==465== Invalid read of size 4
==465==    at 0x40011FB: main (4-crash.c:18)
==465==  Address 0x4a91040 is 0 bytes inside a block of size 5,242,880 free'd
```

**Type:** two separate use-after-free errors on the same block: an invalid write at line
17 (`big[0] = 1337;`) and an invalid read at line 18 (the `printf` reading `big[0]` again
to print it). Both point back to the same `free()` at line 15 and the same `malloc()` at
line 7.

**What it means:** this is a classic use-after-free, code touching memory after its
owning `free()` call, but there's a detail that only shows up if you actually run it and
don't just read the log. Full root cause and the deterministic-crash explanation are in
`crash_report.md`, since that file is specifically about the segfault. Here I want to flag
something about Valgrind's own behavior on this exact program:

**The program does not crash under Valgrind.** The full log ends with:

```
==465== HEAP SUMMARY:
==465==     in use at exit: 0 bytes in 0 blocks
==465== All heap blocks were freed -- no leaks are possible
==465== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0)
```

and the captured stdout is:

```
big[0] before free: 42
big[0] after free: 1337
```

Exit code 0, both prints happened. But running the exact same binary directly (no
valgrind) gives a real segmentation fault, signal 11, confirmed with `echo $?` = 139
(`128 + 11`). Same program, same bug, two different observed outcomes depending on whether
Valgrind is attached.

**Why:** Valgrind doesn't run your program on real memory using the real glibc allocator.
It substitutes its own `malloc`/`free` (`vg_replace_malloc.c` in the stack traces above) and
tracks validity itself in shadow memory, it does not necessarily hand pages back to the
kernel the moment `free()` is called the way real glibc does for a block this size (see
`crash_report.md` for why the *real* glibc actually unmaps this one via `munmap`). Under
Valgrind the bytes are still there and still mapped, so the write and the read both
physically succeed at the hardware level, Valgrind just notices they shouldn't have been
allowed and reports them after the fact, then lets execution continue. Outside Valgrind,
there's no such safety net, so the real, unmapped-page segfault happens instead.

I asked an AI (before I'd actually run it under Valgrind myself) what it expected the
Valgrind output for this program to look like, and it said something like: "Valgrind will
show the invalid write and then the program will crash there, same as running it
normally, since dereferencing freed memory is undefined behavior regardless of the tool."
That's wrong for this specific program, for the reason above: the crash under normal
execution and the two non-fatal errors under Valgrind are genuinely different outcomes of
the same bug, not the same outcome observed two ways. I only caught this because I
compared the actual `analysis/raw/4-crash.stdout.txt` (both prints present, exit 0) against
the actual `echo $?` from running it natively (139). If I'd just trusted the "should behave
the same" assumption I would have written this section wrong.
