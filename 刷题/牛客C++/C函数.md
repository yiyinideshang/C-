# 内存或字符串操作函数`<cstring>`

**头文件**：`<string.h>`（C）或 `<cstring>`（C++）

## 拷贝函数

| 函数          | 主要功能           | 是否处理重叠             | 数据形式             | 停止条件         |
| :------------ | :----------------- | :----------------------- | :------------------- | :--------------- |
| **memcpy()**  | 内存复制（不重叠） | 不保证（可能未定义行为） | 二进制数据           | 按指定字节数复制 |
| **memmove()** | 内存复制（可重叠） | 是（安全复制）           | 二进制数据           | 按指定字节数复制 |
| **memset()**  | 内存设置（填充值） | 不适用（单一块操作）     | 二进制数据           | 按指定字节数填充 |
| **strcpy()**  | 字符串复制         | 不保证（可能未定义行为） | 字符串（以`\0`结尾） | 遇到`\0`停止     |

- **memcpy()** 和 **memmove()** 都用于内存块的复制，但 **memmove()** 能够正确处理源和目标内存区域重叠的情况，因此更通用、安全。
- **memset()** 用于将内存块填充为指定的值，而不是复制。
- **strcpy()** 专用于字符串复制，遇到空字符（`\0`）终止，且不指定长度，因此不适合任意二进制数据的复制。





以下哪个函数可以在源地址和目的地址的位置任意的情况下，在源地址和目的地址的空间大小任意的情况下实现二进制代码块的复制？

A	`memcpy()`

B	`memmove()`

C	`memset()`

D	`strcpy()`

- A	**错误**:**memcpy()** 不保证重叠内存的正确复制，行为未定义。
- B    **正确**：**memmove()** 能够处理源地址和目的地址重叠的情况，它会根据内存区域是否重叠选择合适的复制方向（从前往后或从后往前），确保数据正确复制。
- C    **错误**：**memset()** 用于填充内存，而非复制。
- D    **错误**:**strcpy()** 用于字符串复制，遇到空字符停止，且不处理二进制数据块的重叠问题。

# `printf`函数

**打印一下两项 内容分别需要使用什么转换说明** 

- 一个字段宽度为22、左对齐的字符串
- 字符宽度为11的unsigned long类型的整数a

1. ```c
   %-22s
   ```

   - `%s` 用于打印字符串
   - `22` 指定最小字段宽度
   - `-` 表示左对齐

2. ```c
   %11lu
   ```

   - `%lu` 用于打印`unsigned long`类型
   - `11` 指定最小字段宽度（默认为右对齐）



**以下程序的输出结果是什么：**

```c
#include <stdio.h>
    main(){
    int a = 2, c = 5;
    printf("a=%%d,b=%%d\n", a, c);
}
```

- 在C语言的`printf`函数中，`%` 是格式说明符的开始，如：遇到 `%d`、`%f`、`%s` 等，它会将其解释为“需要在这里插入一个对应类型的变量值”
- 通过`%%` 这个转义序列来表示“输出一个实际的 `%` 字符。就像以下一样：
  - 字符串中想输出双引号：`\"`
  - 字符串中想输出反斜杠：`\\`
  - 在 `printf` 中想输出百分号：`%%`

# C 库函数 - `memset()`

## 描述

C 库函数 **void \*memset(void \*str, int c, size_t n)** 用于将一段内存区域设置为指定的值。

memset() 函数**将指定的值 c 复制到 str 所指向的内存区域的前 n 个字节中**，这**可以用于将内存块清零或设置为特定值。**

在一些情况下，需要快速初始化大块内存为零或者特定值，memset() 可以提供高效的实现。

**在清空内存区域或者为内存区域赋值时，memset() 是一个常用的工具函数。**

## 声明

`void *memset(void *ptr, int value, size_t num);`

下面是 memset() 函数的声明。

```c++
void *memset(void *str, int c, size_t n)
```

## 参数

- **str** -- 指向要填充的内存区域的指针。
- **c** -- 要设置的值，通常是一个无符号字符。
- **n** -- 要被设置为该值的字节数。

## 返回值

该值返回一个指向存储区 str 的指针。

## 注意事项

- `memset()` 并不对指针 `ptr` 指向的内存区域做边界检查，因此使用时需要确保 `ptr` 指向的内存区域足够大，避免发生越界访问。
- `memset()` 的第二个参数 `value` 通常是一个 `int` 类型的值，但实际上只使用了该值的低8位。这意味着在范围 `0` 到 `255` 之外的其他值可能会产生未定义的行为。
- `num` 参数表示要设置的字节数，通常是通过 `sizeof()` 或其他手段计算得到的。

## 实例

下面的实例演示了 memset() 函数的用法。

## 实例

```c++
#include <stdio.h>
#include <string.h>
 
int main ()
{
   char str[50];
 
   strcpy(str,"This is string.h library function");
   puts(str);
 
   memset(str,'$',7);
   puts(str);
   
   return(0);
}
```

让我们编译并运行上面的程序，这将产生以下结果：

```
This is string.h library function
$$$$$$$ string.h library function
```

## 实例

```c++
#include <stdio.h>
#include <string.h> // 引入 string.h 头文件以使用 memset

int main() {
    char buffer[10];

    // 将 buffer 数组的前5个字节设置为字符 'A'，并添加字符串终止符
    memset(buffer, 'A', 5);
    buffer[5] = '\0'; // 确保添加字符串终止符
    printf("Buffer after memset: %s\n", buffer);

    // 将 buffer 数组清零，使用 '\0' 替代 0
    memset(buffer, '\0', sizeof(buffer)); // 使用'\0'确保一致性及可读性
    printf("Buffer after memset: %s\n", buffer);

    return 0;
}
```

让我们编译并运行上面的程序，这将产生以下结果：

```c++
Buffer after memset: AAAAA
Buffer after memset: 
```



下面代码不能正确输出hello的选项为（）

```c++
#include<stdio.h>
struct str_t{
   long long len;
   char data[32];
};
struct data1_t{
   long long len;
   int data[2];
};
struct data2_t{
   long long len;
   char *data[1];
};
struct data3_t{
   long long len;
   void *data[];
};
int main(void)
{
   struct str_t str;
   memset((void*)&str,0,sizeof(struct str_t));
   str.len=sizeof(struct str_t)-sizeof(int);
   snprintf(str.data,str.len,"hello");//VS下为_snprintf
   ____________________________________;
   ____________________________________;
   return 0;
}
```

A	`struct data3_t *pData = (struct data3_t*) &str; printf("data:%s%s\n", str.data, (char*) (&(pData->data[0])));`

B	`struct data2_t *pData = (struct data2_t*) &str; printf("data:%s%s\n", str.data, (char*) (pData->data[0]));`

C	`struct data1_t *pData = (struct data1_t*) &str; printf("data:%s%s\n", str.data, (char*) (pData->data));`

D	`struct str_t *pData = (struct str_t*) &str; printf("data:%s%s\n", str.data, (char*) (pData->data));`

- **选项A**：`struct data3_t *pData = (struct data3_t*) &str; printf("data:%s%s\n", str.data, (char*) (&(pData->data[0])));`
  - `data3_t` 结构体包含柔性数组 `void *data[]`，其元素 `data[0]` 位于 `len` 之后的内存位置，即 `str.data` 的起始地址。`&(pData->data[0])` 获取的是 `data[0]` 的地址，也就是 `str.data` 的起始地址，强制转换为 `char*` 后可以作为字符串输出 `"hello"`。
- **选项B**：`struct data2_t *pData = (struct data2_t*) &str; printf("data:%s%s\n", str.data, (char*) (pData->data[0]));`
  - `data2_t` 结构体包含指针数组 `char *data[1]`，`pData->data[0]` 的值是从 `str.data` 的前8个字节解释出的一个指针地址，该地址通常无效（非指向 `"hello"` 字符串的合法地址），因此尝试作为字符串输出会导致未定义行为（如段错误或乱码），无法正确输出 `"hello"`。
- **选项C**：`struct data1_t *pData = (struct data1_t*) &str; printf("data:%s%s\n", str.data, (char*) (pData->data));`
  - `data1_t` 结构体包含 `int data[2]`，`pData->data` 在表达式中退化为指向 `data[0]` 的 `int*` 指针，其地址即为 `str.data` 的起始地址。强制转换为 `char*` 后可以作为字符串输出 `"hello"`。
- **选项D**：`struct str_t *pData = (struct str_t*) &str; printf("data:%s%s\n", str.data, (char*) (pData->data));`
  - `pData->data` 直接就是 `str.data`，强制转换为 `char*` 后输出自身存储的 `"hello"`，显然正确。

综上，只有选项B不能正确输出 `"hello"`。
