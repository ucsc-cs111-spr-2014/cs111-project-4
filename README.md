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
	+ http://cise.ufl.edu/class/cop4600sp14/Minix-Syscall_Tutorialv2.pdf
+ Inode
	+ www.cis.syr.edu/~wedu/seed/Labs/Documentation/Minix3/Inode.pdf

==================================

+ create new message type in vfsif.h
    - basically same fields as read() and write() message (mess_6)
        - file pointer
        - memory pointer
        - count

+ VFS syscall handler
    - use file descriptor arg to find vnode
    - pass message (our new type) to MFS

+ MFS handler
    - stickybit for "is metadata present"
    - convert vnode to inode
    - allocate zone 9 if neccessary.
    - find/allocate block, store metadata
    - store block number
