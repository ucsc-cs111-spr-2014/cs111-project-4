#include <lib.h>
#include <sys/cdefs.h>
#include <stdio.h>
#include <string.h>

/*
metatag FILE TEXT, write metadata of FILE, overwriting any contents
*/
int main(int argc, char *argv[])
{
	message m;
	int ret;
	
	m.m1_i1 = 0;/*contains flag (isRead) for read or write*/
	m.m1_i3 = fileno(fopen(argv[1], "r+")); /*contains filename*/

	printf("metadata: %s\n", m.m1_buf);
	m.m1_buf = "MONKEYBALLS"; /*contains text, ie metadata to write*/
	printf("metadata: %s\n", m.m1_buf);

	m.m1_i2 = sizeof(argv[2]);

	ret = _syscall(VFS_PROC_NR, 69, &m);
	printf("ret: %d\n", ret);
 
	return 0;
}