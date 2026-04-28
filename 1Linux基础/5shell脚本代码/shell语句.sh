#!/bin/bash

#输入语句
#read -a NUM1
#echo ${NUM1[*]}

#输入重定向语句
read student < 1.txt
echo $student

#输出重定向语句
echo "abc123" >> 1.txt
echo "def456" >> 1.txt
