# `STL`容器和类型

## 支持下标“[]”运算的容器和类型

- **`vector`**：支持随机访问，提供 `operator[]`。
- **`deque`**：支持随机访问，提供 `operator[]`。
- **`map`**：提供 `operator[]`，用于按键访问值（若键不存在则插入）。
- **`unordered_map`**：提供 `operator[]`，功能类似 `map`。
- **`string`**：提供 `operator[]`，用于访问字符。

**不支持的容器和类型**

-  **`list`**：不支持随机访问，无 `operator[]`。
-  **`set`**：不提供 `operator[]`。
-  **`unordered_set`**：不提供 `operator[]`。
-  **`stack`**：容器适配器，不支持随机访问，无 `operator[]`。



以下关于`STL`各种容器和算法的``sort和`find`函数对重载运算符的描述正确的是**[多选]**

**A	二叉树类型的容器的`sort`和`find`都会调用`operator <`**

**B	线性类型容器`sort`会调用`operator <`**

C	二叉树类型的容器的`find`会调用`operator ==`

**D	线性类型容器使用`std::find`会调用`operator ==`**

- **A正确**：
- **B正确**：线性类型容器（如 `vector`、`list`）的排序操作（`std::sort` 或 `list::sort`）默认使用 `operator<` 进行比较，因此描述正确（在默认情况下）。
- **C错误**：二叉树类型的容器的 `find` 基于等价性，使用比较函数（默认 `operator<`）而非 `operator==`，因此描述错误。
- **D正确**：线性类型容器使用 `std::find` 算法时，默认通过 `operator==` 判断相等性，因此描述正确（在默认情况下）。

## 示例1

下面 C++ 程序的运行结果为（）

```c++
#include <iostream>
#include <vector>
#include <algorithm>    // 算法库（包含sort函数）
using namespace std;
 
bool cmp(pair<int, int> a, pair<int, int> b) {
    return a.second < b.second;// 比较第二个元素，从小到大排序
}
 
int main() {
    std::vector<std::pair<int, int>> v_p;// 存储整数对的向量
 
    v_p.emplace_back(1, 2);  // 添加 (1, 2)
    v_p.emplace_back(2, 1);  // 添加 (2, 1)
    v_p.emplace_back(3, 4);  // 添加 (3, 4)
    v_p.emplace_back(4, 3);  // 添加 (4, 3)
 
    sort(v_p.begin(), v_p.end(), cmp);    
    //这个函数告诉sort函数如何比较两个pair：只看它们的第二个值（.second）。

 
    for (auto t : v_p)
    {
        std::cout << t.first << " " << t.second << std::endl;
    }
    return 0;
}
```

这段代码创建了一个存储**整数对（pair）** 的向量（vector），按照每个整数对的**第二个值**进行排序，然后输出排序结果。

### **`pair<int, int>`**

- 一个包含两个整数的结构
- 可以通过 `.first` 和 `.second` 访问两个值
- 如：`(1, 2)` 中，`first = 1`, `second = 2`

### **`emplace_back`**

- 直接在容器末尾构造元素，比 `push_back` 更高效
- `emplace_back(1, 2)` 直接在向量中构造 `pair(1, 2)`

### **`sort` 函数**

- 对指定范围内的元素排序
- `v_p.begin()` 指向第一个元素
- `v_p.end()` 指向最后一个元素的下一个位置
- `cmp` 是自定义的比较规则

### **范围for循环**

```c++
for (auto t : v_p)  // 遍历v_p中的每个元素
```

- `auto t` 会自动推导类型为 `pair<int, int>`
- 每次循环，`t` 是向量中一个元素的副本



## vector

下面关于 C++ 中 vector 容器的说法错误的是（）[**多选**]

A	一个指向 vector 的 iterator 在 push_back 后会失效

B	vector 可以在其任意位置添加元素

C	vector 的 erase 成员函数可以删除其任意位置 iterator 对应的元素，为了实现这一功能，vector 的实际数据采用不连续的内存存储

D	vector 会动态调整容量，当容量不足时，会申请一块更大的空间，然后只使用拷贝构造函数，将原始数据拷贝到新空间中

- **A 错误**：
  - 向 vector 添加元素时，如果导致容量不足并触发重新分配内存，所有迭代器都会失效。
  - 若未重新分配，则除 `end()` 外的迭代器通常保持有效，(容量足够时原迭代器仍有效（end 迭代器会变)）
- **B 正确**：vector 通过 `insert` 方法可以在任意位置添加元素。
- **C 错误**：vector 底层采用**连续内存**存储数据，而不是不连续的内存。`erase` 可以删除任意位置元素，但删除后需要移动后续元素以保持连续性。
- **D错误**：vector 容量不足时会申请更大空间，并将原数据拷贝或移动到新空间（对于类类型对象，通常使用拷贝构造函数，但 C++11 后可能使用移动构造函数）。表述中“只使用拷贝构造函数”不够全面



下面C++代码的运行结果为：

```c++
#include <iostream>
#include <vector>
 
class MyClass {
public:
    MyClass() {
        std::cout << "Constructor" << std::endl;
    }
 
    MyClass(const MyClass&) {
        std::cout << "Copy Constructor" << std::endl;
    }
 
    ~MyClass() {
        std::cout << "Destructor" << std::endl;
    }
};
 
int main() {
    std::vector<MyClass> v; // 容量 = 0，大小 = 0
    v.emplace_back();// 容量从 0 → 1，构造第1个元素
    v.emplace_back();// 容量 1 → 2，需要重新分配：
     					// 1. 分配新内存（容量=2）
                         // 2. 复制旧元素（拷贝构造）
                         // 3. 构造新元素
                         // 4. 销毁旧元素
    return 0;
}
```



有以下程序

```c++
#include <iostream>
#include <vector>
using namespace std;
int main() {
    vector<int> A(10);
    int count=0,n;
    cout<<"请输入n的值：";
    cin>>n;
    A.__________(n);
    for(int i=2;i<=n;i++)
        if(i%3==0&&i%5==0) A[count++]=i;
    for(i=0;i<count;i++)
        cout<<A[i]<<" ";
    cout<<endl;
}
```

当键盘输入20，程序的运行结果是15，请为横线处选择合适的程序（   ）

A	size

B	reserve

C	resize

D	length

程序的目的是找出 2 到 n 之间同时能被 3 和 5 整除的数，并存储到向量 `A` 中。当输入 n=20 时，满足条件的数只有 15，因此输出结果为 15。

向量 `A` 初始大小为 10，但若 n 较大时，满足条件的数可能超过 10 个，直接通过下标赋值 `A[count++]` 可能导致越界。因此，需要通过成员函数调整向量的大小，确保有足够的空间存储数据。

- **A. `size`**：`size()` 函数用于获取当前向量的大小，不接受参数，因此 `A.size(n)` 语法错误。
- **B. `reserve`**：`reserve(n)` 函数为向量预留容量，但不改变其大小。使用下标赋值仍需保证索引小于当前大小，否则可能越界。
- **C,正确. `resize`**：`resize(n)` 函数将向量的大小调整为 n，确保有足够的空间进行下标访问，符合程序需求。
- **D. `length`**：`vector` 没有 `length` 成员函数，语法错误。

因此，横线处应填入 `resize`，即选项 C。

## 迭代器

下面 C++ 代码段中空白处应填入的代码正确的是（  ）

```c++
void delete_obj(std::vector<Obj> &v_obj) {
    std::vector<Obj>::iterator it = v_obj.begin();
    while(it != v_obj.end()) {
        if(!it->isEmpty()) {
            ______ //此处填代码
        }else {
            ++it;
        }
    }
}
```


A	`it = v_obj.erase(it++);`

B	`v_obj.erase(it);`

C	`it = v_obj.erase(it); it++;`

D	`v_obj.erase(it++);`

**`erase`返回一个指向被删除元素之后位置的迭代器**（若删除的是最后一个元素，则返回 `end()`）。

- A:**正确**，删除`it`当前迭代器指向的元素，然后返回被删除元素之后的迭代器，并通过it接收

- B: 错误，虽然删除了`it`当前迭代器指向的元素，但是没有通过迭代器接收返回的新的迭代器，

  当 `std::vector` 进行容量调整时，迭代器就失效了，不使用新的迭代器接收会



```c++
CONTAINER::iterator iter, tempIt;
for (iter = cont.begin(); iter != cont.end();)    　　
{
    tempIt = iter;
    ++iter;
    cont.erase(tempIt);
    　　
}
```

假设cont是一个CONTAINER的实例，里面包含数个元素，那么当CONTAINER为：

1、vector  2、list  3、map  4、`deque`

会导致上面的代码片段崩溃的CONTAINER类型是？

A	1，4

B	2，3

C	1，3

D	2，4

对于 vector 和 `deque` 这样的顺序容器，在 `erase` 操作后，所有指向被删除元素及其之后元素的迭代器都会失效。而代码中虽然先递增了 iter 并将其指向下一个元素，但在删除 tempIt 后，iter 可能已经失效，导致后续循环条件判断出现未定义行为，可能引发崩溃。对于 list 和 map，erase 操作通常只使被删除元素的迭代器失效，不会影响其他迭代器，因此可以安全运行。

因此，会导致崩溃的容器类型是 vector 和 `deque`，对应选项 **A**。

## 

# string

## `sort`

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

