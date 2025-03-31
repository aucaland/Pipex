Pipex
Pipex is a project designed to explore the inner workings of UNIX pipes by implementing them in C. This program replicates the following shell behavior:

sh
Copy
Edit
< file1 cmd1 | cmd2 > file2
Features
Execution of two chained commands through a pipe.

Proper error handling and memory allocation without leaks.

Compliance with UNIX conventions and execve compatibility.

Usage
Compilation:

sh
Copy
Edit
make
Execution:

sh
Copy
Edit
./pipex file1 "cmd1" "cmd2" file2
Example:

sh
Copy
Edit
./pipex infile "ls -l" "wc -l" outfile
is equivalent to:

sh
Copy
Edit
< infile ls -l | wc -l > outfile
Bonus
Handling multiple commands (cmd1 | cmd2 | ... | cmdn).

Support for "here_doc" mode (<< LIMITER).
