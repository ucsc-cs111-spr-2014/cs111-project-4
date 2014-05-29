cs111-project-4
===============
+ Extend filesystem to hold 1 extra block of metadata (1024 B)
+ Design an interface (syscall) for user space processes to read and write that metadata
+ Make MINIX call our interface on its own
+ Make library functions for the syscalls
+ Test program
+ command line utility

===============
READ UP ON DIS SHIT
+ VFS
    + Full Document: www.minix3.org/doc/gerofi_thesis.pdf
    + README: http://users.sosdg.org/~qiyong/mxr/source/servers/vfs/README
    + Messaging: http://users.sosdg.org/~qiyong/mxr/source/include/minix/vfsif.h
+ Syscall
	+ www.cis.syr.edu/~wedu/seed/Labs/Documentation/Minix3/How_to_add_system_call.pdf
+ Inode
	+ cis.syr.edu/~wedu/seed/Labs/Documentation/Minix3/Inode.pdf
