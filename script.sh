#!/bin/bash
make
read ARG1
read ARG2
read ARG3
./pipex ./srcs/file1.txt "$ARG1" "$ARG2" "$ARG3" ./srcs/file2.txt && echo && echo "REAL:" && echo && < ./srcs/file1.txt $ARG1 | $ARG2 | $ARG3 > ./srcs/file2.txt