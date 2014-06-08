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
We made a lot of changes from our first draft of our design document. Previously we were going to use a sticky bit and store the message in a buffer which we stored in a message in the VFS handler. We decided not to use the sticky bit or the buffer, and changed to grant IDs instead.


\section*{Design}

\subsection*{Posix System Call}

In Posix, we created two calls {\tt metacat()} and {\tt metatag()} that are handled in the VFS server.
\begin {itemize}
\item {\tt metacat()} reads the metadata stored in the specified file's inode. It is called using the following syntax: {\tt ./metacat FILENAME}.
\item {\tt metatag()} writes the specified message to the specified file's inode. It is called using the following syntax: {\tt ./metatag FILENAME "message"}.
\end{itemize}

\subsection*{VFS Handler}

We created two handlers to handle {\tt metacat()} and {\tt metatag()} called {\tt do\_metaread()} and {\tt do\_metawrite()}, respectively.

\begin{itemize}

\item{Request Handler}

The request handler, {\tt req\_metarw()}, packs the relevant information into a message and uses {\tt fs\_sendrec()} to send the message to the MFS.

\item{Message}

Our message takes a similar form to the existing message for {\tt read()} and {\tt write()}. It holds 5 fields: the request type, the inode number of the file, the grand ID, and the number of bytes to transfer.

\end{itemize}

\subsection*{MFS Handler}

We decided to use the triple indirection zone 9 to store our metadata because it rarely used for most files. Our MFS handler extracts the inode number from the message it was sent. We, then, allocate a block in zone 9 and copy the metadata from the grant to the block.


\section*{Implementation}

\indent We use the grant ID to communicate between the MFS and the VFS. In MFS we use the grant ID to use the {\tt sys\_safecopyto()} and {\tt sys\_safecopyfrom()} between the message buffer and the block in the inode.

\subsection*{Posix System Call}




\subsection*{VFS Handler}

\subsection*{Request Handler}

\subsection*{Message}

\subsection*{MFS Handler}

\subsection*{Copying Metadata}

We successfully copied the metadata when we copy the file. To do this, we modified the {\tt cp} function to call {\tt metacat()} to get the metadata from the original file and {\tt metatag()} that to the new file. 

\subsection*{Removing Metadata}

\subsection*{Testing}

We created a test script, {\tt mk.tests}, to test all requirements.

\end{document}



Secret info: SecurePassword1