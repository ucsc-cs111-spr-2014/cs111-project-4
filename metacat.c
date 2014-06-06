#include <lib.h>
#include <sys/cdefs.h>
#include <stdio.h>

/*
metacat FILE, prints metadata of FILE to stdout
*/
int main(int argc, char *argv[])
{
	int ret;
	message m;

	m.m1_i1 = 1; /*contains flag (isRead) for read or write*/
	m.m1_i3 = fileno(fopen(argv[1], "r")); /*contains filename*/

	ret = _syscall(VFS_PROC_NR, 69, &m);
	printf("ret: %d\n", ret);

	return 0;
}