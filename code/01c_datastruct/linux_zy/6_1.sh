#! /bin/bash

read -p "请输入任意一个目录：" dir
cd $dir

sum=0
for i in `ls -a`; do
	if [ -d $i ]; then
		sum=$[$sum+1]
	fi
done
echo "$dir 下的目录总数是：$sum"

