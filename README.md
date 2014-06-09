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
    + http://users.sosdg.org/~qiyong/mxr/source/servers/vfs/param.h#L9
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
Latex url: https://www.writelatex.com/1167602zbrsnd#/2789082/
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

\titleformat{\subsection}[block]{\normalsize\bfseries\filcenter}{\thesection}{1em}{}

\title{Program 4 Design Document Take Two}

\lhead{Cameron Gravel\newline Anthony D'Ambrosio\newline Rachelle Tanase\newline Jordan Hyman }
\chead{CMPS111 Project 4} 
\rhead{8 June 2014} 

\begin{document} 

\section*{}

\section*{Design}

\subsection*{Posix Library Functions}

In Posix, we created two functions {\tt metacat()} and {\tt metatag()} that sys call do_metacat and do_metatag.
\begin {itemize}
\item {\tt metacat()} reads the metadata stored in the specified file's inode. 
\item {\tt metatag()} writes the specified message to the specified file's inode. 
\end{itemize}

\subsection*{VFS Handler}

We created two VFS sys call handlers to handle {\tt metacat()} and {\tt metatag()}, called {\tt do\_metaread()} and {\tt do\_metawrite()}, respectively.

\begin{itemize}
\item{Request} The request function, {\tt req\_metarw()}, opens a grant to communicate with MFS. It then packs the relevant information into a message and sends the message to the MFS.
\item{Message} Our message takes a similar form to the existing message for {\tt read()} and {\tt write()}. It holds 4 fields: the request type, the inode number of the file, the grand ID, and the number of bytes to transfer.
\end{itemize}

\subsection*{MFS Handler}
We decided to use the triple indirection zone 9 to store our metadata because it rarely used for most files. Our MFS handler allocates a block in zone 9 in the inode if it is not already allocated, then reads or writes to the block as requested.


\section*{Implementation}

\subsection*{Posix Library Functions and System Calls}
We changed two entries of the  VFS call table:  {\tt do\_metaread()} in entry 69 and {\tt do\_meatwrite()} in entry 70.  These tell the system (ie sys calls) which functions will handle the system calls sent from {\tt metacat()} and {\tt metatag()}.  The {\tt metacat()} and {\tt metatag()} library functions are in {\tt \_metarw.c} in {\tt lib/libc/posix}.  They both take a file descriptor, number of bytes, and buffer pointer as arguments.  They pack the arguments into a message, and then call {\tt \_syscall()} with their respective call table entries.

\subsection*{VFS Handler}
In {\tt servers/vfs/read.c}, {\tt do\_metaread()} and {\tt do\_metawrite()} are our system call handlers.  They each call {\tt meta\_read\_write()} and pass in an argument that represents their intention to read or write.  {\tt meta\_read\_write()} checks validity of the arguments for our request function, {\tt req\_metarw()}, and then calls that request.

\subsection*{Request Handler}
In {\tt servers/vfs/request.c}, {\tt req\_metarw()} first calls {\tt cpf\_grant\_magic()} to set up a grant to transfer data between the user space and MFS.  It then populates a message with the inode number of the file, the grand ID, and the number of bytes.  Finally it uses {\tt fs\_sendrec()} to send the request to the MFS.

\subsection*{MFS Handler}
We first needed to add two calls to the MFS call table.  We added {\tt fs\_metarw()} to entries 33 and 34.  We also had to add a $34^{th}$ and $35^{th}$ entry to each other filesystem server (ext2, hgfs, pfs).  The new entries in the extra filesystems are left as {\tt no\_sys}.  In {\tt fs\_metarw()} we first find the inode of the desired file.  Then it determines the block size of the inode and store the values sent in with the request message.  Finally, it calls {\tt meta\_rw\_chunk()}, which handles the actual reading and writing.  {\tt meta\_rw\_chunk()} first allocates zone 9 of the inode if it has not been allocated yet, otherwise it just accesses i_zone[9] scaled by s_log_zone_size (found in the super block).  Next, it uses {\tt sys\_safecopyfrom()} to write from the grant to the inode block, or {\tt sys\_safecopyto()} to read from the inode block to the grant.  If we wrote metadata, we first zero the block (zero_block) so that any old data will not be re-used, and then we mark the block dirty before calling {\tt put\_block()} and returning.

\subsection*{Copy}
In {\tt /usr/src/commands/cp}, {\tt cp.c} holds the minix implementation for copying a file.  In {\tt copy()}, after the file contents have been copied, we call {\tt metacat()} on the original file to get the metadata associated with it.  We then {\tt metatag()} that data onto the new copy.

\section*{Testing}

We created a test script, {\tt mk.tests}, to test all requirements.  Running {\tt ./mk.tests} will cause all tests to run.  First we  add metadata to a file and {\tt metacat()} it to the screen. Then we copy that file to a new file and affirm that the metadata remains the same in both files.  Next we change the metadata of the original file and affirm that the new file's metadata remains unchanged.  Next we change the content of a file and affirm the metadata associated with it does not change.  Then we check that changing the metadata does not change the file contents.  Finally we create 1000 files with metadata and remove them, and affirm that we have no memory leaks.

\subsection*{Results}
All tests run as expected except removing the metadata, which shows a 4kB memory leak per file removed.

\end{document}



Secret info: SecurePassword1
