#!/bin/bash

sum=` expr 1 + 2 `
echo $sum

read NUM1 NUM2
SUM=` expr $NUM1 + $NUM2 `
echo $SUM

NUM=` expr 1 \* 5`
echo $NUM