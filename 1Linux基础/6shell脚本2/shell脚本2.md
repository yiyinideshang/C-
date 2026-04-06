# shell脚本2

# expr算术运算语句

`expr` ：用于进行算术运算的一个终端命令

- 语法： `expr 操作数1 运算符 操作数2`

==注意：整个表达式必须要用空格分隔==

```bash
expr 1 + 2 会直接将结果输出到终端
// ------------------------
` expr 1 + 2 ` # 计算1 + 2 的值
SUM=` expr 1 + 2 ` # 计算1 + 2 的值 并保存到SUM中
NUM =` expr 1 \* 2
```

**注意**：

- 乘法运算符 `* `在 `Shell` 里面属于一个**元字符**，需要进行一个**转义**之后才能正常的使用乘法功能。 
- `expr` 它是一条命令，如果需要获取计算的结果，需要用反撇号，再用变量去接收结果 
- `expr` 与表达式中间一定要有空格

```bash
#!/bin/bash

sum=` expr 1 + 2 `
echo $sum

read NUM1 NUM2
SUM=` expr $NUM1 + $NUM2 `
echo $SUM

NUM=` expr 1 \* 5`
echo $NUM

yishang@yishang-virtual-machine:~/文档/shell脚本$ ./expr语句.sh 
3
1 2
3
5
```

# test 测试语句

`test `：可以测试三类对象： 

- **字符串**
-  **整数** 
- **文件**

## test 测试字符串

- `=` ：测试字符串**是否相等**     **(符合要求返回0,否则返回1)**

  - 比较两个字符串内容是否完全一样 

    - 如果相同： test 的返回码为`0 `
    - 如果不同： test 的返回码为`1`

  - 示例

    ````bash
    test "abc" = "abc" # 测试结果放在 $? 中
    ````

- `!=` ：测试字符串**是否不相同**    **(符合要求返回0,否则返回1)**

  - 比较两个字符串内容是否不一样
    - 如果相同： test 的返回码为`1 `
    - 如果不同： test 的返回码为`0`

- `-z `：测试字符串**是否为空**

  - 测试字符串内容是否为空 

    - 如果为空： test 的返回码为`0 `
    - 如果不空： test 的返回码为`1`

  - 示例

    ```bash
    test -z "abc"
    ```

- `-n` ：测试字符串**是否为不为空**

  - 测试字符串内容是否不为空 
    - 如果为空： test 的返回码为`1`
    - 如果不空： test 的返回码为`0`

## test 测试整数

==`test `命令的结果是使用的测试逻辑来的，如果逻辑成立了结果就是`0`，逻辑不成立结果就是`1`。==

- `-eq`：测试整数是否相等

  - 示例

    ```bash
    test 123 -eq 123 # 测试123和123是否相等
    ```

- `-ne`：测试整数是否不相等

  - 示例

    ```bash
    test 123 -ne 123 # 测试123和123是否不相等
    ```

- `-gt`：测试整数是否大于

  - 示例：

    ```bash
    test 123 -gt 123 # 测试123是否大于123
    ```

- `lt`：测试整数是否小于

  - 示例：

    ```bash
    test 123 -lt 123 # 测试123是否小于123
    ```

- `-ge`：测试整数是否大于等于

  - 示例：

    ```bash
    test 123 -ge 123 # 测试123是否大于等于123
    ```

- `-le`：测试整数是否小于等于

  - 示例：

    ```bash
    test 123 -le 123 # 测试123是否小于等于123
    ```

## test 测试文件

### 测试文件的类型 

- `test -d 文件名` ：测试这个文件是不是**目录**
-  `test -f 文件名 `：测试这个文件是不是**普通文件**
-  `test -L 文件名` ：测试这个文件是不是**链接文件**
-  ..

### 测试文件的权限

- `test -r 文件名` ：测试这个文件是否有可读权限
- ` test -w 文件名` ：测试这个文件是否有可写权限 
- `test -x 文件名 `：测试这个文件是否有可执行权限

### 测试文件的内容 

- `test -s 文件名` ：测试这个文件的**长度是否为0**（文件是否存在） 
  - 为空（不存在）：0
  -  不为空（存在）：1 
- `test 文件名1 -nt 文件名2`
  - `-nt` ： `new time/than` 新于/更新 
  - 测试文件1的是不是相对于文件2而言要更新。（依据最后的修改时间）
- `test 文件名1 -ot 文件名2 `
  - `-ot `： `old time/than` 旧于/更旧
  -  测试文件1的是不是相对于文件2而言要更旧。（依据最后的修改时间）

==注意： `test` 有一个更简单的写法，用` [] `来替代` test 命令` 注意：用` [] `代替 `test `命令使用，需要注意` [] `两边需要有空格，不然会报错==

# 结构性语句



## 分支语句

### if 分支

```bash
if 命令 ;then
	语句块1
elif 命令 ;then
	语句块2
else
	语句块3
fi

```

执行逻辑： 

- 先执行 `命令` 
- 再判断命令逻辑结果
-  最后执行`then` 后面的代码

`if`和`test`结合的示例：

```bash
#!/bin/bash

read $NUM1 $NUM2

if [ $NUM1 = $NUM2] ; then
    echo "两个字符串相等"
else
    echo "两个字符串不相等"
fi
```

```bash
read filename #从终端接收一个文件的名字
#判断文件的类型
if [ -f $filename ] ;then
echo "$filename 是普通文件"
else
echo "$filename 不是普通文件"
fi
```

### case 多路分支

````bash
case 变量 in
	情况1)
		语句块1
		;;
	情况2)
		语句块2
		;;
	情况3)
		语句块3
		;;
...
	情况n)
		语句块n
		;;
esac
````

`case` 语句和 `C++ `的 `switch` 语句是类似的，但是 `case` 功能更加强大可以使用**模糊搜索**

- 是指可以使用通配符和正则表达式（支持有限，比如 `[] `）一般使用 `*?` ，其实就是去匹配一个大概。
  - `* `表示任意个数任意字符
  - `? `表示至少有一个任意字符

==注意：` case `中` ;; `等价于 `switch` 里面` break` ，但是` break` 可以省略,但是 `;; `不能省略==

示例:

````bash
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
````



## 循环语句

### for 循环

```bash
for 变量名 in 变量需要遍历的值
do
	循环体语句
done
```

```bash
for num in 1 2 3 4 5 6 7 8 9
do
	echo $num
done

for ((num=1;num<10;num++))
{
    echo $num
}
```

也可以写成C++的形式

```bash
for((表达式1;表达式2;表达式3))
{
	循环体语句
}
```

==注意：在` shell`  的循环里面是可以使用` break` 和 `continue` 的。==

​	

### while 循环

```bash
while 表达式
do
	循环体语句
done
```

示例：

```bash
#打印小于等于10
num=0
while [ num -le 10 ]
do
	echo $num
	num=` expr $num + 1 `
done
```

# Shell 函数

```bash
函数名()
{
	函数体语句
}
```

==注意： `Shell `函数是不需要形参列表也不需要返回值，参数传递使用位置变量实现。==

```bash
sum()
{
	s=` expr $1 + $2 `
	return $s
}
```

调用函数

```bash
函数名 参数1 参数2 参数3 ... 参数9
-------------------------------
函数名

sum 1 2 # 函数调用
```

==注意：` Shell `中的函数它没有所谓作用域的区别，只要本文件存在的变量都可以用。==

```
#!/bin/bash

sum()
{
	s=` expr $1 + $2 `
	return $s
}

sum 1 2
echo $?
echo $s


yishang@yishang-virtual-machine:~/文档/shell脚本$ chmod +x shell函数.sh 
yishang@yishang-virtual-machine:~/文档/shell脚本$ ./shell函数.sh 
3
3
```

# 作业

- 编写一个简单的 Shell 脚本计算器
  -  实现简单两个操作数的加减乘除运算

```bash
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
```

