## C++ 中的原子变量（std::atomic）

原子变量（std::atomic)是C++中用于多线程编程的强大工具之一，它们提供了一种线程安全的方式来访问和修改共享数据，而无需使用显式的互斥锁

# 基本概念

原子变量是一种特殊的数据类型，用于执行原子操作。

原子操作是不可分割的操作，要么完全执行，要么完全不执行，不会出现执行到一半被其他线程中断的情况。可以确保在多线程环境中线程安全地执行。

C++中的`std::atomic`提供了对原子操作的支持。

`std::atomic`支持各种数据类型，如整数、布尔值、指针等。您可以创建`std::atomic`对象，并使用原子操作来读取和修改它们的值。

头文件

```c++
#include <atomic>
```

# 声明和初始化

```c++
std::atomic<int> counter(0); //初始化counter 值为0
std::atomic<bool> flag(false); //原子布尔值falg
std::atomic<int*> ptr(nullptr); //原子指针
```

# 基本操作

## 读取值 `load`

```c++
std::atomic<int> atomicInt(0);
std::atomic<bool> atomicBool(true);

int value = atomicInt.load();
bool flag = atomicBool.load();
```

## 修改值(写入值) `store`

```c++
atomicInt.store(42);
atomicBool.store(false);
```

## 交换值

```c++
int old = atomicInt.exchange(30); // 返回旧值42，设置新值30
```

# 原子操作

`std::atomic`提供了一系列的原子操作函数，如`exchange`、`compare_exchange_weak`、`compare_exchange_strong`、`fetch_add`、`fetch_sub`等，用于执行各种原子操作。

````c++
std::atomic<int> count(0);

// 原子加法
count.fetch_add(5);     // 相当于 count += 5，返回旧值
count.fetch_sub(3);     // 相当于 count -= 3，返回旧值

// 原子自增/自减
count++;                // 等价于 fetch_add(1)
++count;

// 原子位运算
std::atomic<int> flags(0);
flags.fetch_and(0x0F);  // 相当于 flags &= 0x0F
flags.fetch_or(0x80);   // 相当于 flags |= 0x80
flags.fetch_xor(0xFF);  // 相当于 flags ^= 0xFF
````

