# file_io

Small project about file I/O on Linux using the raw syscalls (open, close,
read, write) instead of the stdio functions like fopen/fread/fprintf.

## Files

- `main.h` - header with all the function prototypes and the include guard
- `0-read_textfile.c` - reads a text file and prints it to stdout
- `1-create_file.c` - creates a file and writes some text to it
- `2-append_text_to_file.c` - appends text to the end of an existing file
- `3-cp.c` - a small `cp` clone, copies one file to another 1024 bytes at a
  time

## Notes

- Only `malloc`, `free` and `exit` are used from the standard library
  (plus `dprintf` in `3-cp.c`, which is explicitly allowed for that task).
  No `printf`, `strlen`, etc.
- Compiled with `-Wall -Wextra -Werror -pedantic -std=gnu89`.
- `3-cp.c` exits with 97/98/99/100 depending on what went wrong (bad usage,
  can't read the source file, can't write the destination file, can't close
  a fd).
