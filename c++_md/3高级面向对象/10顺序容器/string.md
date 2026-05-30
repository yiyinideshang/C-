# `std::string`

## `std::sort`

对`string`进行排序

```c++
#include <iostream>
#include <algorithm>
#include <string>

int main() {
    std::string s = "hello";
    std::sort(s.begin(), s.end());
    std::cout << s << std::endl;  // 输出: ehllo
    return 0;
}
```

## `to_string(a)`

- `to_string(a)` 是 C++11 引入的一个标准库函数，用于将数值类型转换为对应的字符串。

```c++
std::to_string(42);        // int → "42"
std::to_string(42L);       // long → "42"
std::to_string(42LL);      // long long → "42"
std::to_string(3.14f);     // float → "3.140000"
std::to_string(3.14);      // double → "3.140000"
std::to_string(3.14L);     // long double → "3.140000"
```

## `substr()`

`std::string`类提供了`substr()`函数来获取子字符串。以下是详细说明：

```c++
// 函数原型：
string substr(size_t pos = 0, size_t count = npos) const;

// 参数说明：
// pos    - 要包含的首个字符的位置（默认0）
// count  - 子串的长度（默认到字符串末尾）
// 返回值 - 包含子串的string对象

// 特殊值：
// npos  - size_t类型的特殊值，表示直到字符串结尾
```

## `bzero()`

**将内存块（字符串）的前n个字节清零**，在`string.h`头文件中

- 函数原型

```c++
void bzero(void *s, int n);
```

```c++
#include<string.h>
int main(int argc,char** argv){
    struct sockaddr_in serveraddr;
    bzero(&serveraddr,sizeof(serveraddr));
    return 0;
}
```

与 `memset((void*)s, 0,size_tn)`是等价的，都是用来将内存块的前 n 个字节清零。

## 将存储数字的字符串转换为`int`类型

### `std::stoi()`

将存储数字的字符串转换为`int`类型有多种方法

```c++
#include <iostream>
#include <string>
#include <stdexcept>  // 用于异常处理

int main() {
    std::string str1 = "12345";
    std::string str2 = "-6789";
    std::string str3 = "  42  ";  // 有空格
    
    try {
        // 基本用法
        int num1 = std::stoi(str1);
        std::cout << "str1 转换为: " << num1 << std::endl;
        
        // 可以处理负数
        int num2 = std::stoi(str2);
        std::cout << "str2 转换为: " << num2 << std::endl;
        
        // 自动跳过前导空格
        int num3 = std::stoi(str3);
        std::cout << "str3 转换为: " << num3 << std::endl;
        
        // 处理大数或溢出
        std::string bigStr = "999999999999999";
        try {
            int bigNum = std::stoi(bigStr);
            std::cout << "大数转换: " << bigNum << std::endl;
        } catch (const std::out_of_range& e) {
            std::cout << "数值超出范围: " << e.what() << std::endl;
        }
        
    } catch (const std::invalid_argument& e) {
        std::cout << "无效参数: " << e.what() << std::endl;
    } catch (const std::out_of_range& e) {
        std::cout << "超出范围: " << e.what() << std::endl;
    }
    
    return 0;
}
```

| 方法                | 优点                         | 缺点                       | 适用场景               |
| :------------------ | :--------------------------- | :------------------------- | :--------------------- |
| `std::stoi()`       | 标准库函数，安全，有异常处理 | C++11以上，性能一般        | 大多数情况推荐使用     |
| `std::stringstream` | 类型安全，可重用             | 性能较差，创建流对象开销大 | 需要复杂格式化时       |
| `atoi()`            | C语言兼容，简单快速          | 无错误处理，失败返回0      | 简单场景，确认输入有效 |
| `sscanf()`          | 灵活，可解析复杂格式         | C语言风格，类型不安全      | 需要解析复杂格式时     |
| 手动转换            | 完全控制，可自定义           | 代码复杂，易出错           | 教学或特殊需求         |
| `std::from_chars()` | 性能最好，不分配内存         | C++17以上，接口较新        | 高性能需求，C++17环境  |
| 模板函数            | 类型安全，可复用             | 需要模板知识               | 通用代码库             |

## 将`int`类型转换为字符串

### `std::to_string()`

```c++
#include <iostream>
#include <string>

int main() {
    int num = 12345;
    int negative = -6789;
    
    // 转换为字符串
    std::string str1 = std::to_string(num);
    std::string str2 = std::to_string(negative);
    
    std::cout << "正数: " << str1 << std::endl;
    std::cout << "负数: " << str2 << std::endl;
    
    // 其他整数类型也可以使用
    long long bigNum = 123456789012345;
    std::string str3 = std::to_string(bigNum);
    std::cout << "长整数: " << str3 << std::endl;
    
    // 浮点数转换
    double pi = 3.14159;
    std::string str4 = std::to_string(pi);
    std::cout << "浮点数: " << str4 << std::endl;
    
    return 0;
}
```

1. **推荐使用 `std::to_string()`**：在大多数情况下是最佳选择，简单且易读。
2. **如果需要自定义格式**：使用 `std::stringstream` 或 `sprintf()`。
3. **如果追求高性能**：考虑使用 C++17 的 `std::to_chars()`。
4. **注意异常处理**：虽然 `std::to_string()` 通常不会失败，但在资源极度受限的情况下可能抛出异常。
5. **考虑代码可读性和维护性**：在大多数应用场景中，性能差异不大，应优先考虑代码清晰度。

根据具体需求选择最适合的方法。在C++11及以上的项目中，`std::to_string()` 通常是首选。

## 字符串反转

### 1. `std::reverse()` 就地反转

```c++
#include <iostream>
#include <string>
#include <algorithm>  // 需要包含这个头文件

int main() {
    std::string str = "Hello, World!";
    
    std::cout << "原始字符串: " << str << std::endl;
    
    // 使用std::reverse原地反转字符串
    std::reverse(str.begin(), str.end());
    
    std::cout << "反转后: " << str << std::endl;
    
    // 也可以反转字符串的一部分
    std::string str2 = "ABCDEFGHIJKLMNOP";
    std::cout << "\n原始: " << str2 << std::endl;
    
    // 反转前5个字符
    std::reverse(str2.begin(), str2.begin() + 5);
    std::cout << "反转前5个字符: " << str2 << std::endl;
    
    return 0;
}
```

### 使用反向迭代器构造新字符串

```c++
#include <iostream>
#include <string>

int main() {
    std::string str = "Hello, World!";
    
    // 使用反向迭代器构造新字符串（原字符串不变）
    std::string reversed(str.rbegin(), str.rend());
    
    std::cout << "原始字符串: " << str << std::endl;
    std::cout << "反转后字符串: " << reversed << std::endl;
    
    // 也可以只反转部分
    std::string partial(str.rbegin() + 2, str.rend() - 3);
    std::cout << "部分反转: " << partial << std::endl;
    
    return 0;
}
```

### 3. 手动交换字符（双指针法）

### 使用 `std::transform()` 算法

### 总结

| 方法             | 优点               | 缺点                | 适用场景           |
| :--------------- | :----------------- | :------------------ | :----------------- |
| `std::reverse()` | 最简单，标准库实现 | 需要包含<algorithm> | 大多数情况推荐使用 |
| 反向迭代器       | 简洁，不修改原串   | 创建新字符串开销    | 需要保留原字符串时 |
| 手动交换         | 完全控制，高效     | 代码相对复杂        | 教学或特殊需求     |
| 递归             | 理论上有趣         | 栈溢出风险，效率低  | 学习递归概念       |
| 栈               | 直观展示后进先出   | 额外内存开销        | 演示数据结构应用   |
| 异或交换         | 不使用临时变量     | 可读性差，不常用    | 内存受限环境       |
| UTF-8处理        | 正确处理Unicode    | 复杂，性能开销      | 国际化应用         |

# 字符串大小写转换

## 1. C语言风格字符串

### 转换为大写 - `toupper()`

```c++
#include <cctype>  // 需要包含这个头文件
#include <iostream>
#include <cstring>

int main() {
    char str[] = "Hello World";
    
    // 方法1：逐个字符转换
    for(int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
    std::cout << str << std::endl;  // 输出: HELLO WORLD
    
    return 0;
}
```

### 转换为小写 - `tolower()`

```c++
#include <cctype>
#include <iostream>
#include <cstring>

int main() {
    char str[] = "Hello World";
    
    for(int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
    std::cout << str << std::endl;  // 输出: hello world
    
    return 0;
}
```

## 2. 使用 `std::string` 和 `STL`算法（推荐）

### `std::transform` 参数详解

- ### 函数原型

```c++
template< class InputIt, class OutputIt, class UnaryOperation >
OutputIt transform( InputIt first1, InputIt last1, 
                    OutputIt d_first, UnaryOperation unary_op );
```

```c++
std::transform(result.begin(), // 输入范围的开始
               result.end(),   // 输入范围的结束
               result.begin(), // 输出位置的开始
               ::toupper);     // 要应用的函数
```



```c++
#include <iostream>
#include <string>
#include <algorithm>  // for std::transform
#include <cctype>     // for std::toupper, std::tolower

// 转换为大写
std::string toUpperCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

// 转换为小写
std::string toLowerCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

int main() {
    std::string str = "Hello World 123!";
    
    std::string upper = toUpperCase(str);
    std::string lower = toLowerCase(str);
    
    std::cout << "Original: " << str << std::endl;
    std::cout << "Upper: " << upper << std::endl;  // HELLO WORLD 123!
    std::cout << "Lower: " << lower << std::endl;  // hello world 123!
    
    return 0;
}
```

## 3.  使用 `std::locale` 处理本地化字符

```c++
#include <iostream>
#include <string>
#include <algorithm>
#include <locale>

int main() {
    std::string str = "Café Hello";
    
    // 创建本地化对象
    std::locale loc;
    
    // 转换为大写（支持本地化字符）
    std::string upper = str;
    std::transform(upper.begin(), upper.end(), upper.begin(),
                   [&loc](char c) { return std::toupper(c, loc); });
    
    // 转换为小写（支持本地化字符）
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [&loc](char c) { return std::tolower(c, loc); });
    
    std::cout << "Upper: " << upper << std::endl;
    std::cout << "Lower: " << lower << std::endl;
    
    return 0;
}
```

## 4. 使用C++20的 `std::ranges`

```c++
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

int main() {
    std::string str = "Hello World";
    
    // C++20方式转换为大写
    std::ranges::transform(str, str.begin(), ::toupper);
    std::cout << str << std::endl;
    
    return 0;
}
```

## 注意事项：

1. **字符编码**：`toupper()` 和 `tolower()` 函数只能正确处理ASCII字符。对于UTF-8等非ASCII字符，需要使用其他方法。
2. **性能考虑**：`std::transform` 通常比手写循环性能更好，因为编译器可以更好地优化。

3. **本地化**：如果需要处理非英语字符，使用带有 `std::locale` 的版本。

4. **头文件**：

- `<cctype>`：包含 `toupper`, `tolower`
- `<algorithm>`：包含 `std::transform`
- `<locale>`：包含本地化相关的函数

5. **返回值**：注意 `toupper()` 和 `tolower()` 返回的是 `int`，需要转换为 `char`。

对于C风格字符串和 `std::string`，推荐使用 `std::transform` 的方式，因为它更安全、更易读，且支持 `std::string` 的各种特性。



