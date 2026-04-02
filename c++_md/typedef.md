## 什么是typedef？

`typedef`是C++中的一个关键字，用于为已有的数据类型创建新的名称（别名）。它并不创建新的数据类型，只是给现有类型起一个更容易理解或更简洁的名字。

## 基本语法

```cpp
typedef 原类型 新类型名;
```

## 常见用法示例

### 1. 为基本类型创建别名

```cpp
typedef int Score;          // 给int起别名Score
typedef double Distance;    // 给double起别名Distance

Score studentScore = 95;    // 实际还是int类型
Distance d = 10.5;          // 实际还是double类型
```

### 2. 为结构体创建别名（非常常用）

```cpp
// 传统方式
struct Student {
    char name[20];
    int age;
};
typedef struct Student Student;  // 给struct Student起别名Student

// 更简洁的方式（推荐）
typedef struct {
    char name[20];
    int age;
} Student;

Student s1;  // 现在可以直接用Student，不用写struct
```

### 3. 为指针类型创建别名

```cpp
typedef char* String;        // 给char*起别名String
typedef int* IntPtr;         // 给int*起别名IntPtr

String name = "Hello";       // 实际是char* name
IntPtr p = new int(10);      // 实际是int* p
```

### 4. 为数组类型创建别名

```cpp
typedef int IntArray[10];    // 给int[10]起别名IntArray

IntArray arr;                // 等价于int arr[10];
arr[0] = 1;
```

### 5. 为函数指针创建别名

```cpp
// 定义一个函数指针类型，指向返回int、接受两个int参数的函数
typedef int (*MathOperation)(int, int);	//类型名MathOperation

// 实际的函数
int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }

// 使用
MathOperation op = add;      // op指向add函数
cout << op(3, 4) << endl;   // 输出7

op = multiply;              // 改为指向multiply函数
cout << op(3, 4) << endl;   // 输出12
```

## 实际应用场景

### 1. 提高代码可读性

```cpp
typedef unsigned int UINT32;
typedef char* LPSTR;

UINT32 fileSize = 1024;     // 比unsigned int fileSize更清晰
LPSTR buffer = nullptr;     // 比char* buffer更明确
```

### 2. 平台无关的编程

```cpp
#ifdef _WIN32
    typedef int INT32;
#else
    typedef long INT32;
#endif

INT32 value;  // 在不同平台上都是32位整数
```

### 3. 简化复杂类型声明

```cpp
// 复杂的函数指针数组
typedef void (*Callback)(int);
typedef Callback CallbackArray[5];

CallbackArray handlers;  // 等价于void (*handlers[5])(int)
```

## C++11中的using替代语法

C++11引入了`using`关键字，可以替代`typedef`，语法更直观：

```cpp
// 等价的定义方式
typedef int Score;
using Score = int;

typedef std::vector<std::string> StringList;
using StringList = std::vector<std::string>;

// using在模板别名中更强大
template<typename T>
using StringMap = std::map<std::string, T>;

StringMap<int> ageMap;  // std::map<std::string, int>
```

## 注意事项

1. **typedef不创建新类型**：只是别名，类型检查时与原类型相同
2. **作用域规则**：遵循普通的作用域规则
3. **可读性**：适度使用可以提高可读性，过度使用可能适得其反

## 总结

`typedef`是C++中非常有用的特性，主要用途：
- 提高代码可读性和可维护性
- 简化复杂类型的声明
- 实现平台无关的编程
- 创建更有意义的类型名称

在现代C++中，虽然`using`语法更受欢迎，但理解`typedef`对于阅读和维护现有代码仍然很重要。

# typedef ："把变量声明语句中的变量名变成类型名"

```c++
int number;                    // 变量声明
typedef int Number;            // 类型定义（Number现在代表int类型）

int (*func)(int, int);         // 变量声明  
typedef int (*MathOperation)(int, int);  // 类型定义（MathOperation现在代表函数指针类型）
```

```c++
#include <iostream>
using namespace std;

// 正确的typedef用法
typedef int (*MathOperation)(int, int);

// 实际函数
int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }

int main() {
    // 使用类型声明变量
    MathOperation op1 = add;
    MathOperation op2 = multiply;
    
    cout << op1(3, 4) << endl;  // 输出7
    cout << op2(3, 4) << endl;  // 输出12
    
    return 0;
}
```



、