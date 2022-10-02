#!/bin/bash

size=(500 512 1000 1024 2000 2048)

for n in ${size[@]}
do
    export N=$n
    echo
    echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    echo "matrix size N=$N"
    echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    echo
    ./matmul/matmul_exec_time
done

