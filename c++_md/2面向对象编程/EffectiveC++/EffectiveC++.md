# 一篇文章学完 Effective C++：条款 & 实践

https://zhuanlan.zhihu.com/p/613356779

# 第一章：让自己习惯 C++

## 条款 1：视 C++ 为一个语言联邦

C++ 拥有多种不同的编程范式，而这些范式集成在一个语言中，使得 C++ 是一门即灵活又复杂的语言：

- **C++主要的四个次语言：**
  1. 传统的面向过程 C：区块，语句，预处理器，内置数据类型，数组，指针。
  2. Object-Oriented C++(面向对象的C++)  C with Classes：类，封装，继承，多态，动态绑定。
  3. 模板编程 Template C++ 和堪称黑魔法的模板元编程（TMP）。
  4. C++ 标准库 STL。

C++ 高效编程守则视情况而变化，程序设计没有银弹。

## 条款 2：尽量以 const, enum, inline 替换 #define

### `constexpr` - 编译时计算

`constexpr`（常量表达式）是C++11引入的关键字，用于指定**编译时就能计算出结果的表达式、变量或函数**。它允许**将计算从运行时移到编译时**，提供了零开销抽象的可能性,**提高性能并增强类型安全**。

- ## **基本作用**

  - **编译时求值**：确保值在编译阶段就能确定

  - **替换宏**：提供类型安全、可调试的编译时常量

  - **性能优化**：消除运行时开销

在原书写成时 C++11 中的`constexpr`（**常量表达式**）还未诞生，现在一般认为应当用`constexpr`定义编译期常量来替代大部分的`#define`宏常量定义：

```c++
#define ASPECT_RATIO 1.653
//替换为：
constexpr auto aspect_ratio = 1.653;
```

- ## 基本用法

```cpp
constexpr int size = 10;               //  编译时常量，10是常量表达式
constexpr int doubleSize = size * 2	  //size是常量表达式，所以size*2也是常量表达式
constexpr double pi = 3.1415926535;   //  浮点数也可以
constexpr int arr[size];               //  可用作数组大小

int a = 5;
constexpr int b = a; // 	❌错误，a不是常量表达式
const int runtime_val = getValue();    // ❌ 运行时确定
constexpr int wrong = runtime_val;     // ❌ 编译错误
```

### 与`const`区别

```c++
const int a = 42;          // 运行时或编译时确定
constexpr int b = 42;      // 必须是编译时确定

int x = 5;
const int c = x;           // ✅ 允许（运行时确定）
constexpr int d = x;       // ❌ 编译错误
```

将编译期常量定义为类的静态成员：

```cpp
class GamePlayer {
private:
    constexpr int size = 10; 
    static constexpr auto NumTurns = size;
};

class GamePlayer{
private:
    static const int NumTurns = 5; //static class 常量声明
    int scoures[NumTurns]//使用该常量
};
const int GamePlayer::NumTurns;//static calss NumTurns常量定义

class ConstEstimate{
private:
    static const double FudgeFactor;//常量声明，位于头文件内
};
const dobule ConstEstimate::FudegFactor = 1.35;//常量定义，位于实现文件内
```

### `enum`

`enum`可以用于替代整型的常量，并且在模板元编程中应用广泛（见条款 48）

```cpp
class GamePlayer {
private:
    enum { NumTurns = 5 };//令NumTurns成为5的一个记号
    int scores[NumTruns];
};
```

### `inline`

大部分`#define`宏常量应当用**内联模板函数**（`template inline`）替代：

```cpp
#define CALL_WITH_MAX(a, b) f((a) > (b) ? (a) : (b))
//替代为：
template<typename T>
inline void CallWithMax(const T& a, const T& b) {
    f(a > b ? a : b);
}
```

需要注意的是，宏和函数的行为本身并不完全一致，宏只是简单的替换，并不涉及传参和复制。

### 总结：

- 对于单纯的常量，最好使用**`const`对象或`enum`**替换`#define`
- 对于形似 函数的宏(`macros`)，最好使用**内联模板函数**替换`#define`

- 现代C++优先使用`constexpr`

## 条款3：尽可能使用`const`

- ## 把一个声明从右向左读。

**int \* const a** ： ( * 读成 pointer to ) `a is a const pointer to int` // a 是一个指针，这个指针的地址不能改变，即指针的指向不能改变，但是可以利用这个指针改变值。

**const int \* a :** `a is a pointer to const int`; //a是一个指针，这个指针指向的地址存放的是一个不允许更改的int。即指针可以指向别的位置（指针的指向可以改变），但是就是不能通过该指针改变地址中的值。

关键字`const`在`*`号右边，说明修饰的是指针本身，指针指向是固定的，不能再乱指了。

关键字`const`在`*`左边，说明修饰的是指针指向的内容，指针指向的是一个常量，它的值是固定的，不能在乱改大小了。

如：

```c++
char greeting[] = "Hello";
char* p = greeting;                // 指针可修改，数据可修改
const char* p = greeting;          // 指针可修改，数据不可修改
char const* p = greeting;          // 指针可修改，数据不可修改
char* const p = greeting;          // 指针不可修改，数据可修改
const char* const p = greeting;    // 指针不可修改，数据不可修改
```

- ## 对于 STL 迭代器，分清使用`const`还是`const_iterator`：

```c++
const std::vector<int>::iterator iter = vec.begin();    // 迭代器不可修改，数据可修改
std::vector<int>::const_iterator iter = vec.begin();    // 迭代器可修改，数据不可修改
```

`const std::vector<int>::iterator iter`类似于`int *const p`,指针`iter`本身是常量，不能改变指向，但可以改变指向的元素值

`std::vector<int>::const_iterator iter`类似于`const int *p`,指针`iter`本身可以移动(可以改变指向)，但是不能修改指向的元素值

- ## 如果不想让函数的结果被无意义的当做左值，要使用`const`

```cpp
class Rational{...};
const Rational operator*(const Rational& lhs, const Rational& rhs);
```

重载了`*`运算符，函数返回`const`，避免了以下这种情况

```cpp
if(a* b = c)//这是很有可能的事情，毕竟人难免有失误的时候
//让函数返回一个const，则结果不能放在等号左侧，避免了以上的行为
```

而这时候，将函数的返回值声明为const 可以预防这个没有实际意义的赋值动作，这即是原因。

### const成员函数详解

#### 1.1 什么是const成员函数

const成员函数是指被`const`修饰的成员函数，它承诺**不会修改对象的成员变量**（mutable成员除外）。

```cpp
class MyClass {
public:
    void nonConstFunc();      // 非const成员函数
    void constFunc() const;   // const成员函数
};
```

#### 2.1 重载的const版本
同一个成员函数可以有const和non-const两个版本，编译器会根据调用对象的constness选择合适的版本。

- **const对象**：只能调用const成员函数
- **non-const对象**：可以调用const和非const成员函数

```c++
class Example {
public:
    void print() const {
        cout << "const version" << endl;
    }
    
    void print() {
        cout << "non-const version" << endl;
    }
};

int main() {
    const Example obj1;  // const对象
    Example obj2;        // non-const对象
    
    obj1.print();  // 调用const版本
    obj2.print();  // 调用non-const版本
}
```



```cpp
class TextBlock {
public:
    // const对象使用的重载版本
    const char& operator[](std::size_t position) const {
        return text[position];
    }
    
    // non-const对象使用的重载版本
    char& operator[](std::size_t position) {
        return text[position];
    }

private:
    std::string text;
};
void Print(const TextBlock& ctb) {
    std::cout << ctb[0];  // 调用 const TextBlock::operator[]
}

int main() {
    TextBlock tb;                 // non-const对象
    const TextBlock ctb;          // const对象
    
    tb[0] = 'A';      // 正确：调用non-const版本，可以修改
    // ctb[0] = 'B';  // 错误：调用const版本，返回const引用，不能修改
    
    char c1 = tb[0];   // 调用non-const版本
    char c2 = ctb[0];  // 调用const版本
    
    Print(tb);         // 参数类型是const引用，调用const版本
    Print(ctb);        // 调用const版本
}
```

#### 3.1 编译器视角：bitwise constness
编译器采用**物理常量性**检查，确保const成员函数不会修改任何非mutable成员变量的二进制位。

```cpp
class BitwiseExample {
    int data;
public:
    int getData() const {
        // data = 10;  // ❌ 编译器错误：试图修改成员变量
        return data;
    }
};
```

#### 3.2 程序员视角：logical constness
程序员关心的是**逻辑常量性**：const应该保证对象的逻辑状态不变，但允许修改一些不影响"常量观感"的实现细节。

#### 3.3 mutable关键字
`mutable`用于标记那些在const成员函数中**允许被修改**的成员变量。

```cpp
class CachedData {
private:
    std::string data;
    mutable bool cache_valid;      // 可以修改
    mutable std::string cached_result;  // 可以修改
    
public:
    const std::string& getResult() const {
        if (!cache_valid) {
            // 重新计算结果并缓存
            cached_result = computeExpensiveResult();
            cache_valid = true;  // ✅ 允许：修改mutable成员
        }
        return cached_result;
    }
    
private:
    std::string computeExpensiveResult() const {
        // 复杂的计算
        return data + " processed";
    }
};
```

#### 4.2 避免代码重复的技巧

```cpp
class TextBlock {
private:
    std::string text;
    
public:
    // const版本是基础实现
    const char& operator[](std::size_t position) const {
        // 边界检查、日志记录等
        return text[position];
    }
    
    // non-const版本通过const_cast调用const版本
    char& operator[](std::size_t position) {
        // 避免重复代码：调用const版本，然后移除const
        return const_cast<char&>(
            static_cast<const TextBlock&>(*this)[position]
        );
    }
};
```

#### 4.3 线程安全的const成员函数

```cpp
class ThreadSafeContainer {
private:
    std::vector<int> data;
    mutable std::mutex mtx;  // mutable：const方法也需要锁
    
public:
    // const方法也需要线程安全
    bool contains(int value) const {
        std::lock_guard<std::mutex> lock(mtx);  // 修改mutex状态
        return std::find(data.begin(), data.end(), value) != data.end();
    }
    
    size_t size() const {
        std::lock_guard<std::mutex> lock(mtx);
        return data.size();
    }
};
```

#### 五、常见陷阱与解决方案

- #### 5.1 返回内部数据的引用

```cpp
class Problematic {
private:
    std::vector<int> data;
    
public:
    // ❌ 危险：返回内部数据的引用
    std::vector<int>& getData() const {
        return data;  // 错误：data不是mutable
    }
    
    // ✅ 正确做法
    const std::vector<int>& getData() const {
        return data;  // 返回const引用
    }
};
```

- #### 5.2 mutable的滥用

```cpp
class BadDesign {
private:
    mutable int important_state;  // ❌ 滥用mutable
    
public:
    int getState() const {
        important_state = 42;  // 修改了逻辑状态
        return important_state;
    }
};

class GoodDesign {
private:
    int state;
    mutable int access_count;  // ✅ 合理使用
    
public:
    int getState() const {
        access_count++;  // 不影响逻辑状态
        return state;
    }
};
```

#### 六、总结要点

1. **const成员函数**保证不修改对象状态（mutable成员除外）
2. **函数重载**允许const和non-const版本共存
3. **bitwise constness**是编译器的检查标准
4. **logical constness**是程序员的实现目标
5. **mutable关键字**在保持逻辑常量性的同时允许必要的修改
6. **设计原则**：
   1. **尽可能使用const**：对于不修改对象的成员函数，都声明为const
   2. **避免代码重复**：当const和non-const版本实现相同时，使用const_cast
   3. **明确标记mutable**：只在确实需要的地方使用mutable
   4. 当const和non-const成员函数有着实质等价的实现时，令non-const版本调用const版本可**避免代码重复**

通过正确理解和运用const成员函数，可以：
- 提高代码的可读性和安全性
- 实现更精确的接口设计
- 避免意外的数据修改
- 支持更多的优化机会

## 条款 4：确定对象在使用前已被初始化

# 第二章：构造/析构/赋值运算

## 条款 7：为多态基类声明虚析构函数

- #### 子类（派生类）的析构函数在完成自己的清理工作后，会自动调用基类的析构函数。

当派生类对象经由一个基类指针被删除，而该基类指针带着一个非虚析构函数，其结果是未定义的，可能会无法完全销毁派生类的成员，造成内存泄漏。消除这个问题的方法就是对基类使用虚析构函数：

```cpp
class Base {
public:
    Base();
    virtual ~Base();
};
```

只要基类的析构函数是虚函数，那么派生类的析构函数不论是否用virtual关键字声明，都自动成为虚析构函数。

虚析构函数的运作方式是，最深层派生的那个类的析构函数最先被调用，然后是其上的基类的析构函数被依次调用。

- ## 纯虚析构函数

如果你想将基类作为抽象类使用，但手头上又没有别的虚函数，那么将它的析构函数设为纯虚函数是一个不错的想法。考虑以下情形：

纯虚析构函数是**同时具有`= 0`纯虚函数声明和函数体实现的特殊纯虚函数**。它使类成为抽象类（不能实例化），但与其他纯虚函数不同，**纯虚析构函数必须有定义（函数体）**。



当派生类对象被销毁时，析构函数的调用链是：

1. 调用派生类析构函数
2. **自动调用基类析构函数** ← 这里需要基类析构函数的定义

- **纯虚析构函数定义法1：**

```c++
class OldStyle {
public:
    virtual ~OldStyle() = 0;  // 声明
};

OldStyle::~OldStyle() {}      // 单独定义
```

- **纯虚析构函数定义法2**：

```c++
// C++11及以后：可以合并
class NewStyle {
public:
    virtual ~NewStyle() = 0 {}  // 声明和定义在一起
};
```

## RAII（Resource Acquisition Is Initialization）

**RAII** 是 C++ 最重要的设计哲学之一，直译为"**资源获取即初始化**"。它的核心思想是：

**将资源（内存、文件、锁等）的生命周期与对象的生命周期绑定**：

- **构造函数中获取资源**（分配内存、打开文件、获取锁）
- **析构函数中释放资源**（释放内存、关闭文件、释放锁）

## 条款 10：令 operator= 返回一个指向 *this 的引用

然并不强制执行此条款，但为了实现连锁赋值，大部分时候应该这样做：

```cpp
class Widget {
public:
    Widget& operator+=(const Widget& rhs) {    // 这个条款适用于
        ...                                    // +=, -=, *= 等等运算符
        return *this;
    }
    Widget& operator=(int rhs) {               // 即使参数类型不是 Widget& 也适用
        ...
        return *this;
    }
};
```

自我赋值是合法的操作，但在一些情况下可能会导致意外的错误，例如在复制堆上的资源时：

```cpp
Widget& operator+=(const Widget& rhs) {
    delete pRes;                          // 删除当前持有的资源
    pRes = new Resource(*rhs.pRes);       // 复制传入的资源
    return *this;
}
```

但若`rhs`和`*this`指向的是相同的对象，就会导致访问到已删除的数据。

最简单的解决方法是在执行后续语句前先进行**证同测试（Identity test）**：

```cpp
Widget& operator=(const Widget& rhs) {
    if (this == &rhs) return *this;        // 若是自我赋值，则不做任何事

    delete pRes;
    pRes = new Resource(*rhs.pRes);
    return *this;
}
```

另一个常见的做法是只关注异常安全性，而不关注是否自我赋值：

```cpp
Widget& operator=(const Widget& rhs) {
    Resource* pOrigin = pRes;             // 先记住原来的pRes指针
    pRes = new Resource(*rhs.pRes);       // 复制传入的资源
    delete pOrigin;                       // 删除原来的资源
    return *this;
}
```

仅仅是适当安排语句的顺序，就可以做到使整个过程具有异常安全性。

还有一种取巧的做法是使用 copy and swap 技术，这种技术聪明地利用了栈空间会自动释放的特性，这样就可以通过析构函数来实现资源的释放：

```cpp
Widget& operator=(const Widget& rhs) {
    Widget temp(rhs);
    std::swap(*this, temp);
    return *this;
}
```

上述做法还可以写得更加巧妙，就是利用按值传参，自动调用构造函数：

```cpp
Widget& operator=(Widget rhs) {
    std::swap(*this, rhs);
    return *this;
}
```

# 第三章：资源管理

# 第四章：设计与声明

## 条款 18：让接口容易被正确使用，不易被误用

## 条款 19：设计 class 犹如设计 type

**新 type 对象应该如何被创建和销毁？**

**对象的初始化和赋值该有什么样的差别？**

**新 type 的对象如果被按值传递，意味着什么？**

**什么是新 type 的合法值？**

**你的新 type 需要配合某个继承图系吗？**

**什么样的运算符和函数对此新 type 而言是合理的？	**

**什么样的标准函数应该被驳回？**

**谁该取用新 type 的成员？**

**什么是新 type 的“未声明接口”？**

**你的新 type 有多么一般化？**

## 条款 20：宁以按常引用传参替换按值传参

也并非永远都使用按引用传参，对于内置类型、STL的迭代器和函数对象，我们认为使用按值传参是比较合适的。

## 条款 21：必须返回对象时，别妄想返回其引用

**不要返回局部变量的引用**因为局部变量在离开函数时就被销毁了，除此之外，**返回一个指向局部静态变量的引用也是不被推荐的。**

尽管返回对象会调用拷贝构造函数产生开销，但这开销比起出错而言微不足道。

## 条款22：将成员变量声明为`private`

封装性，尽可能地隐藏类中的成员变量，并通过对外暴露函数接口来实现对成员变量的访问

## 条款 23：宁以非成员、非友元函数替换成员函数

虽然成员函数和非成员函数都可以完成我们的目标，但此处更建议使用非成员函数，这是为了遵守一个原则：**越少的代码可以访问数据，数据的封装性就越强**。

友元函数和成员函数拥有相同的权力，所以在能使用**非成员函数**完成任务的情况下，就不要使用友元函数和成员函数。

如果你觉得一个全局函数并不自然，也可以考虑将`ClearEverything`函数放在工具类中充当静态成员函数，或与`WebBrowser`放在同一个命名空间中：

```cpp
namespace WebBrowserStuff {
    class WebBrowser { ... };
    void ClearEverything(WebBrowser& wb) { ... }
}
```

## 条款 24：若所有参数皆需类型转换，请为此采用非成员函数

运算符重载时，可以先考虑非成员函数，如

现在我们手头上拥有一个`Rational`类，并且它可以和`int`隐式转换：

```cpp
class Rational {
public:
    Rational(int numerator = 0, int denominator = 1);
    ...
};
```

当然，我们需要重载乘法运算符来实现`Rational`对象之间的乘法：

```cpp
class Rational {
public:
    ...
    const Rational operator*(const Rational& rhs) const;
};
```

将运算符重载放在类中是行得通的，至少对于`Rational`对象来说是如此。但当我们考虑混合运算时，就会出现一个问题：

```cpp
Rational oneEight(1, 8);
Rational oneHalf(1, 2);
Rational result = oneHalf / oneEight;

result = oneHalf * 2;    // 正确
result = 2 * oneHalf;    // 报错
```

假如将乘法运算符写成函数形式，错误的原因就一目了然了：

```cpp
result = oneHalf.operator*(2);    // 正确
result = 2.operator*(oneHalf);    // 报错
```

在调用`operator*`时，`int`类型的变量会隐式转换为`Rational`对象，因此用`Rational`对象乘以`int`对象是合法的，但反过来则不是如此。

所以，为了避免这个错误，我们应当将运算符重载放在类外，作为非成员函数：

```cpp
const Rational operator*(const Rational& lhs, const Rational& rhs);
```

## 条款 25：考虑写出一个不抛异常的swap函数

`std::swap`函数 -》》》在 C++11 后改为了用`std::move`实现

# 第五章：实现

## 条款 26：尽可能延后变量定义式出现的时间

- 当变量定义出现时，程序需要承受其构造成本；
- 当变量离开其作用域时，程序需要承受其析构成本。

因此，避免不必要的变量定义，以及延后变量定义式直到你确实需要它。

延后变量定义式还有一个意义，即“默认构造+赋值”效率低于“直接构造”：

```c++
// 效率低
std::string encrypted;
encrypted = password;

// 效率高
std::string encrypted(password);
```

对于循环中变量的定义，我们一般有两种做法：

**A**. 定义于循环外，在循环中赋值：

```cpp
Widget w;
for (int i = 0; i < n; ++i) {
    w = 取决于 i 的某个值;
    ...
}
```

这种做法产生的开销：1 个构造函数 + 1 个析构函数 + n 个赋值操作

**B**. 定义于循环内：

```cpp
for (int i = 0; i < n; ++i) {
    Widget w(取决于 i 的某个值);
    ...
}
```

这种做法产生的开销：n 个构造函数 + n 个析构函数

由于做法A会将变量的作用域扩大，因此除非知道该变量的赋值成本比“构造+析构”成本低，或者对这段程序的效率要求非常高，否则建议使用做法B。

## 条款 27：少做转型动作

C 式转型：

```cpp
(T)expression
T(expression)
```

C++ 式转型：

```cpp
const_cast<T>(expression)
dynamic_cast<T>(expression)
reinterpret_cast<T>(expression)
static_cast<T>(expression)
```

- `const_cast`用于常量性转除，这也是唯一一个有这个能力的 C++ 式转型。
- `dynamic_cast`用于安全地向下转型，这也是唯一一个 C 式转型无法代替的转型操作，它会执行对继承体系的检查，因此会带来额外的开销。只有拥有虚函数的基类指针能进行`dynamic_cast`。
- `reinterpret_cast`用于在任意两个类型间进行低级转型，执行该转型可能会带来风险，也可能不具备移植性。
- `static_cast`用于进行强制隐式转换，也是最常用的转型操作，可以将内置数据类型互相转换，也可以将`void*`和typed指针，基类指针和派生类指针互相转换。



尽量在 C++ 程序中使用 C++ 式转型，因为 C++ 式转型操作功能更明确，可以避免不必要的错误。

**唯一使用 C 式转型的时机可能是在调用 explicit 构造函数时：**

```cpp
class Widget {
public:
    explicit Widget(int size);  // explicit 单参数构造函数
    ...
};

void DoSomeWork(const Widget& w);  // 接受 Widget 常引用

DoSomeWork(Widget(15));  // 调用方式一  C语言函数式类型转换
// 等价于 DoSomeWork(static_cast<Widget>(15));  // 调用方式二 C++ static_cast 显式类型转换

不允许的写法：
DoSomeWork(15);  // 错误！explicit 禁止隐式转换
```

`explicit` 构造函数意味着：
- **禁止隐式转换**：不能将 `int` 隐式转换为 `Widget`
- 必须显式调用构造函数

- ## 写法一：`Widget(15)`

  - 这是**函数式类型转换**（functional-style cast）

  - 直接调用构造函数创建临时 `Widget` 对象

  - 临时对象的生命周期：整个完整表达式（到分号结束）


- ## 写法二：`static_cast<Widget>(15)`

  - 这是 **static_cast 显式类型转换**

  - 同样调用构造函数创建临时对象

  - 与函数式类型转换语义相同

- ## 功能对比

```cpp
// 两者在功能上完全等价，都：
// 1. 创建临时 Widget 对象
// 2. 将该临时对象绑定到 const Widget& 参数
// 3. 在 DoSomeWork 调用期间有效

Widget(15);           // 函数式转换 - 更简洁
static_cast<Widget>(15);  // C++风格转换 - 更显式
```

- ## 临时对象的生命周期

```cpp
DoSomeWork(Widget(15));
// 等价于：
{
    const Widget& w = Widget(15);  // 临时对象绑定到引用
    DoSomeWork(w);                 // 在函数调用期间有效
}  // 临时对象在此销毁
```

- ## 实际应用建议

```cpp
// 推荐使用 Widget(15) - 更简洁
DoSomeWork(Widget(15));

// 在模板编程中可能需要 static_cast
template<typename T>
void process(T value) {
    DoSomeWork(static_cast<Widget>(value));
}

// 注意：以下写法是错误的
DoSomeWork(15);  // explicit 禁止隐式转换
```

尽量在 C++ 程序中使用 C++ 式转型，因为 C++ 式转型操作功能更明确，可以避免不必要的错误。

**唯一使用 C 式转型的时机可能是在调用 explicit 构造函数时：**

`explicit` 构造函数强制调用者显式创建对象，提高代码安全性，避免意外的类型转换。两种写法都是合法的显式转换方式。

需要注意的是，转型并非什么都没有做，而是可能会更改数据的底层表述，或者为指针附加偏移值，这和具体平台有关，因此不要妄图去揣测转型后对象的具体布局方式。

避免对`*this`进行转型，参考以下例子：

```cpp
class Window {
public:
    virtual void OnResize() { ... }
    ...
};

class SpecialWindow : public Window {
public:
    virtual void OnResize() {
        static_cast<Window>(*this).OnResize();
        ...
    }
    ...
};
```

这段代码试图通过转型`*this`来调用基类的虚函数，然而这是严重错误的，这样做会得到一个新的`Window`副本并在该副本上调用函数，而非在原本的对象上调用函数。

正确的做法如下：

```cpp
class SpecialWindow : public Window {
public:
    virtual void OnResize() {
        Window::OnResize();
        ...
    }
    ...
};
```

当你想知道一个基类指针是否指向一个派生类对象时，你需要用到`dynamic_cast`，如果不满足，则会产生报错。但是对于继承体系的检查可能是非常慢的，所以在注重效率的程序中应当避免使用`dynamic_cast`，改用`static_cast`或别的代替方法。

## 条款 28：避免返回 handles 指向对象的内部成分

**Handles（句柄）** 是指：

- **引用（references）**
- **指针（pointers）**
- **迭代器（iterators）**

这些都被称为"句柄"，因为它们都提供了一种访问或引用另一个对象的**间接方式**。

考虑以下`Rectangle`类：

```cpp
struct RectData {
    Point ulhc;
    Point lrhc;
};

class Rectangle {
public:
    Point& UpperLeft() const { return pData->ulhc; }
    Point& LowerRight() const { return pData->lrhc; }

private:
    std::shared_ptr<RectData> pData;
};
```

这段代码看起来没有任何问题，但其实是在做自我矛盾的事情：我们通过const成员函数返回了一个指向成员变量的引用，这使得成员变量可以在外部被修改，而这是违反 logical constness 的原则的。换句话说，你**绝对不应该令成员函数返回一个指针指向“访问级别较低”的成员函数**。

改成返回常引用可以避免对成员变量的修改：

```cpp
const Point& UpperLeft() const { return pData->ulhc; }
const Point& LowerRight() const { return pData->lrhc; }
```

但是这样依然会带来一个称作 **dangling handles（空悬句柄）** 的问题，当对象不复存在时，你将无法通过引用获取到返回的数据。

采用最保守的做法，返回一个成员变量的副本：

```cpp
Point UpperLeft() const { return pData->ulhc; }
Point LowerRight() const { return pData->lrhc; }
```

避免返回 handles（包括引用、指针、迭代器）指向对象内部。遵循这个条款可增加封装性，使得const成员函数的行为符合常量性，并将发生 “空悬句柄” 的可能性降到最低。设计类接口时应尽量避免这种做法。

- ## 实际建议

1. **最小化句柄返回**：除非必要，不要返回句柄
2. **优先返回副本**：对于小型对象（如Point）
3. **明确文档说明**：如果必须返回句柄，说明生命周期
4. **考虑智能指针**：使用shared_ptr管理共享所有权

## 条款 29：为“异常安全”而努力是值得的

异常安全函数提供以下三个保证之一：

**基本承诺：** 如果异常被抛出，程序内的任何事物仍然保持在有效状态下，没有任何对象或数据结构会因此败坏，所有对象都处于一种内部前后一致的状态，然而程序的真实状态是不可知的，也就是说客户需要额外检查程序处于哪种状态并作出对应的处理。

**强烈保证：** 如果异常被抛出，程序状态完全不改变，换句话说，程序会回复到“调用函数之前”的状态。

**不抛掷（nothrow）保证：** 承诺绝不抛出异常，因为程序总是能完成原先承诺的功能。作用于内置类型身上的所有操作都提供 nothrow 保证。

原书中实现 nothrow 的方法是`throw()`，不过这套异常规范在 C++11 中已经被弃用，取而代之的是`noexcept`关键字：

```cpp
int DoSomething() noexcept;
```

注意，使用`noexcept`并不代表函数绝对不会抛出异常，而是在抛出异常时，将代表出现严重错误，会有意想不到的函数被调用（可以通过`set_unexpected`设置），接着程序会直接崩溃。

当异常被抛出时，带有异常安全性的函数会：

1. 不泄漏任何资源。
2. 不允许数据败坏。

考虑以下`PrettyMenu`的`ChangeBackground`函数：

```cpp
class PrettyMenu {
public:
    ...
    void ChangeBackground(std::vector<uint8_t>& imgSrc);
    ...
private:
    Mutex mutex;        // 互斥锁
    Image* bgImage;     // 目前的背景图像
    int imageChanges;   // 背景图像被改变的次数
};

void PrettyMenu::ChangeBackground(std::vector<uint8_t>& imgSrc) {
    lock(&mutex);
    delete bgImage;
    ++imageChanges;
    bgImage = new Image(imgSrc);
    unlock(&mutex);
}
```

很明显这个函数不满足我们所说的具有异常安全性的任何一个条件，若在函数中抛出异常，`mutex`会发生资源泄漏，`bgImage`和`imageChanges`也会发生数据败坏。

通过以对象管理资源，使用智能指针和调换代码顺序，我们能将其变成一个具有强烈保证的异常安全函数：

```cpp
void PrettyMenu::ChangeBackground(std::vector<uint8_t>& imgSrc) {
    Lock m1(&mutex);
    bgImage.reset(std::make_shared<Image>(imgSrc));

    ++imageChanges;
}
```

另一个常用于提供强烈保证的方法是我们所提到过的 copy and swap，为你打算修改的对象做出一份副本，对副本执行修改，并在所有修改都成功执行后，用一个不会抛出异常的swap方法将原件和副本交换：

```cpp
struct PMImpl {
    std::shared_ptr<Image> bgImage;
    int imageChanges;
};

class PrettyMenu {
    ...
private:
    Mutex mutex;
    std::shared_ptr<PMImpl> pImpl;
};

void PrettyMenu::ChangeBackground(std::vector<uint8_t>& imgSrc) {
    Lock m1(&mutex);

    auto pNew = std::make_shared<PMImpl>(*pImpl);    // 获取副本
    pNew->bgImage.reset(std::make_shared<Image>(imgSrc));
    ++pNew->imageChanges;

    std::swap(pImpl, pNew);
}
```

当一个函数调用其它函数时，函数提供的“异常安全保证”通常最高只等于其所调用的各个函数的“异常安全保证”中的最弱者。

强烈保证并非永远都是可实现的，特别是当函数在操控非局部对象时，这时就只能退而求其次选择不那么美好的基本承诺，并将该决定写入文档，让其他人维护时不至于毫无心理准备。

## 条款 30：透彻了解 inlining 的里里外外

将函数声明为内联一共有两种方法，一种是为其显式指定`inline`关键字，另一种是直接将成员函数的定义式写在类中，如下所示：

```cpp
class Person {
public:
    ...
    int Age() const { return theAge; }  // 隐式声明为 inline
    ...
private:
    int theAge;
};
```

在`inline`诞生之初，它被当作是一种对编译器的优化建议，即将“对此函数的每一个调用”都以函数本体替换之。但在编译器的具体实现中，该行为完全被优化等级所控制，与函数是否内联无关。

在现在的 C++ 标准中，`inline`作为优化建议的含义已经被完全抛弃，取而代之的是“允许函数在不同编译单元中多重定义”，使得可以在头文件中直接给出函数的实现。

在 C++17 中，引入了一个新的`inline`用法，使静态成员变量可以在类中直接定义：

```cpp
class Person {
public:
    ...
private:
    static inline int theAge = 0;  // since C++17
};
```

## 条款 31：将文件间的编译依存关系降至最低

C++ 坚持将类的实现细节放置于类的定义式中，这就意味着，即使你只改变类的实现而不改变类的接口，在构建程序时依然需要重新编译。这个问题的根源出在编译器必须在编译期间知道对象的大小，如果看不到类的定义式，就没有办法为对象分配内存。也就是说，C++ 并没有把“将接口从实现中分离”这件事做得很好。

**用“声明的依存性”替换“定义的依存性”：**

我们可以玩一个“将对象实现细目隐藏于一个指针背后”的游戏，称作 **pimpl idiom（pimpl 是 pointer to implemention 的缩写）**：将原来的一个类分割为两个类，一个只提供接口，另一个负责实现该接口，称作**句柄类（handle class）**：

```cpp
// person.hpp 负责声明类

class PersonImpl;

class Person {
public:
    Person();
    void Print();
    ...
private:
    std::shared_ptr<PersonImpl> pImpl;
};

// person.cpp 负责实现类

class PersonImpl {
public:
    int data{ 0 };
};

Person::Person() {
    pImpl = std::make_shared<PersonImpl>();
}

void Person::Print() {
    std::cout << pImpl->data;
}
```

这样，假如我们要修改`Person`的private成员，就只需要修改`PersonImpl`中的内容，而`PersonImpl`的具体实现是被隐藏起来的，对它的任何修改都不会使得`Person`客户端重新编译，真正实现了“类的接口和实现分离”。

**如果使用对象引用或对象指针可以完成任务，就不要使用对象本身：**

你可以只靠一个类型声明式就定义出指向该类型的引用和指针；但如果定义某类型的对象，就需要用到该类型的定义式。

**如果能够，尽量以类声明式替换类定义式：**

当你在声明一个函数而它用到某个类时，你不需要该类的定义；但当你触及到该函数的定义式后，就必须也知道类的定义：

```cpp
class Date;                     // 类的声明式
Date Today();
void ClearAppointments(Date d); // 此处并不需要得知类的定义
```

**为声明式和定义式提供不同的头文件：**

为了避免频繁地添加声明，我们应该为所有要用的类声明提供一个头文件，这种做法对 template 也适用：

```cpp
#include "datefwd.h"            // 这个头文件内声明 class Date
Date Today();
void ClearAppointments(Date d);
```

此处的头文件命名方式`"datefwd.h"`取自标准库中的`<iosfwd>`。

上面我们讲述了接口与实现分离的其中一个方法——提供句柄类，另一个方法就是将句柄类定义为抽象基类，称作**接口类（interface class）**：

```cpp
class Person {
public:
    virtual ~Person() {}
    virtual void Print();
    ...
};
```

为了将`Person`对象实际创建出来，我们一般采用工厂模式。可以尝试在类中塞入一个静态成员函数`Create`用于创建对象：

```cpp
class Person {
public:
    ...
    static std::shared_ptr<Person> Create();
    ...
};
```

但此时`Create`函数还无法使用，需要在派生类中给出`Person`类中的函数的具体实现：

```cpp
class RealPerson : public Person {
public:
    RealPerson(...) { ... }
    virtual ~RealPerson() {}
    void Print() override { ... }

private:
    int data{ 0 };
};
```

完成`Create`函数的定义：

```cpp
static std::shared_ptr<Person> Person::Create() {
    return std::make_shared<RealPerson>();
}
```

毫无疑问的是，句柄类和接口类都需要额外的开销：句柄类需要通过 pimpl 取得对象数据，增加一层间接访问、指针大小和动态分配内存带来的开销；而接口类会增加存储虚表指针和实现虚函数跳转带来的开销。

而当这些开销过于重大以至于类之间的耦合度在相形之下不成为关键时，就以具象类（concrete class）替换句柄类和接口类。

# 第六章：继承与面向对象设计

## 条款 32：确定你的public继承塑模出 is-a 关系

​	“public继承”意味着 is-a，所谓 is-a，就是指适用于基类身上的每一件事情一定也适用于继承类身上，因为我们可以认为每一个派生类对象也都是一个基类对象。

这看似很自然，但在面对自然语言的表述时，往往会产生歧义。

考虑`Bird`类和`Penguin`类的继承关系：

```c++
class Bird {//鸟类
public:
    virtual void Fly();
    ...
};

class Penguin : public Bird { //企鹅
    ...
};
```

`Penguin`类会获得来自`Bird`类的飞行方法，这就造成了误解，因为企鹅恰恰是不会飞的鸟类。一种解决方法是当调用`Penguin`类中的`Fly`函数时，抛出一个运行期错误，但这种做法通常不够直观；另一个解决方法是使用双继承，区分会飞和不会飞的鸟类：

```c++
class Bird {
public:
    ...
};

class FlyingBird : public Bird {//会飞的鸟类
public:
    virtual void Fly();
    ...
};

class Penguin : public FlyingBird {//不会飞的鸟类
    ...
};
```

但若要处理鸟类的多钟不同属性时，双继承模式就不太管用了，因此我们总是说程序设计没有银弹。

另一个常见的例子是用`Square`(正方形)类继承自`Rectangle`(矩形)类，从几何学的角度来讲这很自然，然而正方形的长宽是相等的，矩形却不是如此，因此`Square`类和`Rectangle`类也无法满足严格的 is-a 关系。

## 条款 33：避免遮掩继承而来的名称

之前我们了解过 C++ 名称查找法则，这在继承体系中也是类似的，当我们在派生类中使用到一个名字时，编译器会优先查找派生类覆盖的作用域，如果没找到，再去查找基类的作用域，最后再查找全局作用域。

考虑以下情形：

```c++
class Base {
public:
    void mf();
    void mf(double);
};

class Derived : public Base {
public:
    void mf();
};
```

以上是简单的类声明，以下为了测试将类定义补全了

```c++
class Base {
public:
    void mf(){ cout << "Base::mf()" << endl; };// 版本1：无参数
    void mf(double){ cout << "Base::mf(double)" << endl; };// 版本2：带double参数（函数重载）
};

class Derived : public Base {
public:
    void mf() {cout << "Derived::mf()" << endl; }// 版本3：无参数（隐藏了基类的所有同名函数）
};
int main() {
    Derived d;
    d.mf();    		 // ✓ 调用 Derived::mf()
    d.mf(3.14);      // ✗ 编译错误！Base::mf(double) 被隐藏了
	d.Base::mf();    // ✓ 明确指定基类作用域    
    d.Base::mf(3.14);// ✓ 明确指定基类作用域
    return 0;
}
```

这样会导致派生类无法使用来自基类的重载函数，因为派生类中的名称`mf`掩盖了来自基类的名称`mf`。

- #### 对于名称掩盖问题的一种方法是使用`using`关键字：

```c++
class Derived : public Base {
public:
    using Base::mf;//引入基类的mf函数
    void mf();
};
```

`using`关键字会将基类中所有使用到名称`mf`的函数全部包含在派生类中，包括其重载版本。

```c++
#include <iostream>
using namespace std;

class Base {
public:
    //这两个mf构成函数重载
    void mf() { cout << "Base::mf()" << endl; }
    void mf(double) { cout << "Base::mf(double)" << endl; }
};

class Derived : public Base {
public:
    using Base::mf;  // 引入基类的两个mf
    void mf() {cout << "Derived::mf()" << endl;}
};

int main() {
    Derived d;
    d.mf();    //调用子类自己的mf，虽然引入了基类的无参版本，但是移动到子类中构成函数重写
    d.Base::mf();//调用基类的 mf无参版本
    d.mf(1.32);//调用基类的mf double参数版本
    return 0;
}
```

- #### 若有时我们不想要一个函数的全部版本，只想要单一版本（特别是在private继承时），可以考虑使用**转发函数（forwarding function）**：- 显式转发

```c++
#include <iostream>
using namespace std;

class Base {
public:
    void mf() { cout << "Base::mf()" << endl; }
    void mf(double) { cout << "Base::mf(double)" << endl; }
};

class Derived : public Base {
public:
    virtual void mf(double d) {
        Base::mf(d);
    }// 显式转发
 	void mf(){cout << "Derived::mf()" << endl;}
};

int main() {
    Derived d;
    d.mf();//子类自己的mf 无参函数
    d.mf(1.32);//在子类中的mf double参数版本中显式转发基类的mf函数
    return 0;
}
```

## 条款 34：区分接口继承和实现继承

| 特征         | 接口继承 | 实现继承    |
| :----------- | :------- | :---------- |
| **目的**     | 定义契约 | 代码复用    |
| **使用**     | 纯虚函数 | 非虚/虚函数 |
| **耦合度**   | 低       | 高          |
| **灵活性**   | 高       | 低          |
| **典型用途** | 多态基类 | 代码共享    |

## 条款 36：绝不重新定义继承而来的非虚函数

public继承意味着 is-a 关系，而在基类中声明一个非虚函数将会为该类建立起一种不变性（invariant），凌驾其特异性（specialization）。而若在派生类中重新定义该非虚函数，则会使人开始质疑是否该使用public继承的形式；如果必须使用，则又打破了基类“不变性凌驾特异性”的性质，就此产生了设计上的矛盾。

## 条款 37：绝不重新定义继承而来的缺省参数值

