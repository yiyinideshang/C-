#!/bin/bash

read filename
case $filename in
    *.cpp)
        echo "这是一个c++源代码文件"
    ;;
    *.h)
        echo "这是一个头文件"
    ;;
    *)
        echo "这是一个未知文件"
esac