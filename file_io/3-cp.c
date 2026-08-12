#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define BUF_SIZE 1024

/**
 * close_fd - closes a file descriptor, exits with 100 on failure
 * @fd: file descriptor to close
 *
 * Return: nothing
 */
void close_fd(int fd)
{
	if (close(fd) == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", fd);
		exit(100);
	}
}

/**
 * copy_content - copies the content of fd_from into fd_to, 1024 bytes
 * at a time
 * @fd_from: source file descriptor
 * @fd_to: destination file descriptor
 * @from: name of the source file, used for the error message
 * @to: name of the destination file, used for the error message
 *
 * Return: nothing
 */
void copy_content(int fd_from, int fd_to, char *from, char *to)
{
	char buf[BUF_SIZE];
	ssize_t r, w;

	r = read(fd_from, buf, BUF_SIZE);
	while (r > 0)
	{
		w = write(fd_to, buf, r);
		if (w == -1 || w != r)
		{
			dprintf(STDERR_FILENO, "Error: Can't write to %s\n", to);
			exit(99);
		}
		r = read(fd_from, buf, BUF_SIZE);
	}

	if (r == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't read from file %s\n", from);
		exit(98);
	}
}

/**
 * main - copies the content of a file to another file
 * @ac: argument count
 * @av: argument vector
 *
 * Return: 0 on success
 */
int main(int ac, char **av)
{
	int fd_from, fd_to;

	if (ac != 3)
	{
		dprintf(STDERR_FILENO, "Usage: cp file_from file_to\n");
		exit(97);
	}

	fd_from = open(av[1], O_RDONLY);
	if (fd_from == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't read from file %s\n", av[1]);
		exit(98);
	}

	fd_to = open(av[2], O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (fd_to == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't write to %s\n", av[2]);
		exit(99);
	}

	copy_content(fd_from, fd_to, av[1], av[2]);

	close_fd(fd_from);
	close_fd(fd_to);

	return (0);
}
