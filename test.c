#include<lib.h>
#include <sys/cdefs.h>

int main (void)
{
	message m;
	_syscall(VFS_PROC_NR,69,&m);
	return 0;
}
