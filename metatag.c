#include <lib.h>
#include <sys/cdefs.h>
#include <stdio.h>
#include <string.h>
#include <minix/safecopies.h>
#include <minix/syslib.h>
/*
metatag FILE TEXT, write metadata of FILE, overwriting any contents
*/
int main(int argc, char *argv[])
{
	message m;
	int ret;
	cp_grant_id_t grant_id;

	m.m1_i1 = 0;/*contains flag (isRead) for read or write*/
	m.m1_i3 = fileno(fopen(argv[1], "r+")); /*contains filename*/
	m.m1_i2 = 1024;
	m.m1_p1 = argv[2];

	ret = _syscall(VFS_PROC_NR, 69, &m);
	printf("ret: %d\n", ret);
 
	return 0;
}
