#!/bin/bash

read int1 int2
`test $int1 -eq $int2`
echo $?

`test $int1 -ne $int2`
echo $?