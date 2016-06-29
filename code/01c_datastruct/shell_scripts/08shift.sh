#!/bin/bash

if [ $# -le 0 ]; then
	echo "命令行参数不够，至少1个！"
	exit 1
fi

sum=0
for i in $@; do
	sum=$[$sum + $1]
	shift
done
echo "sum = $sum"
