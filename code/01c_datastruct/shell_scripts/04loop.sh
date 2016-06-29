#! /bin/bash

i=1
sum=0
while [ $i -le 100 ]; do
	sum=$[$sum + $i]
	i=$(($i + 1))
#i=`expr $i + 1`
#let i=$i+1
done
echo "while, 1+...+100 = $sum"

i=0
sum=0
until [ $i -gt 100 ]; do
	sum=$(($sum + $i))
	i=$[$i + 1]
done
echo "until, 1+...+100 = $sum"

for((i=0,sum=0; $i<=100; i++)); do
	sum=$(($sum + $i))
done
echo "for, 1+...+100 = $sum"

echo "for in..."
for var in `seq 10 -0.5 5`
do
	echo $var
done
