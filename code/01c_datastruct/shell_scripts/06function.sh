#! /bin/bash

abc=20

#函数的定义
function foo() {
	echo "foo, abc = $abc"

	#local用于说明该变量是局部变量
	local abc=10
#	exit 0
	return 0 #return 用在函数中可以使得函数执行结束返回，但不会退出程序
}

#函数的调用
foo
echo "abc = $abc"

#函数的定义中，函数名称后面的()可以省去
function test {
	echo "test"
}

test
