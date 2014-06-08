#include <unistd.h> /*MUST BE FIRST*/
#include <lib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>

/*
metacat FILE, prints metadata of FILE to stdout
*/
int main(int argc, char *argv[])
{
	int r; int fd;
	char buffer[1024];

	fd = open(argv[1], O_RDONLY); /*contains filename*/
	r = metacat(fd, buffer, 1024);

	printf("%s:%s\n", "metadata:", buffer);

	return r;
}