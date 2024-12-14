#!/bin/bash
name="manager"
name_worker="worker"
prog="$name.c"
prog_worker="$name_worker.c"
out="$name.result"
clear && mpicc $prog_worker -o $name_worker.o && mpicc $prog -o $name.o &&
echo "Workers\tComm Time\tProcess Time" > $out &&
mpirun -np 1 ./$name.o 2 256 >> $out
