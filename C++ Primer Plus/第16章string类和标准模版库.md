# 随机数

`std::srand(std::time(0))` 是C++中初始化伪随机数生成器种子的语句。让我详细解释：

## 组成部分

1. **`std::srand()`** - 设置随机数生成器的种子（seed）
2. **`std::time(0)`** - 获取当前时间（从1970年1月1日到现在的秒数）--- 用当前时间初始化随机数生成器，确保程序每次运行产生不同的随机序列。
3. **`0`** 作为 `time()` 的参数 - 表示获取当前时间戳

## 工作原理

```cpp
// 常用写法
std::srand(std::time(nullptr));  // 或 std::time(0)

// 然后使用 rand() 生成随机数
int randomNum = std::rand() % 100;  // 生成 0-99 的随机数
```

## 为什么需要设置种子？

- 如果不设置种子，`rand()` 每次都会生成**相同的随机数序列**
- 使用当前时间作为种子，可以确保**每次程序运行时**得到不同的随机序列

## 实际示例

```cpp
#include <iostream>
#include <cstdlib>  // 包含 srand() 和 rand()
#include <ctime>    // 包含 time()

int main() {
    // 设置种子
    std::srand(std::time(nullptr));
    
    // 生成5个随机数
    for (int i = 0; i < 5; i++) {
        std::cout << std::rand() << " ";
    }
    
    return 0;
}
// 每次运行输出不同的随机数
```

## 现代C++的替代方案

C++11 引入了更好的随机数库：

```cpp
#include <iostream>
#include <random>
#include <chrono>

int main() {
    // 使用随机设备
    std::random_device rd;
    
    // 使用Mersenne Twister引擎
    std::mt19937 gen(rd());
    
    // 定义分布范围
    std::uniform_int_distribution<> dis(1, 100);
    
    // 生成随机数
    for (int i = 0; i < 5; i++) {
        std::cout << dis(gen) << " ";
    }
    
    return 0;
}
```

## 注意事项

1. **不要频繁调用** `srand()` - 通常只在程序开始时调用一次
2. **并发问题** - 在多线程程序中，`rand()` 不是线程安全的
3. **随机性质量** - `rand()` 生成的随机数质量一般，不适合加密等用途
4. **NULL vs 0** - `std::time(0)` 和 `std::time(nullptr)` 是等价的

## 常见错误

```cpp
// 错误：在循环中多次调用 srand()
for (int i = 0; i < 10; i++) {
    std::srand(std::time(nullptr));  // 错误！时间可能相同
    std::cout << std::rand() << " ";
}
// 可能导致相同的"随机"数
```

# `string`类的构造函数

string的7个构造函数（用`ctor`标识，这是传统C++中构造函数的缩写）和	C++11新增的两个构造函数。

使用构造函数时都进行了简化，即隐藏了这样一个事实：string实际上是模板具体化`basic_string<char>`的一个`typedef`，同时省略了与内存管理相关的参数（这将在本章后面和附录F中讨论）。`size_type`是一个依赖于实现的整型，是在头文件`string`中定义的。`string`类将**`string::npos`**定义为字符串的最大长度，通常为`unsigned int`的最大值。

另外，表格中使用缩写`NBTS`（`null-terminated string`）来表示以空字符结束的字符串——传统的C字符串。

|                         构 造 函 数                          |                             描述                             |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
|                   `string(const char * s)`                   |            将`string`对象初始化为`s`指向的`NBTS`             |
|                `string(size_type n, char c)`                 | 创建一个包含`n`个元素的`string`对象，其中每个元素都被初始化为字符c |
|                 `string(const string & str)`                 | 将一个`string`对象初始化为`string`对象`str`（复制构造函数）  |
|                          `string()`                          |     创建一个默认的`string`对象，长度为0（默认构造函数）      |
|            `string(const char * s, size_type n)`             | 将`string`对象初始化为s指向的`NBTS`的前`n`个字符，即使超过了`NBTS`结尾 |
|     `template<class Iter> string(Iter begin, Iter end)`      | 将`string`对象初始化为区间[begin, end)内的字符，其中begin和end的行为就像指针，用于指定位置，范围包括begin在内，但不包括end |
| `string(const string & str, size_type pos, size_type n = npos)` | 将一个`string`对象初始化为对象`str`中从位置`pos`开始到结尾的字符，或从位置`pos`开始的`n`个字符 |
|                     C++11新增的构造函数                      |                                                              |
|               `string(string && str) noexcept`               | 这是`C++11`新增的，它将一个`string`对象初始化为`string`对象`str`，并可能修改`str`（移动构造函数） |
|             `string(initializer_list<char> il)`              | 这是C++11新增的，它将一个string对象初始化为初始化列表`il`中的字符 |

`string`重载了`+=`、`=`、`<<`、`[]`等各种运算符 

`string`重载了6种关系运算符，以便能够将`string`对象与另一个`string`对象、C风格字符串进行比较，并能够将C风格字符串与string对象进行比较

```c++
// str1.cpp -- introducing the string class
#include <iostream>
#include <string>
// using string constructors

int main()
{
    using namespace std;
    string one("Lottery Winner!"); // ctor #1
    cout << one << endl;           // overloaded <<
    string two(20, '$');           // ctor #2
    cout << two << endl;
    string three(one);             // ctor #3
    cout << three << endl;
    one += " Oops!";               // overloaded +=
    cout << one << endl;
    two = "Sorry! That was ";
    three[0] = 'P';
    string four;                   // ctor #4
    four = two + three;            // overloaded +, =
    cout << four << endl;
    char alls[] = "All's well that ends well";
    string five(alls,20);          // ctor #5
    cout << five << "!\n";
    string six(alls+6, alls + 10); // ctor #6
    cout << six << ", ";
    string seven(&five[6], &five[10]); // ctor #6 again
    cout << seven << "...\n";
    string eight(four, 7, 16);     // ctor #7
    cout << eight << " in motion!" << endl;
    return 0;
}

Lottery Winner!
$$$$$$$$$$$$$$$$$$$$
Lottery Winner!
Lottery Winner! Oops!
Sorry! That was Pottery Winner!
All's well that ends!
well, well...
That was Pottery in motion!
```

## `C++11`新增的构造函数

- 构造函数`string（string && str）`类似于复制构造函数，导致新创建的`string`为`s`tr的副本。但与复制构造函数不同的是，它不保证将`str`视为`const`。这种构造函数被称为移动构造函数（`move constructor`）。在有些情况下，编译器可使用它而不是复制构造函数，以优化性能。第18章的“移动语义和右值引用”一节将讨论这个主题。
  - **移动语义和右值引用**

- 构造函数`string（initializer_list<char> il`）让您能够将列表初始化语法用于string类
  - 模板`initializer_list`。

```c++
string piano_man = {'L', 'i', 's','z','t'};
string comp_lang {'L', 'i', 's', 'p'};
```

# `string`类的输入

对于C风格字符串，有3种方式：

```c++
char info[100];
cin >> info;             // 读取一个单词，保留空白符
cin.getline(info, 100);  // 读取一行数据，丢弃换行符
cin.get(info, 100);      // 读取一行数据，保留分隔符\n在缓冲区
```

对于string对象，有两种方式：

```c++
string stuff;
cin >> stuff;        // 读取一个单词，保留空白符
getline(cin, stuff); // 读取一行数据，丢弃换行符
```

`getline`的可选参数

```c++
cin.getline(info,100,':'); // 按照:读取数据并丢弃这个分隔符
getline(stuff, ':');       // read up to :, discard :
```

```c++
char fname[10];
string lname;
cin >> fname;            // 如果输入超过9个字符（因为fname只有10个字符，需要留一个给空字符'\0'），则会导致缓冲区溢出，这是不安全的。
cin >> lname;            // string对象可以动态调整大小，可以读取很长的单词，但注意：cin读取字符串时会在空白符（空格、制表符、换行符）处停止。
cin.getline(fname, 10);  // 可能截断输入,但不会溢出;第二个参数 10 指定最大读取字符数（包括结尾的 '\0'）
getline(cin, lname);     // 不进行截断
```

| 方法                     | 类型     | 安全性   | 备注             |
| :----------------------- | :------- | :------- | :--------------- |
| `cin >> fname`           | 字符数组 | ❌ 不安全 | 可能缓冲区溢出   |
| `cin >> lname`           | string   | ✅ 安全   | 自动调整大小     |
| `cin.getline(fname, 10)` | 字符数组 | ✅ 安全   | 可能截断输入     |
| `getline(cin, lname)`    | string   | ✅ 安全   | 不截断，推荐使用 |

string对象的最大允许长度，由常量`string::npos`指定。这通常是最大的`unsigned int`值

string版本的`getline()`函数从输入中读取字符，并将其存储到目标`string`中，直到发生下列三种情况之一：

- 到达文件尾，在这种情况下，输入流的`eofbit`将被设置，这意味着方法`fail()`和`eof()`都将返回`true`；
- 遇到分界字符（默认为\n），在这种情况下，将把分界字符从输入流中删除，但不存储它；
- 读取的字符数达到最大允许值（`string::npos`和可供分配的内存字节数中较小的一个），在这种情况下，将设置输入流的`failbit`，这意味着方法fail()将返回true。

## `size()`和`length()`

都返回字符串中的字符数

`length()`成员来自较早版本的`string`类，而`size()`则是为提供`STL`兼容性而添加的。

# `find()`

重载的find()方法

|                         方 法 原 型                          |                            描　述                            |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| `size_type find(const string & str, size_type pos = 0)const` | 从字符串的`pos`位置开始，查找子字符串`str`。如果找到，则返回该子字符串首次出现时其首字符的索引；否则，返回`string :: npos` |
|   `size_type find(const char * s, size_type pos = 0)const`   | 从字符串的`pos`位置开始，查找子字符串s。如果找到，则返回该子字符串首次出现时其首字符的索引；否则，返回`string :: npos` |
| `size_type find(const char * s, size_type pos, size_type n)` | 从字符串的`pos`位置开始，查找s的前`n`个字符组成的子字符串。如果找到，则返回该子字符串首次出现时其首字符的索引；否则，返回`string :: npos` |
|      `size_type find(char ch, size_type pos = 0)const`       | 从字符串的`pos`位置开始，查找字符`ch`。如果找到，则返回该字符首次出现的位置；否则，返回`string :: npos` |

string库还提供了相关的方法：`rfind()、find_first_of()、find_last_of()、find_first_not_of()和find_last_not_of()`，它们的重载函数特征标都与find()方法相同。

- `rfind()`方法查找子字符串或字符最后一次出现的位置；

- `find_first_of()`方法在字符串中查找参数中任何一个字符首次出现的位置。

  ```c++
  string snake1("cobra");
  int where1 = snake1.find_first_of("hark");
  int where2 = snake1.find_last_of("hark");
  int where3 = snake1.find_first_no_of("hark");
  ```

  查找字符串中第一个与给定参数(字符集合) 中任意一个字符相同(相匹配)的这个字符下标位置

  如：字符串cobra，c没有与参数中任意一个字符相匹配，o同上，b同上，r在参数中能找到，返回字符串中r的下标位置，即3

- `find_last_of()`方法的功能与此相同，只是它查找的是最后一次出现的位置。

  查找字符串中最后一个与给定参数(字符集合) 中任意一个字符相同(相匹配)的这个字符下标位置

  它开始搜索的位置是从字符串末尾开始，向前搜索。

  如：从字符串`snake1`的最后一个字符a开始，a在字符集合中能找到，返回a的下标，即4

- `find_first_not_of()`方法在字符串中查找第一个不包含在参数中的字符

  `where3` 返回c在“cobra”中的位置，因为“hark”中没有c：

# `Hangman`拼字游戏

````c++
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>
using std::string;
const int NUM = 26;
const string wordlist[NUM] = {"apiary", "beetle", "cereal",
    "danger", "ensign", "florid", "garage", "health", "insult",
    "jackal", "keeper", "loaner", "manage", "nonce", "onset",
    "plaid", "quilt", "remote", "stolid", "train", "useful",
    "valid", "whence", "xenon", "yearn", "zippy"};
int main()
{
    using std::cout;
    using std::cin;
    using std::tolower;//大写转小写
    using std::endl;
    std::srand(std::time(0));//用当前时间初始化随机数生成器，确保程序每次运行产生不同的随机序列。
    char play;
    cout<<"是否要开始单词游戏？输入y确认，n退出:";
    cin>>play;
    play = tolower(play);
    while(play == 'y')
    {
        string target = wordlist[std::rand()%NUM];
        int length = target.length();
        string attempt(length,'-');
        string badchars;
        int guesses = length;
        cout<<"这个单词长度为："<<length<<";你需要猜测这个单词由哪些字母组成。\n"
        <<"你有："<<guesses<<"次猜错的机会\n";
        cout<<"-------------"<<endl;
        cout<<"你目前的单词"<<attempt<<endl;
        while(guesses > 0 && attempt != target)
        {
            char letter;
            cout<<"输入你猜的字母:";
            cin>>letter;
            if(badchars.find(letter) != string::npos ||
                attempt.find(letter) != string::npos)
            {
                cout<<"你猜过这个字符了，请换个字母再试试\n";
            }
            int loc = target.find(letter);
            if(loc == string::npos)
            {
                cout<<"Oh,你猜错了！\n";
                --guesses;
                badchars += letter;
            }
            else
            {
                cout<<"你猜对了，确实有这个字母！";
                attempt[loc] = letter;
                loc = target.find(letter,loc+1);
                while(loc != string::npos)
                {
                    attempt[loc] = letter;
                    loc = target.find(letter,loc+1);
                }
            }
            cout<<"你目前的单词："<<attempt<<endl;
            if(attempt != target)
            {
                if(badchars.length()>0)
                {
                    cout<<"目前猜错的字母，它是："<<badchars<<endl;
                }
                cout<<"你目前还有："<<guesses<<"次机会\n";
                cout<<"---------"<<endl;
            }
        }
        if(guesses > 0)
        {
            cout<<"恭喜你，猜对了!"<<endl;
            cout<<"你还要继续吗？\n输入y新的开始，输入n结束游戏:";
            cin>>play;
            play = tolower(play);
        }
        else  
        {
            cout<<"很遗憾，这个单词是："<<target<<".\n";
            cout<<"你还要继续吗？\n输入y新的开始，输入n结束游戏:";
            cin>>play;
            play = tolower(play);
        }
    }
    cout<<"Bye~\n";
    return 0;
}
````

## `tolower()`和`toupper()`

- `tolower`:将大写字母转换为小写字母。如果输入的不是小写字母，则返回该字符本身
- `toupper`:将小写字母转换为大写字母。如何输入的不是大写字母，则返回该字符本身

# `capacity()`和`reserve()`

- `capacity()`返回当前分配给字符串的内存块的大小
- `reserve()`重新请求内存块的最小长度

```c++
#include <iostream>
#include <string>
int main()
{
    using namespace std;
    string empty;
    string small = "bit";
    string larger = "Elephants are a girl's best friend";
    cout << "Sizes:\n";
    cout << "\tempty: " << empty.size() << endl;
    cout << "\tsmall: " << small.size() << endl;
    cout << "\tlarger: " << larger.size() << endl;
    cout << "Capacities:\n";
    cout << "\tempty: " << empty.capacity() << endl;
    cout << "\tsmall: " << small.capacity() << endl;
    cout << "\tlarger: " << larger.capacity() << endl;
    empty.reserve(50);
    cout << "Capacity after empty.reserve(50): "
         << empty.capacity() << endl;
    return 0;
}

Sizes:
        empty: 0
        small: 3
        larger: 34
Capacities:
        empty: 15
        small: 15
        larger: 34
Capacity after empty.reserve(50): 50
Sizes:
        empty: 0
        small: 3
        larger: 34
Capacities:
        empty: 15
        small: 15
        larger: 34
Capacity after empty.reserve(50): 50
```

# `string`对象转为C风格字符串

```c++
string filename;
cout << "Enter file name: ";
cin >> filename;
ofstream fout;
fout.open(filename.c_str());
```

`open()`方法要求使用一个C风格字符串作为参数；

`c_str()`方法返回一个指向C风格字符串的指针，该C风格字符串的内容与用于调用`c_str()`方法的string对象相同

# ==--------------==

# 智能指针模版类

- ## 参看3高级面向对象-12全局算法

- ##### C++ 智能指针 - 全部用法详解：https://blog.csdn.net/cpp_learner/article/details/118912592

智能指针是行为类似于指针的类对象，可以帮助管理动态内存分配

`auto_ptr`、`unique_ptr`和`shared_ptr`以及`weak_ptr`

头文件

```c++
#include <memory>
```

智能指针背后的思想

智能指针作为有析构函数的类对象，当对象过期时，它的析构函数删除指向的内存

- 模版`auto_ptr`在`C++11`后已被抛弃

这三个智能指针模板（`auto_ptr`、`unique_ptr`和`shared_ptr`）都定义了类似指针的对象，可以将new获得（直接或间接）的地址赋给这种对象。当智能指针过期时，其析构函数将使用delete来释放内存。因此，如果将new返回的地址赋给这些对象，将无需记住稍后释放这些内存：在智能指针过期时，这些内存将自动被释放。

![屏幕截图 2025-12-24 113920](D:\Typora\typora_work\C++ Primer Plus\屏幕截图 2025-12-24 113920.png)

所有**智能指针类**都有一个**explicit构造函数**，该构造函数将指针作为参数。

因此，不会自动将指针转换为智能指针对象：

**普通指针**要想成为智能指针对象需要**显式类型转换**

```c++
shared_ptr<double> pd;//创建一个空的shared_ptr，引用计数为0
double *p_reg = new double;//在堆上分配double内存，并返回裸指针p_reg
pd = p_reg; // 禁止隐式类型转换
pd = shared_ptr<double>(p_reg);     // 允许显式类型转换
shared_ptr<double> pshared = p_reg; // 裸指针的隐式构造shared_ptr，不允许
shared_ptr<double> pshared(p_reg);  // 显式构造shared_ptr，允许
```

三种智能指针都应**避免**的一点：**智能指针不能用于非堆内存**(如：栈内存），只用于new堆内存

```c++
string vacation("I wandered lonely as a cloud.");
shared_ptr<string> pvac(&vacation); // NO!
```

`pvac`过期时，程序将把`delete`运算符用于非堆内存，这是错误的。

# C++智能指针详解：函数与使用陷阱

C++ 智能指针 - 全部用法详解：https://blog.csdn.net/cpp_learner/article/details/118912592

## 一、三大智能指针概览

### 1. **unique_ptr**

- 独占所有权，不可复制
- 轻量级，开销小

### 2. **shared_ptr**

- 共享所有权，使用引用计数
- 支持复制和赋值

### 3. **weak_ptr**

- 弱引用，不增加引用计数
- 解决循环引用问题

## 二、核心函数详解

### 各种函数

```c++
class Test {
public:
	Test() { cout << "Test的构造函数..." << endl; }
	~Test() { cout << "Test的析构函数..." << endl; }

	int getDebug() { return this->debug; }

private:
	int debug = 20;
};
int main()
{
   
	
    return 0;
}
```

#### **get**() 获取智能指针托管的指针地址

```c++
    auto_ptr<Test> test(new Test);

    Test *tmp = test.get();		// 获取指针返回
    cout << "tmp->debug：" << tmp->getDebug() << endl;
```

#### 赋值`std::move()`

- **定义赋值运算符**，使之执行深复制。这样两个指针将指向不同的对象，其中的一个对象是另一个对象的副本。

- **对于`auto_ptr`和`unique_ptr`**：建立所有权（`ownership`）概念，对于特定的对象，只能有一个智能指针可拥有它，这样只有拥有对象的智能指针的构造函数会删除该对象。然后，让赋值操作转让所有权。这就是用于`auto_ptr`和`unique_ptr`的策略，但`unique_ptr`的策略更严格。

- **对于`shared_ptr`**：创建智能更高的指针，跟踪引用特定对象的智能指针数。这称为**引用计数（`reference counting`）**。例如，赋值时，计数将加1，而指针过期时，计数将减1。仅当最后一个指针过期时，才调用`delete`。这是`shared_ptr`采用的策略。

```c++
unique_ptr<Test> t7(new Test);
unique_ptr<Test> t8(new Test);
t7 = std::move(t8);	// 必须使用移动语义，结果，t7的内存释放，t8的内存交给t7管理
t7->doSomething();
```

#### 主动放弃对象`reset()`

```c++
unique_ptr<Test> t9(new Test);
t9 = NULL;
t9 = nullptr;
t9.reset();
```

#### 重置、托管新对象 `reset()`重置智能指针托管的内存地址，如果地址不一致，原来的会被析构掉

```c++
test.reset();			// 释放掉智能指针托管的指针内存，并将其置NULL
test.reset(new Test());	// 释放掉智能指针托管的指针内存，并将参数指针取代之
```

reset函数会将参数的指针(不指定则为NULL)，与托管的指针比较，如果地址不一致，那么就会析构掉原来托管的指针，然后使用参数的指针替代之。然后智能指针就会托管参数的那个指针了。

#### 放弃对象的控制权 `release()`取消智能指针对动态内存的托管

```c++
Test *tmp2 = test.release();	// 取消智能指针对动态内存的托管
delete tmp2;	// 之前分配的内存需要自己手动释放
//也就是智能指针不再对该指针进行管理，改由管理员进行管理！
```

### 各种智能指针的函数

#### **unique_ptr 的主要函数**

```cpp
#include <memory>
#include <iostream>

// 1. 构造函数
std::unique_ptr<int> p1(new int(10));
auto p2 = std::make_unique<int>(20);  // C++14推荐方式

// 2. reset() - 重置指针(主动放弃对象)、托管新对象
p1.reset();           // 释放当前对象，置空
p1.reset(new int(30)); // 释放旧对象，管理新对象

// 3. release() - 释放所有权
int* raw_ptr = p2.release();  // p2变为空，需要手动delete raw_ptr
delete raw_ptr;

// 4. get() - 获取原始指针
int* ptr = p1.get();  // 不放弃所有权，只是获取

// 5. swap() - 交换
std::unique_ptr<int> p3 = std::make_unique<int>(40);
p1.swap(p3);

// 6. 数组支持
auto arr = std::make_unique<int[]>(10);  // C++14
```

#### **shared_ptr 的主要函数**

```cpp
#include <memory>
#include <iostream>

// 1. 构造函数
std::shared_ptr<int> sp1(new int(10));
auto sp2 = std::make_shared<int>(20);  // 推荐，更高效

// 2. use_count() - 获取引用计数
std::cout << sp1.use_count() << std::endl;  // 输出当前引用数

// 3. reset()
sp1.reset();           // 减少引用计数，置空sp1
sp1.reset(new int(30)); // 管理新对象
sp1.reset(sp2); //将sp1重置为sp2（的值）,sp1 管控的对象计数减1，sp1接管对sp2指针的管控

// 4. unique() - 是否唯一所有者（C++17已弃用，用use_count()==1代替）
bool isUnique = sp1.use_count() == 1;

// 5. 类型转换
class Base { virtual void func() {} };
class Derived : public Base {};

auto basePtr = std::make_shared<Derived>();
auto derivedPtr = std::static_pointer_cast<Derived>(basePtr);
auto dynamicPtr = std::dynamic_pointer_cast<Derived>(basePtr);

// 6. 自定义删除器
auto custom_sp = std::shared_ptr<int>(
    new int(10),
    [](int* p) {
        std::cout << "Custom deleter called" << std::endl;
        delete p;
    }
);
// 7. 交换
std::swap(sp1,sp2); // 交换p1 和p2 管理的对象，原对象的引用计数不变
sp1.swap(sp2);    // 交换p1 和p2 管理的对象，原对象的引用计数不变
```

#### **weak_ptr 的主要函数**

```cpp
#include <memory>
#include <iostream>

// 1. 构造函数 - 从shared_ptr创建
auto shared = std::make_shared<int>(10);
std::weak_ptr<int> weak(shared);

// 2. lock() - 尝试获取shared_ptr
if (auto locked = weak.lock()) {
    std::cout << "Value: " << *locked << std::endl;
} else {
    std::cout << "Object has been destroyed" << std::endl;
}

// 3. expired() - 检查对象是否已销毁
if (weak.expired()) {
    std::cout << "Weak pointer is expired" << std::endl;
}

// 4. use_count()
std::cout << "Reference count: " << weak.use_count() << std::endl;
```

## 三、常见使用陷阱

### **陷阱1：`shared_ptr`循环引用（最常见）**

```cpp
class Node {
public:
    std::shared_ptr<Node> next;
    std::shared_ptr<Node> prev;
    // 或者更隐蔽的循环引用
    std::shared_ptr<Node> child;
    std::weak_ptr<Node> parent;  // 应该使用weak_ptr
};

// 循环引用示例
auto node1 = std::make_shared<Node>();
auto node2 = std::make_shared<Node>();
node1->next = node2;
node2->prev = node1;  // 循环引用！内存泄漏
```

**解决方案**：使用 `weak_ptr` 打破循环

```cpp
class SafeNode {
public:
    std::shared_ptr<SafeNode> next;
    std::weak_ptr<SafeNode> prev;  // 使用weak_ptr
};
```

### **陷阱2：多个智能指针管理同一个原始指针**

不要把一个原生指针给多个智能指针管理;

```cpp
int* raw = new int(10);
std::shared_ptr<int> sp1(raw);
std::shared_ptr<int> sp2(raw);  // 错误！以上代码使sp1和sp2指向同一个内存，会有两个独立的控制块，释放时导致双重释放。
```

**解决方案**：

1. **每个原始指针只被一个智能指针管理**
2. 使用 `make_shared` 或 `make_unique` 避免直接使用 `new`

```cpp
//正确做法1
int* raw = new int(10);
std::shared_ptr<int> sp1(raw);
std::shared_ptr<int> sp2 = sp1;  // 正确：共享控制块

//正确做法2
auto sp1 = std::make_shared<int>(10);  // 避免显式new
auto sp2 = sp1;  // 正确：共享控制块
```

### **陷阱3：get() 函数误用**

**`shared_ptr` 之间只能通过拷贝/赋值来共享所有权，不能通过原始指针来"连接"它们。**

```cpp
auto sp = std::make_shared<int>(10);  // 创建控制块A，引用计数=1
int* raw = sp.get();                   // 获取原始指针，没有影响控制块A

{
    // 错误：使用原始指针构造新的shared_ptr
    std::shared_ptr<int> sp2(raw);    // 创建新的控制块B，引用计数=1
    // 现在有两个独立的控制块管理同一块内存！
}
// sp2析构时：控制块B引用计数减为0 → 释放内存 ❌
// sp析构时：控制块A引用计数减为0 → 再次释放同一块内存（双重释放）❌❌

// 这里使用sp会导致未定义行为
```

### **陷阱4：在函数实参中创建智能指针**

```cpp
void process(std::shared_ptr<int> ptr, int value) {
    // ...
}

int computeValue() {
    // 可能抛出异常
    throw std::runtime_error("error");
    return 42;
}

// 危险的调用方式
process(std::shared_ptr<int>(new int(10)), computeValue());
// 编译器可能重排执行顺序：new int -> computeValue() -> shared_ptr构造函数
// 如果computeValue()抛出异常，内存泄漏
```

**解决方案**：使用 `make_shared`

```cpp
process(std::make_shared<int>(10), computeValue());  // 安全
```

### **陷阱5：shared_ptr 数组管理**

```cpp
// 错误
std::shared_ptr<int[]> arr(new int[10]);  // 默认删除器是delete，不是delete[]

// 正确：提供自定义删除器
std::shared_ptr<int> arr(new int[10], [](int* p) { delete[] p; });

// 或者使用unique_ptr（C++11起支持数组）
std::unique_ptr<int[]> arr2(new int[10]);  // 自动使用delete[]
// C++14后更好
auto arr3 = std::make_unique<int[]>(10);
```

### **陷阱6：误用 unique_ptr**

```cpp
auto p1 = std::make_unique<int>(10);
auto p2 = p1;  // 错误！unique_ptr不可复制

// 正确：移动语义
auto p3 = std::move(p1);  // p1变为nullptr

// 常见陷阱：函数返回
std::unique_ptr<int> create() {
    auto p = std::make_unique<int>(10);
    return p;  // 正确！这里发生移动构造
}
```

### ==`auto_ptr` 与 `unique_ptr`智能指针的内存管理陷阱==

```c++
auto_ptr<string> p1;
string *str = new string("智能指针的内存管理陷阱");
p1.reset(str);	// p1托管str指针
{
	auto_ptr<string> p2;
	p2.reset(str);	// p2接管str指针时，会先取消p1的托管，然后再对str的托管
}

// 此时p1已经没有托管内容指针了，为NULL，在使用它就会内存报错！
cout << "str：" << *p1 << endl;
```

### **陷阱7：性能考虑**

```cpp
// make_shared vs shared_ptr构造函数
auto sp1 = std::make_shared<int>(10);  // 一次内存分配
std::shared_ptr<int> sp2(new int(10)); // 两次内存分配

// 但make_shared有缺点：内存不会立即释放
class LargeObject {
    char data[1024 * 1024];  // 1MB
};

auto sp = std::make_shared<LargeObject>();
std::weak_ptr<LargeObject> weak = sp;
sp.reset();  // 对象析构，但内存块（包含控制块）不会释放
// 只要还有weak_ptr存在，整个内存块就不会释放
```

### **陷阱8：线程安全问题**

```cpp
// shared_ptr的引用计数是原子的，但指向的对象不是线程安全的
std::shared_ptr<int> global_ptr;

void thread_func() {
    // 多个线程同时读写global_ptr指向的内容需要同步
    // 使用mutex保护
}
```

## 四、最佳实践总结

1. **优先使用 `make_shared` 和 `make_unique`**
   - 避免显式使用 `new` 和 `delete`
   - 提供异常安全

2. **正确使用所有权语义**
   - 单一所有权用 `unique_ptr`
   - 共享所有权用 `shared_ptr`
   - 观察用 `weak_ptr`

3. **避免循环引用**
   - 仔细设计对象关系
   - 使用 `weak_ptr` 打破循环

4. **不要混用智能指针和原始指针**
   - 如果必须使用 `get()`，确保原始指针不会在智能指针生命周期之外使用

5. **小心处理数组**
   - `unique_ptr` 支持数组（`T[]`）
   - `shared_ptr` 需要自定义删除器

6. **了解性能开销**
   - `unique_ptr` 几乎无开销
   - `shared_ptr` 有引用计数开销
   - `weak_ptr` 有额外控制块开销

记住：智能指针是为了简化内存管理，但需要正确理解其语义和限制才能安全使用。

