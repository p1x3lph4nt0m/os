#!/bin/bash
if [ "$#" -ne 3 ]; then 
	echo "Usage: $0 arg1 arg2 arg3 "
	exit 1
fi
echo "Arguments: $@" >> tests.txt
ARGS="$1 $2 $3"
for case in case1 case2; do
	start=$(date +%s.%N)
	./$case $ARGS
	end=$(date +%s.%N)
	duration=$(echo "$end-$start"|bc)
	echo "$case execution time: $duration seconds" >> tests.txt
done
