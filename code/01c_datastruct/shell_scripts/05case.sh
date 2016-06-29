#!/bin/bash

read -p "请输入两个整数：" a b

# select 可以形成表单的选项
select opr in "+" "-" "*" "/"
do
	break
done

case $opr in
"+")
	echo "$a + $b = `expr $a + $b`" 
	;;
"-")
	echo "$a - $b = `expr $a - $b`" 
	;;
"*")
	# expr中，*必须使用转义符号\
	echo "$a * $b = `expr $a \* $b`" ;;
"/") echo "$a / $b = `expr $a / $b`" ;;
*) echo "not known opr"
esac
	
