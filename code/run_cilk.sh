#!/bin/bash

for i in {1..15}
do
	export CILK_NWORKERS=$i
	./Cilk/triangle_counting_cilk 1 1 $i 1
done
