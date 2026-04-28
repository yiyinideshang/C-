#!/bin/bash

read $NUM1 $NUM2

if [ $NUM1 = $NUM2] ; then
    echo "两个字符串相等"
else
    echo "两个字符串不相等"
fi