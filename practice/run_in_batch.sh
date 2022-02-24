#!/bin/bash

for ((i=0; i<8; i++)); do
    time ./solution data/c_coarse.in.txt 10000 $i > out/c_${i}.out 2> ${i}.score &
done
wait
