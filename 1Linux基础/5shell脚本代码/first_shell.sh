#!/bin/bash
#指定这shell脚本使用 /bin/bash 这个解析器进行解析
# 一般是注释
echo "Hello Shell" # 通过echo这条指令来在终端上面打印一句话

# shell 变量的定义和使用
NUM=123
echo $NUM

# 反撇号执行指令
Ls=`ls`
echo $Ls
