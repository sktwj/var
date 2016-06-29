#!/bin/bash

if [ ! -e ~/.trash ]; then
	mkdir ~/.trash
fi

if [ $# -eq 0 ]; then
	echo "用法: 脚本文件名 要删除的文件名1 [, 要删除的文件名2, [...]]"
	exit 0
else
	for i in $@; do
		read -p "你是否要删除文件$i [Y/N][y/n]? " reply
#if [ $reply = 'Y' -o  $reply = 'y' ]; then
		if [ $reply = Y ] || [ $reply = y ]; then
			mv $i ~/.trash
		fi
	done
fi
