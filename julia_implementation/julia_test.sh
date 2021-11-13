#!/bin/bash

for i in {1..15}
do
	julia --threads $i triangle_counting.jl
done
