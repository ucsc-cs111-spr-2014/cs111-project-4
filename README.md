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
    - 
======================================



\documentclass[10pt]{article}
\usepackage{geometry}
\usepackage{amsmath}
\usepackage{amssymb}
\usepackage{enumitem}
\usepackage{fancyhdr}
\usepackage{sectsty}
\usepackage{tikz}
\usetikzlibrary{trees}
\pagestyle{fancy}
\usepackage{titlesec}
\pagenumbering{gobble}
\titleformat{\section}[block]{\Large\bfseries\filcenter}{\thesection}{1em}{}


\lhead{Cameron Gravel\newline
		Anthony D'Ambrosio\newline
		Rachelle Tanase\newline
		Jordan Hyman
		}
\chead{CMPS111 Project 3}
\rhead{21 May 2014}
\begin{document}
\section*{}

\section*{System Call}
Our system calls {\tt metaread()} and {\tt metawrite()} will take three arguments: a file descriptor, a pointer to a buffer, and a {\tt size\_t}.  The call is handled by a handler in the VFS server.

\section*{VFS Handler}
We choose to use one handler for both {\tt metaread()} and {\tt metawrite()}.  The handler first will map the file descriptor to a vnode.  It will then check the validity of the arguments.  Next we call a new request function {\tt req\_metarw()} that sends a message to MFS

\section*{Request Handler}
The request handler packs the relevant information into a message and uses {\tt fs\_sendrec()} to send the message to the MFS.

\section*{Message}
Our message takes a similar form to the existing message for {\tt read()} and {\tt write()}.  It holds 5 fields: the request type, the innode number of the file, the endpoint number of the caller process, the current file position, the number of bytes to transfer, and the address of the buffer array.

\section*{MFS Handler}
Our MFS handler will extract the inode number from the message it was sent.  We decided to use the triple indirection zone 9 to store our metadata because it is very rarely actually used for files.  First it will compare the inode's zone 9 to the macro {\tt NO\_ZONE} to check that the inode's zone 9 is allocated. If not allocated, we use {\tt zone\_alloc()} to allocate it.  We use the stickybit field of inode to represent the presence of metadata: 0 means no metadata is available for the file, 1 means metadata is available.  If the stickybit is high, we then use the scaling factor to find the first block of zone 9 and then store the block number in the {\tt ctime} field of the inode.

\end{document}
