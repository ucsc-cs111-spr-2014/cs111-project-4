#include <unistd.h> /*MUST BE FIRST*/
#include <lib.h>

PUBLIC int metacat(int fd, void *buffer, size_t nbytes) {
	message m;

	m.m1_i1 = fd;
	m.m1_i2 = nbytes;
	m.m1_p1 = buffer;

	return(_syscall(VFS_PROC_NR, 69, &m));
}

PUBLIC int metatag(int fd, _CONST void *buffer, size_t nbytes{
	message m;

	m.m1_i1 = fd;
	m.m1_i2 = nbytes;
	m.m1_p1 = buffer;

	return(_syscall(VFS_PROC_NR, 70, &m));
}

