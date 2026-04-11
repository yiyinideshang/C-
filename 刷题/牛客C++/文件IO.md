 [第17章输入输出和文件.md](..\..\C++ Primer Plus\第17章输入输出和文件.md) 

# 三种字符串输入

| 特性           | `std::cin.get()` | `std::cin >>`                                                | `std::getline()` |
| :------------- | :--------------- | :----------------------------------------------------------- | :--------------- |
| **空白符处理** | 读取空白符       | 跳过前导空白符(空格、制表符、换行符)                         | 读取直到换行     |
| **分隔符**     | 不提取分隔符     | 以空白符分隔                                                 | 提取并丢弃分隔符 |
| **缓冲区**     | 保留分隔符       | **不保留前导分隔符**，**不提取尾随(如末尾的Enter`\n`)空白符** | 不保留分隔符     |
| **使用场景**   | 精细控制         | 简单输入                                                     | 读取整行         |

# `scanf` 和 `printf`

若有说明语句：int a；float b；以下输入语句正确的是（**C**）

`A` 	scanf("%f%f",&a,&b)；

`B`	scanf("%f%d",&a,&b)；

`C`	scanf("%d,%f",&a,&b)；

`D`	scanf("%6.2f%6.2f",&a,&b)；

在C语言的`scanf()`函数中，**`%6.2f` 是无效的格式说明符**。

- **`printf()`** 中：`%6.2f` 是有效的
  - `6`：最小字段宽度（总宽度至少6个字符）
  - `.2`：精度（小数点后保留2位）
- **`scanf()`** 中：**不能指定精度**，只能指定最大字段宽度,不要添加精度说明符
- 格式化输出应该在`printf()`中完成。

## `scanf()` 中正确的浮点数格式

**`%f`** - 读取一个浮点数

**`%lf`** - 读取一个双精度浮点数（用于`double`类型）

`%nf` -指定最大宽度

```c
float num;
scanf("%5f", &num);  // 最多读取5个字符
输入 123.456 → 只读取 123.4（5个字符）
```

# `iostream`

在下列选项中，istream类对象的是（）

`A`	`cerr` —— 类型 `ostream`（标准错误，无缓冲）

`B`	`cin` —— 类型 `istream`（标准输入）

`C`	`clog`  —— 类型 `ostream`（标准错误，有缓冲）

`D`	`cout` —— 类型 `ostream`（标准输出）

# `cstring`

`<cstring>` 是 C++ 标准库中的头文件，它包含了 C 语言标准库 `<string.h>` 的函数，但将这些函数放在 `std` 命名空间中。它主要用于处理 **C 风格字符串**（以 `\0` 结尾的字符数组）。

## `strlen` - 获取字符串长度

表达式 `strlen("std\n007\1\\")` 的值是 :

表达式 `strlen("std\n007\1\\")` 用于计算给定字符串的长度，**不包括终止空字符 `\0`。**字符串中的字符解析如下：

1. `s`、`t`、`d`：三个普通字符。
2. `\n`：换行转义序列，计为1个字符。
3. `0`、`0`、`7`：三个普通数字字符。
4. `\1`：八进制转义序列（ASCII值为1），计为1个字符。
5. `\\`：反斜杠转义序列，计为1个字符。

总计字符数为：3 + 1 + 3 + 1 + 1 = **9**。

#### `strcmp` / `strncmp` - 字符串比较

#### `strcpy` / `strncpy` - 字符串复制

#### `strcat` / `strncat` - 字符串连接

#### `strchr` / `strrchr` - 查找字符

#### `strstr` - 查找子串

#### `strtok` - 字符串分割

# 文件指针

**文件指针**是C/C++中用于文件操作的一个关键概念，它是一个指向`FILE`结构体的指针，用于跟踪和管理打开文件的状态信息。

```c++
FILE *fp;  // 声明一个文件指针
```

 **FILE结构体**

每个打开的文件都有一个对应的`FILE`结构体，包含以下信息：

- **文件描述符**（文件在操作系统中的标识）
- **当前读写位置**（文件位置指针）
- **缓冲区信息**
- **文件状态标志**（读/写/追加模式、错误标志、EOF标志等）
- **缓冲区指针和大小**

```c++
// FILE结构体的大致定义（实际实现可能不同）
struct _iobuf {
    char *_ptr;      // 下一个要读取/写入的位置
    int   _cnt;      // 缓冲区中剩余字符数
    char *_base;     // 缓冲区基地址
    int   _flag;     // 文件状态标志
    int   _file;     // 文件描述符
    int   _charbuf;  // 字符缓冲区
    int   _bufsiz;   // 缓冲区大小
    char *_tmpfname; // 临时文件名
};
typedef struct _iobuf FILE;
```

| 特性         | 说明                                                         |
| :----------- | :----------------------------------------------------------- |
| **本质**     | 指向`FILE`结构体的指针                                       |
| **作用**     | 管理文件状态、位置、缓冲区等                                 |
| **声明**     | `FILE *指针名;`                                              |
| **获取**     | 通过`fopen()`函数                                            |
| **释放**     | 通过`fclose()`函数                                           |
| **标准指针** | `stdin`, `stdout`, `stderr`                                  |
| **相关函数** | `fopen()`, `fclose()`, `fread()`, `fwrite()`, `fseek()`, `ftell()`等 |

下面选项中关于 " 文件指针 " 概念的叙述正确的是（）

A	文件指针是 `FILE` 指针类型的变量

B	文件指针就是文件位置指针，表示当前读写数据的位置

C	文件指针指向文件在计算机中的存储位置

D	把文件指针传给 `fscanf` 函数，就可以向文本文件中写入任意的字符

- **A 正确**：文件指针在 C 语言中定义为 `FILE *` 类型的变量，它指向一个 `FILE` 结构体，该结构体包含了文件操作所需的各种信息（如缓冲区、文件状态、当前位置等）。
- **B 错误**：文件指针与文件位置指针是两个不同的概念。文件指针指向 `FILE` 结构体，而文件位置指针（通常通过 `ftell` 获取）表示文件内的当前读写位置偏移量。
- **C 错误**：文件指针并不直接指向文件在磁盘上的物理存储位置，而是指向管理文件 I/O 的 `FILE` 结构体。文件的物理存储由操作系统管理，不直接暴露给用户程序。
- **D 错误**：`fscanf` 函数用于从文件中读取格式化数据，而非写入。向文本文件写入字符应使用 `fprintf`、`fputc` 或 `fputs` 等函数。



重复多次 `fclose` 一个打开过一次的 `FILE *fp` 指针会有什么结果？

A	导致文件描述符结构中指针指向的内存被重复释放，进而导致一些不可预期的异常

B	不会出现异常，释放一个已经释放的指针，系统会自动忽略

C	无法编译通过

D	以上答案都不正确

- 第一次 `fclose(fp)` 会关闭文件，并释放与该文件流相关的系统资源（如文件描述符、缓冲区内存等）。
- 第二次及后续的 `fclose(fp)` 相当于对无效的 `FILE` 指针进行操作，可能导致：
  - 程序崩溃（例如 double-free 内存错误）
  - 数据损坏
  - 或其他不可预测的后果

**因此：**

- **A** ✅ 描述正确，重复释放会导致未定义异常（典型表现如内存重复释放）。
- **B** ❌ 错误，系统不会自动忽略，而是未定义行为，常见实现（如 glibc）会报错或崩溃。
- **C** ❌ 错误，代码可以编译通过，因为这是运行时问题。
- **D** ❌ 错误

# `ftell`

- `ftell()` 用于获取当前文件指针的位置（偏移量），不移动指针。

# `fseek`

- `fseek()` 可以将文件指针移动到指定位置，虽然能实现回到文件首地址（如 `fseek(fp, 0, SEEK_SET)`），但 `rewind()` 是更直接的方式。

# `ferror`

- `ferror()` 用于检查文件流是否发生了错误，不涉及指针移动。

# `rewind`

- **`rewind()`** 函数的作用正是将文件指针重新定位到文件的开始位置，相当于 `fseek(fp, 0, SEEK_SET)`，同时还会清除文件的错误标志和结束标志。

在C++程序开发中，也会遇到很多文件上传，文件写入等对于文件的操作业务需要开发，文件处理也是任何应用程序的重要组成部分。C++有几种创建，读取，更新和删除文件的方法。本文主要介绍C++ File文件操作常用的函数方法。

# C++文件操作

## 关于输入和输出

- C++ 输入输出始终是相对于程序(内存)而言的

### 输出（Output）- 写入 = 程序 → 外部设备

- **含义**：程序将数据**写**到外部设备（如文件、屏幕、网络等）
- **方向**：从程序**输出**到外部 （程序推数据出去）
- **操作**：写入、发送
- **示例**：

````c++
ofstream outFile("data.txt");  // 创建输出文件流
outFile << "Hello";            // 程序向文件输出数据，也就是写入数据到文件中

cout << "Hello";               // 程序向屏幕输出数据
````

### 输入（Input）- 读取 = 外部设备 → 程序

- **含义**：程序从外部设备**读**取数据
- **方向**：从外部**输入**到程序 （程序拉数据进来）
- **操作**：读取、接收
- **示例**：

```c++
ifstream inFile("data.txt");  // 创建输入文件流
int data;
inFile >> data;               // 程序从文件输入数据，也就是读取文件的内容到程序中

cin >> data;                  // 程序从键盘输入数据
```

1. **C++流运算符**：
   - `<<` 输出运算符：数据流向箭头方向（`cout << data`：数据流向`cout`）
   - `>>` 输入运算符：数据流向箭头方向（`cin >> data`：数据流向变量）

## **1、常用的文件打开方式(打开模式标记)**

| `模式标记`                            | `适用对象`                   | `作用`                                                       |
| ------------------------------------- | ---------------------------- | ------------------------------------------------------------ |
| `ios::in`                             | `ifstream fstream`           | 打开文件用于**读取**数据。如果文件不存在，则打开出错。       |
| `ios::out`                            | `ofstream stream`            | 打开文件用于**写入**数据。如果文件不存在，则新建该文件；如果文件原来就存在，则打开时清除原来的内容。 |
| `ios::app`                            | `ofstream`  `fstream`        | 打开文件，用于在其尾部添加数据。如果文件不存在，则新建该文件。 |
| `ios::ate`                            | `ifstream`                   | 打开一个已有的文件，并将文件读指针指向文件末尾（读写指 的概念后面解释）。如果文件不存在，则打开出错。 |
| `ios:: trunc`                         | `ofstream`                   | 打开文件时会清空内部存储的所有数据，单独使用时与 `ios::out` 相同。 |
| `ios::binary`                         | ``ifstream ofstream fstream` | 以二进制方式打开文件。若不指定此模式，则以文本模式打开。     |
| `ios::in` | `ios::out`                | `fstream`                    | 打开已存在的文件，既可读取其内容，也可向其写入数据。文件刚打开时，原有内容保持不变。如果文件不存在，则打开出错。 |
| `ios::in` | `ios::out`                | `ofstream`                   | 打开已存在的文件，可以向其写入数据。文件刚打开时，原有内容保持不变。如果文件不存在，则打开出错。 |
| `ios::in` | `ios::out` | `ios::trunc` | `fstream`                    | 打开文件，既可读取其内容，也可向其写入数据。如果文件本来就存在，则打开时清除原来的内容；如果文件不存在，则新建该文件。 |

注意：可以用“或”把以上属性连接起来，如，`ios::out|ios::binary` 。文件的打开模式标记代表了文件的使用方式，这些标记可以单独使用，也可以组合使用。

## 2、C++ 文件支持文件的输入输出的类

1）`ofstream`：写操作，输出文件类。

2）`ifstream`：读操作，输入文件类

3）`fstream`：可同时读写的文件类。



## 3、文件打开与关闭相关函数方法

**1）文件打开函数**

`ofstream`、`ifstream`、`fstream`都有`open` 成员函数：

`void open(const char* szFileName, int mode)`

`szFileName`参数是指向文件名的指针，`mode`参数是文件的打开模式标记。

例如，

```text
#include <iostream>
#include <fstream>
using namespace std;
int main()
{
    ifstream inFile;
    inFile.open("c:\\tmp\\test.txt", ios::in);
    if (inFile)  //true说明文件打开成功
        inFile.close();
    else
        cout << "test.txt doesn't exist" << endl;
    ofstream oFile;
    oFile.open("test1.txt", ios::out);
    if (!oFile)  //true说明文件打开出错
        cout << "error 1" << endl;
    else
        oFile.close();
    oFile.open("tmp\\test2.txt", ios::out | ios::in);
    if (oFile)  //true说明文件打开成功
        oFile.close();
    else
        cout << "error 2" << endl;
    fstream ioFile;
    ioFile.open("..\\test3.txt", ios::out | ios::in | ios::trunc);
    if (!ioFile)
        cout << "error 3" << endl;
    else
        ioFile.close();
    return 0;
}
```

**2）文件关闭函数**

调用 `open()` 方法打开文件，是文件流对象和文件之间建立关联的过程。调用 `close()` 方法关闭已打开的文件，该文件流并会被销毁，其后续还可用于关联其它的文件。

语法：

void close( )

例如，

```text
#include <fstream>
using namespace std;
int main()
{
    const char *url="www.cjavapy.com";
    ofstream outFile("site.txt", ios::out);
    //向 url.txt 文件中写入字符串
    outFile.write(url, 30);
    //关闭已打开的文件
    outFile.close();
    return 0;
}
```

**3） ofstream::is_open**

`bool is_open()`：文件打开返回 true ，否则 false

## 4、文件读取函数方法

C++输入文件流`ifstream`的继承关系：

`ios_base <- ios <- istream <- ifstream`

**1）`istream::get()`**



| `single character (1)` | `int get();istream& get (char& c);`                          |
| ---------------------- | ------------------------------------------------------------ |
| `c-string (2)`         | `istream& get (char* s, streamsize n);istream& get (char* s, streamsize n, char delim);` |
| `stream buffer (3)`    | `istream& get (streambuf& sb);istream& get (streambuf& sb, char delim);` |

istream& get (char& c)：从`cin`中输出一个字符，如`cin.get(c)`。

istream& get (char* s, streamsize n)：从`cin`或者`istream`中提取c风格的字符串，最大字符串长度为`n`，最后一个字符为`‘\0’`字符串结束字符。

例如：

```text
 char c[20];
 cin.get(c,20);
```

istream& get (char* s, streamsize n, char delim)：`delim`为定界字符(delimiting characters),其默认值应该是`'\n'`(换行字符),如果人为定义，可以是任意定义的字符，

例如，

```text
 cin.get(c,20,'a');//当遇到a字符时，只取a字符前的字符串部分输出给c字符串，此时，a字符并不从cin输入缓冲区提取，而是后来的变量提取，而getline则直接丢弃该字符;
```

`istream& ignore (streamsize n = 1, int delim = EOF)`：丢弃多`n`个字符，或者遇到`dlim`为止。

**2）istream::getline**

`istream& getline (char* s, streamsize n )`：读取一行到字符数组。默认`delim`是换行字符`'\n'`，遇到后丢弃，第二次读取从`delim`后开始读。

`istream& getline (char* s, streamsize n, char delim )`：自己定义停止符`delim`

3）std::getline (string)

```text
//用户定义截止字符
istream& getline (istream&  is, string& str, char delim);
istream& getline (istream&& is, string& str, char delim); //c++11 标准
//截止字符默认'\n'
istream& getline (istream&  is, string& str);
istream& getline (istream&& is, string& str); // c++11 标准
```

从流对象is中读取一行存到字符串str 直到遇到截止字符，如果遇到截止字符，则把它从流中取出来，然后丢弃（它不被存储，下一个操作的起点在它之后）函数调用前str 中的内容将被覆盖。

例如，

```text
#include<iostream>
#include<fstream>
#include<string>
using namespace std;
int main()
{
    string str;
    ifstream ifs("test.txt");
    if(!ifs){
        cout<<"open file fail!"<<endl;
        return 1;
    }
    while( getline(ifs,str))
    {
        cout<<str<<endl;
    }
    return 0;
}
```

**4）istream::read**

istream& read (char* s, streamsize n)：从输入流中提取n个字符，并把他们存数组`s`中，不检测内容，也不加字符串结尾符号`‘\0’`。

例如，

```text
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
int main () {
  std::ifstream is ("test.txt", std::ifstream::binary);
  if (is) {
    // get length of file:
    is.seekg (0, is.end);
    int length = is.tellg();
    is.seekg (0, is.beg);
    char * buffer = new char [length];
    std::cout << "Reading " << length << " characters... ";
    // read data as a block:
    is.read (buffer,length);
    if (is)
      std::cout << "all characters read successfully.";
    else
      std::cout << "error: only " << is.gcount() << " could be read";
    is.close();
    // ...buffer contains the entire file...
    delete[] buffer;
  }
  return 0;
}
```

**5）istream::putback**

istream& putback (char c)：从输入流读取一个字符，再把它返回。

例如，

```text
char c = std::cin.get(); 
std::cin.putback (c);
```

**6）istream::unget**

istream& unget()：返回最后一次读取的字符到输入流，类似`putback()`。

例如，

```text
char c = std::cin.get();
std::cin.unget();
```

## **5、文件写入函数方法**

C++输出文件流`ofstream`的继承关系：

ios_base <- ios <- ostream <- ofstream

**1）std::ostream::operator<<**

用法和 `cout <<` 相同 ，将数据写入到文件最方便的函数，重载了常用的数据类型。

`arithmetic types (1)`
`ostream& operator<< (bool val);`
`ostream& operator<< (short val);`
`ostream& operator<< (unsigned short val);`
`ostream& operator<< (int val);`
`ostream& operator<< (unsigned int val);`
`ostream& operator<< (long val);`
`ostream& operator<< (unsigned long val);`
`ostream& operator<< (long long val);`
`ostream& operator<< (unsigned long long val);`
`ostream& operator<< (float val);`
`ostream& operator<< (double val);`
`ostream& operator<< (long double val);`
`ostream& operator<< (void* val);`
`stream buffers (2)`
`ostream& operator<< (streambuf* sb );`
`manipulators (3)`
`ostream& operator<< (ostream& (*pf)(ostream&));
ostream& operator<< (ios& (*pf)(ios&));`
`ostream& operator<< (ios_base& (*pf)(ios_base&));`

**2）ostream::put**

ostream& put (char c)：插入字符 `c` 到流中。

**3）ostream::write**

ostream& write (const char* s, streamsize n)：从数组`s`中取`n`个字符插入到流中。

例如，

```text
#include <iostream>
#include <fstream>
using namespace std;
int main(){
  //1.ofstream写入out.txt
  const char * filename = "test.txt";
  string end= "123456";
  ofstream out("out.txt");
  if(out.is_open())
  {
    /*
      "<<":插入器，向流输出数据.
      ">>":析取器，向流输出数据.
     */
    out<<"Hello World." << filename  << " "<< end << endl;
    out.close();
  }
  //2.读取out.txt
  ifstream in("out.txt");
  char buffer[200];
  if(in.is_open())
  {
    while(!in.eof())
    {
      in.getline(buffer, 100);
      cout << buffer << endl;
      out.close();
    }
  }
    return 0;
}
```



## 6、其它函数

1）`istream::ignore`

`istream& ignore (streamsize n = 1, int delim = EOF)`：从输入流中读取`n`个字符并且丢弃，或者读取到`delim`字符再停止读取。

2）`istream::peek`

int peek()：返回输入流下一个字符，并把它留在输入流中，作为下一次读取的起点。返回值是整形ascll码值，可以用 `char(c)`转化为字符。

**3）`istream::tellg`**

streampos tellg()：读取输入流中文件指针的位置，返回值可转化为 `int`。

```text
is.seekg (0, is.end);
int length = is.tellg();
is.seekg (0, is.beg)
```

**4）istream::seekg**

`istream& seekg (streampos pos)，istream& seekg (streamoff off, ios_base::seekdir way)：参数 pos 是流中的绝对位置可以转化为 int参数 off 是偏移量，与way相关，类型是 int 参数 way 可以选下表中的任意一个常量。`

**5）ios::rdstate**

iostate rdstate()：返回当前流中的内部错误状态，`iostate`二进制数，需要做位运算来获取其相应位置上的值。这个函数的功能可以被 `good()`，`eof()`，`fail()`，`bad()` 替换。

例如，

```text
#include<iostream>
#include<fstream>
#include<string>
using namespace std;
int main () {
  std::ifstream is;
  is.open ("test.txt");
  if ( (is.rdstate() & std::ifstream::failbit ) != 0 )
    std::cerr << "Error opening 'test.txt'\n";
  return 0;
}
```

**6）ios::good**

`bool good() const;`
`bool eof() const;`
`bool fail() const;`
`bool bad() const;`

检测流的状态是否正常。当错误的状态flags (eofbit, failbit 和 badbit) 都没被设置的时候返回`true`

特定的错误状态可以用下面的函数（`eof`, `fail`, 和 `bad`）来检测。

| `iostate value` | `indicates`                              | `good()` | `eof()` | `fail()` | `bad()` | `rdstate()` |
| --------------- | ---------------------------------------- | -------- | ------- | -------- | ------- | ----------- |
| `goodbit`       | `No errors (zero value iostate)`         | `true`   | `false` | `false`  | `false` | `goodbit`   |
| `eofbit`        | `End-of-File reached on input operation` | `false`  | `true`  | `false`  | `false` | `eofbit`    |
| `failbit`       | `Logical error on i/o operation`         | `false`  | `false` | `true`   | `false` | `failbit`   |
| `badbit`        | `Read/writing error on i/o operation`    | `false`  | `false` | `true`   | `true`  | `badbit`    |

**7）ios::operator!**

有错误状态返回 true

例如，

```text
#include<iostream>
#include<fstream>
#include<string>
using namespace std;
int main () {
  std::ifstream is;
  is.open ("test.txt");
  if (!is)
    std::cerr << "Error opening 'test.txt'\n";
  return 0;
}
```

**8）ios::operator bool**

布尔运算： 当流对象单独出现在条件语句中时，就间接调用布尔运算。

如：`if(ios)`, `while(ios)`。

c++98: `operator void*() const;`

c++11: `explicit operator bool() const;`

布尔运算一个很方便的用法就是检测文件结束。读到文件末尾的时候， eofbit, failbit 同时被设置为1，所以可以使用`bool()`来判断流的状态。

当文件打开失败的时候failbit 位被设置为1，所以也能检测打开是否成功。

例如，

```text
#include<iostream>
#include<fstream>
#include<string>
using namespace std;
void print_state (const std::ios& stream) 
{
    cout << "good()=" << stream.good();
    cout << " eof()=" << stream.eof();
    cout << " fail()=" << stream.fail();
    cout << " bad()=" << stream.bad()<<endl;
}
int main()
{
    string str;
    ifstream ifs("test.txt");
    if(ifs)
    {
        //while( bool(getline(ifs,str)))// 等价
        //while( getline(ifs,str).good())//等价
        while( getline(ifs,str))
        {
            cout<<"line:"<<str<<endl;
        }
    }
    else{
        cout<<"open file fail!"<<endl;
        return 1;
    }
    print_state(ifs);
    return 0;
}
```



对于下面代码片段

```c++
myFile.open("Sales.dat", ios::app);
```

`则下面说法正确的是（）`

A	该文件只能用于输出

B	该文件只能用于输入

C	该文件既可以用于输出，也可以用于输入

D	若该文件存在，则清除该文件的内容

- **选项A正确**：只使用`ios::app`时，文件只能用于输出。
- **选项B错误**：该模式不支持输入。
- **选项C错误**：未指定`ios::in`模式，无法用于输入。
- **选项D错误**：`ios::app`模式不会清除文件内容，而是保留原有内容并在末尾追加新数据。

# 标准IO

## `fopen`

若执行fopen函数时发生错误，则函数的返回值是（）

A	地址值

B	0

C	1

D	`EOF`

`fopen` 函数在发生错误时会返回一个空指针（NULL），而空指针在C语言中通常表示为地址值 `0`（或宏 `NULL`）。因此，正确答案是 **B 0**。



下哪个是带行缓冲的IO

A	`write(STDOUT_FILENO, &quot;helloworld&quot;, 10);`

B	`fprintf(stderr, &quot;helloworld&quot;);`

C	`fwrite(&quot;helloworld&quot;, 10, 1, stdout);`

D	`fo = fopen(&quot;a.txt&quot;, &quot;w&quot;); fwrite(&quot;helloworld&quot;, 10, 1, fo);`

C标准库中的流（FILE*）有三种缓冲模式：

- **无缓冲（unbuffered）**：立即输出（如 `stderr`）。
- **行缓冲（line buffered）**：遇到换行符或缓冲区满时输出（如连接到终端的 `stdout`）。
- **全缓冲（fully buffered）**：缓冲区满或主动刷新时输出（如普通文件流）。

- ## **各选项分析：**

- **A**：`write(STDOUT_FILENO, "helloworld", 10);`
  使用系统调用 `write`，绕过标准库缓冲，属于无缓冲IO。
- **B**：`fprintf(stderr, "helloworld");`
  `stderr` 默认无缓冲，错误信息立即输出。
- **C**：`fwrite("helloworld", 10, 1, stdout);`
  `stdout` 在连接到终端时默认行缓冲（除非被重定向），因此符合要求。
- **D**：`fo = fopen("a.txt", "w"); fwrite("helloworld", 10, 1, fo);`
  打开普通文件，默认全缓冲。

因此，只有 **C** 使用的是行缓冲的IO流（`stdout`）。
