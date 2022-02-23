#!/bin/bash

for ((i=0; i<8; i++)); do
    ./solution data/c_coarse.in.txt $i > out/c_${i}.out 2> ${i}.score &
done
wait
