gcc -Wall -Werror -Wextra -pedantic -std=gnu89 0-main.c 0-malloc_checked.c -o test
./test
echo $?
rm 0-main.c
