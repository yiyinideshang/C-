#!/bin/bash

read -p "请输入表达式 (例如: 5 + 3): " num1 operator num2

case $operator in
    "+")
        result=$(echo "$num1 + $num2" | bc)
        echo "$num1 + $num2 = $result"
        ;;
    "-")
        result=$(echo "$num1 - $num2" | bc)
        echo "$num1 - $num2 = $result"
        ;;
    "*")
        result=$(echo "$num1 * $num2" | bc)
        echo "$num1 * $num2 = $result"
        ;;
    "/")
        # 检查除数是否为0
        if [ $num2 -eq 0 ]; then
            echo "错误: 除数不能为0!"
        else
            # 使用bc进行除法运算以支持小数
            result=$(echo "scale=2; $num1 / $num2" | bc)
            echo "$num1 / $num2 = $result"
        fi
        ;;
    *)
        echo "不支持的运算符: $operator"
        echo "请使用 +, -, *, 或 /"
        ;;
esac