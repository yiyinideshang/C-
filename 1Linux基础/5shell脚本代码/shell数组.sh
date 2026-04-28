#!/bin/bash

#创建数组并赋值
Array=(abc 1 2 3 4 5 6)
echo ${Array[0]}
echo ${Array[*]}

Array[100]=100
echo ${Array[*]}
echo ${Array[7]}
echo ${Array[100]}