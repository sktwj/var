#!/bin/bash

A=10
B=20

if [ $A -eq $B ]; then
#if test $A -eq $B; then
	echo "$A == $B"
elif [ $A -gt 0 ] 
then
	echo "$A > 0"
else
	echo "$A != $B"
fi

A="hello"
if [ -n $A ]; then
	echo "$A is not empty"
fi

read -p "请输入任意一个文件名：" filename
if [ -e $filename ]; then
	echo "$filename exist!"
	if [ -x $filename ]; then
		./$filename
	else 
		echo "$filename isn't executable file"
		exit 0
		echo "hello world!"
	fi
fi
