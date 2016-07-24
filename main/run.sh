#!/bin/bash

set -e

for i in 10 100 1000 10000 100000 1000000
do
    for j in `seq 1 16`
    do
        echo "$j $i" | ./host.elf >> out.dat
    done   
done