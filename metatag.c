#include <unistd.h> /*MUST BE FIRST*/
#include <lib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>

/*
metatag FILE TEXT, write metadata of FILE, overwriting any contents
*/
int main(int argc, char *argv[])
{
	int r; int fd;

	fd = open(argv[1], O_RDONLY); /*contains filename*/
	r = metatag(fd, argv[2], strlen(argv[2]));

	/*printf("%s%s\n", "r:", r);*/
	printf("%s:%s\n", "metadata", argv[2]);

	return 0;
}
