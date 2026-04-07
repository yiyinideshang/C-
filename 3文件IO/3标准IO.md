# 标准IO

为什么要使用**标准IO**？

我们是知道**系统IO**是挺好用。因为在不同操作系统下面，对文件一些管理和 `API` 接口都是不一样的。

- ### **`API`:应用程序编程接口（Application Programming Interface）。**

Linux： `open read write close...`

- `APP->struct file->struct inode->物理层inode->文件内容`

![屏幕截图 2025-09-27 223052](D:\Typora\typora_work\3文件IO\屏幕截图 2025-09-27 223052.png)

`Window： winopen、winread...`

同一个文件，在不同的操作系统下面， 操作文件的代码其实不一样的。

C/C++几乎支持所有的操作系统、使用C/C++操作文件（系统IO）可移植性**非常差**。

C/C++语言标准委员会，认为自己有责任搞一波操作，来一统江湖。指定/统一文件操作的接口====> **标准IO**

# 标准IO：IO流(文件)

主要就是 统一了文件以及常用的接口函数

**文件**：普通文件、二进制文件（**不包含设备文件**）

- **文本文件**：无组织、无格式的文件、以字符的ASCII来解析文件。
  - `.cpp、.c、.txt、.cs、.html、.java、.py...`
- **二进制文件**：有特定格式的文件
  - 需要按照特定的文件格式解析
  - 某一些字节可能代表特殊含义
  - `.exe、.out、.apk、.jpg、.mp4、.doc、.xls...`

- ### 在标准库中，用结构体`FILE`描述一个文件，然后在这个结构体中创建两个缓冲区：

- **读缓冲区** `*in`
- **写缓冲区** `*out`

```c++
FILE
{
    char *in; // 指向读的缓冲区
    char *out; // 指向写的缓冲区
	...
};
```

对于文件操作的函数接口：

- `fopen,fclose,fread,fwrite,fseek...`
- `puts,fputs`
- `scanf,...`
- `printf,...`

# APP通过标准IO操作文件流程：

- code(代码)-->标准IO库-->对应应用平台的**操作系统IO**-->操作系统内核-->Hardware(硬件)

`*in 读缓冲区` （输出）										`*out 写缓冲区`（输入）

**标准IO是带缓冲的IO，IO流（stream）它的效率比系统IO高原因：**

- **系统IO**：读一个字节，从硬盘上去取一个下来
- **标准IO**：读一个字节，从硬盘上去取一块内容下来，放到标准IO的缓冲区



系统IO：

- 用户态-》内核态

- **还原现场（还原上下文）：在操作完成后恢复原来的执行环境**

- **保护现场（记录上下文）：在操作完成后恢复原来的执行环境**

# -------------------

# 缓冲：同步问题

缓冲区的数据，什么时候才会同步外设（硬件）上面？

## 	标准IO缓冲区的三种类型：

## 行缓冲：

- 缓冲区数据只要达到一行了就会同步到外设（硬盘）上面。假设一行最多80字节，当一行 满了80字节或者遇到 `\n` 的时候就会把缓冲区的数据同步到外设上。
  - printf :行缓冲

  - 当一行数据满了就同步
  - 一行没满，但是遇到换行符也同步
  - 当一行没满，也没有换行符，程序在正常退出的时候也会同步

## 全缓冲

- 只要当缓冲区的数据满了之后，才会同步到外设上面。

## 无缓冲

只要缓冲区有一个字节，就会同步到外设上。标准出错情况`stderr`是典型代表

## **缓冲区**到底有多大？

- 默认全缓冲的大小为4096
-  默认行缓冲是1024 
- 无缓冲的大小为1

# 缓冲区buffer

**缓冲区是$\textcolor{Red}{内存空间的一部分}$。也就是说，在内存空间中预留了一定的存储空间，这些存储空间用来$\textcolor{Red}{缓冲输入或输出的数据}$，这部分预留的空间就叫做缓冲区。**缓冲区根据其对应的是输入设备还是输出设备，分为$\textcolor{red}{输入缓冲区和输出缓冲区}$。

在编程中，缓冲区通常指的是**计算机的随机存取存储器（RAM）** 中划定的一块区域。

- **位置**：物理上在内存条（RAM）里；逻辑上在程序的地址空间中。
- **作用**：
  - **输入缓冲区**：当你从键盘打字时，字符会先存放在一个输入缓冲区里，等你按下回车键后，程序才一次性读取所有字符。
  - **输出缓冲区**：程序要打印内容到屏幕时，数据会先送到输出缓冲区，等到缓冲区满了或遇到换行符时，才一次性显示出来，这样可以提高效率。
  - **视频播放**：在线看视频时，播放器会预先下载一段数据到缓冲区，这样即使网络短暂波动，视频也能流畅播放，而不会卡顿。



在 `linux` 下缓冲区我们可以自己进行设置： `setbuf` 、 `setvbuf`

```c++
int setvbuf(FILE *stream, char *buf, int mode, size_t size);
/*
    @描述：
    	更改缓冲区策略
    @stream：
    	需要更改的输入/输出流
    @buf：
    	缓冲区指针
    @mode：
   	 	缓冲区类型
        _IONBF unbuffered 无缓冲
        _IOLBF line buffered 行缓冲
        _IOFBF fully buffered 全缓冲
    @size：
    	缓冲区大小
*/
```

## 在标准IO库下面，系统会为程序自动打开三个标准IO流(文件)

- 标准输入流: `FILE *stdin`
  - `stdin` 是定义在 `stdio / cstdio` 里面的一个全局变量，它指向标准输入设备（一般是键盘）
  - `scanf`
- 标准输出流：`FILE *stdout`
  - `stdout` 是定义在 `stdio / cstdio` 里面的一个全局变量，它指向标准输出设备（一般是终端）
  - `printf`
- 标准出错流： `FILE *stderr`
  - `stderr` 是定义在 `stdio / cstdio` 里面的一个全局变量，它指向标准出错设备（一般是终端）
  - `perror`

## 示例：自定义设置缓冲区策略

```c++
#include <iostream>

int main()
{
    
    char in_buffer[10] = {0};
    setvbuf(stdout,in_buffer,_IOLBF,10);//设置一个标准输出流，以行缓冲进行输出
   
    printf("1234567890");
    printf("\n");
    printf("1234567890");
    printf("\n");  
    fflush(stdout);//手动刷新缓冲区
    

    return 0;
}
```

# 缓存（cache）与缓冲(buffer)的主要区别

**Buffer的核心作用是用来缓冲，$\textcolor{red}{缓和冲击}（对输出设备的冲击，包括磁盘、打印机、显示器）$。**比如你每秒要写100次硬盘，对系统冲击很大，浪费了大量时间在忙着处理开始写和结束写这两件事嘛。用个buffer暂存起来，变成每10秒写一次硬盘，对系统的冲击就很小，写入效率高了，日子过得爽了。极大缓和了冲击。

**Cache的核心作用是$\textcolor{red}{加快取用的速度}（加快读取速度，包括CPU读内存、内存读磁盘、用户通过浏览器请求资源)。$**比如你一个很复杂的计算做完了，下次还要用结果，就把结果放手边一个好拿的地方存着，下次不用再算了。加快了数据取用的速度。

简单来说就是**<u>buffer偏重于写，而cache偏重于读。</u>**

**缓冲：缓解冲击，缓存：临时存储**

# -------------------

# 标准IO的API接口(应用程序编程接口)

# 1.打开和关闭一个文件流==fopen,fclose==

```c
FILE *fopen(const char * __filename, const char * __modes)
/*
    @描述：
    	打开一个文件流
    @filename：
    	需要打开的文件名字（可以包含路径）
    @modes：
    	打开文件的方式：
        r 打开文本文件，用于读。流被定位于文件的开始。
        r+ 打开文本文件，用于读写。流被定位于文件的开始。
        w 将文件长度截断为零，或者创建文本文件，用于写。流被定位于文件的开始。
        w+ 打开文件，用于读写。如果文件不存在就创建它，否则将截断它。流被定位于文件的开始。
        a 打开文件，用于追加 (在文件尾写)。如果文件不存在就创建它。流被定位于文件的末尾。
        a+ 打开文件，用于追加 (在文件尾写)。如果文件不存在就创建它。读文件的初始位置是文件的开始，但是输出总是被追加到文件的末尾。
    返回值：
        成功返回，打开的文件的指针FILE*
        失败返回NULL，同时errno被设置
*/
```

```c
int fclose(FILE *stream);
/*
    作用：
    	关闭一个文件流
    stream：
    	需要关闭的文件流指针
    返回值：
        成功返回0
        失败返回EOF，同时errno被设置
*/
```

# 2.读写操作==fgetc、fputc、fgets、fputs==

## 文件的读取函数和文件的写入函数

## $\textcolor{red}{fgetc}$ - 从文件中读取单个字符

`fget getc getchar`

### - 功能：从文件或输入流中读取一行文本到内存中。

- ```c
  #include <stdio.h>
  int fgetc(FILE *stream);
  /*
      作用：
      	fgetc用来从stream指定的那个文件流中读取一个字符，并把读取到的字符的ASCII码返回
      stream：
      	需要获取字符的文件流指针
      返回值：
          成功返回字符的ASCII码
          失败返回EOF，同时errno被设置
  */
  
  ```

## ==fputc== - 写入单个字符到文件

- `fputc/putc/putchar....`

### - 功能：向文件写入一个字符

- ```c
  #include <stdio.h>
  int fputc(int c, FILE *stream);
  /*
      作用：
      	把c指定的字符输出到stream指定的那个文件流中
      c：
      	需要输出到文件流的字符的ASCII码
      stream：
      	指定输出的那个文件流指针
      返回值：
          成功返回字符的ASCII码
          失败返回EOF，同时errno被设置
  */
  ```

## $\textcolor{red}{fgets}$- 从文件中读取一行字符串

`fgets`

### - 功能：从文件或输入流中读取一行文本到内存中。

- ```c
  #include <stdio.h>
  char *fgets(char *s, int size, FILE *stream);
  /*
      作用：
      	从文件流指针对应的那个文件中获取一行字符。
      s：
      	指向的空间是用来保存从文件流中获取到的字符串的。
      size：
      	最多获取多少个字节，一般size为s指向的空间的可用长度
      stream：
      	表示从哪个文件中获取
      返回值：
          成功返回s的首地址
          失败返回NULL，同时errno被设置
  */
  ```

- `fgets` 输入结束两种情况 

  - 遇见了\n或者文件结束了
  -  已经获取到了size-1个字符（后面要预留\0的位置）

```c
FILE *file = fopen("1.txt", "r");
char buffer[1024];

// 从文件中读取一行到buffer
// 如果1.txt文件内容是："姓名：张三 年龄：18 专业：上海major"
fgets(buffer, 1024, file);
// 执行后 buffer 的内容："姓名：张三 年龄：18 专业：上海major"
```

## ==fputs==- 写入字符串到文件

`fputs`

### - 功能：向文件写入一个字符串

- ```c
  #include <stdio.h>
  int fputs(const char *s, FILE *stream);
  /*
      作用：
      	用来把s指向的字符串，输出到标准输出流(stdout)
      s：
      	需要输出到标准输出流的字符串指针
      stream：
      	需要输出字符串的文件流指针
      返回值：
          成功返回非负数
          失败返回EOF，同时errno被设置
  */
  ```

- puts和fputs区别

  -  `fputs` 可以指定输出到某个`stream`中去，`puts`只能输出到 `stdout` 中
  -  `fputs` 没有默认输出\n，`puts`是有默认\n的

# 3.直接读写==fread 、 fwrite==

想操作几个字符就操作几个字符

- `fread` 、 `fwrite`

## `fread` 读取

```c
#include <stdio.h>
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
/*
    作用：
    	从文件流中读取size个字节的数据
    ptr：
    	读取到的数据所保存的空间首地址
    size：
    	单个对象（元素的大小）大小
    nmemb：
    	读取对象的个数
    stream：
    	需要读取数据的文件流
    返回值：
        成功返回，实际读取到的对象个数！！！ < === nmemb
        失败返回EOF，同时errno被设置
*/
```

```c++
#include<iostream>

int main()
{   
    FILE *file_ptr =  fopen("设置缓冲区策略.cpp","r");

    char buffer[1024] = {0};
    std::cout<< "元素数量：" << fread(buffer,1,1024,file_ptr) << std::endl;
    fclose(file_ptr);
    std::cout<< buffer <<std::endl;

    return 0;
}
```

## fwrite 写入

```c++
#include <stdio.h>
size_t fwrite(const void *ptr, size_t size, size_t nmemb,FILE *stream);
/*
    作用：
    	往文件流中写入数据
    ptr：	
    	需要写入文件流的数据空间的首地址
    size：
    	单个对象（元素的大小）大小
    nmemb：
    	读取对象的个数，次数
    stream：
    	需要写入数据的文件流
    返回值：
        成功返回，实际读取到的对象个数！！！ < === nmemb
        失败返回EOF，同时errno被设置
        EOF ====> End Of File
*/
```

```c++
// #include <cstdio>
#include <iostream>
int main() {
    FILE* file = fopen("test.txt", "w");  
    if (file) {
        fwrite("Hello1", 1, 6, file);
        fclose(file);
        std::cout << "文件写入完成" << std::endl;
    }
    return 0;
}
```

# ---------------

# 冲洗一个文件流fflush

冲洗流： `fflush` 主要作用就是同步文件。（把缓冲区中的内容同步中硬件中）

标准IO带缓冲的IO，读写时候针对与缓冲区，并不是每次都是操作硬件

有的时候多线程编程的时候会影响到程序结果。

```c++
FFLUSH(3) 									Linux
Programmer's Manual
FFLUSH(3)
    
NAME
	fflush - flush a stream
    		
SYNOPSIS
    
#include <stdio.h>
int fflush(FILE *stream);
/*
    @描述：
    	强制给所在的输出流或是更新流(stream)上，写入在用户空间缓冲的所有数据，使用底层写功能函数
    	流的打开状态是不受影响的。
   	 	如果stream是NULL，fflush刷新所有打开的流。
    @stream
    	需要刷新的文件流指针
    @return：
        成功返回0
        失败返回EOF，同时errno被设置
*/
```

````c++
#include<iostream>

int main()
{
    FILE *file_ptr = fopen("fflush.cpp","a+");
    fwrite("//123456",1,8,file_ptr);
    fclose(file_ptr);
    return 0;
}//123456
````



# 设置缓冲区setvbuf

在 `linux` 下缓冲区我们可以自己进行设置缓冲区： `setbuf` 、 `setvbuf`

```c++
SETBUF(3) 			Linux Programmer's Manual
		SETBUF(3)
NAME
	setbuf, setbuffer, setlinebuf, setvbuf - stream buffering operations
SYNOPSIS
    
#include <stdio.h>
void setbuf(FILE *stream, char *buf);
void setbuffer(FILE *stream, char *buf, size_t size);
void setlinebuf(FILE *stream);
// 上面三个函数的功能它都有
int setvbuf(FILE *stream, char *buf, int mode, size_t size);
/*
    @描述：
        更改缓冲区策略。
        有三种类型的缓冲策略，它们是无缓冲，块缓冲和行缓冲。当输出流无缓冲时，信息在写
        的同时出现于目标文件或终端 上；当是块缓冲时，字符被暂存，然后一起写入；当是行缓
        冲时，字符被暂存，直到要输出一个新行符，或者从任何与终 端设备连接的流中 (典型的
        是 stdin) 读取输入时才输出。函数 fflush(3) 可以用来强制提前输出。(参见
        fclose(3)) 通常所有文件都是块缓冲的。当文件 I/O 操作在文件上发生时，将调用 malloc(3) ，获
        得一个缓冲。 如果流指向一个终端 (通常 stdout 都是这样)，那么它是行缓冲的。标准
        错误流 stderr 默认总是无缓冲的。
    @stream
    	需要设置缓冲区策略的文件流指针
    @buf
        缓冲区指针
        注意！！！！！！！！！！！！！！！！！！！！！！
        buf必须要是在文件流关闭之前，以及程序结束之前它的空间都应该要有效（也就是从文
        件流开始到程序结束该段空间都不能被释放）
    @mode
    	缓冲策略
    	
        参数mode 必须是以下宏值
            _IONBF 无缓冲
            _IOLBF 行缓冲
            _IOFBF 全缓冲
        
    @size
        	缓冲区的大小
    @return
        成功返回0
        失败可能返回任意值
*/
```



# 定位文件流

## ==设置光标位置fseek==

上面有提到 `fread` 和 `fwrite` ....只是说明要从哪个文件读写，读写多少个，并没有指明，从哪个位置开 始。标准IO会为每一个打开的文件流，保存一个“文件偏移量”（系统IO也有）

一般来说，每次读写之前，先确定光标（定位流）。

**`fseek` 就是用来定位文件流的光标的。**

```c
#include <stdio.h>
int fseek(FILE *stream, long offset, int whence);
/*
    作用：
        fseek就是用来定位文件流的光标的。
    stream：
        需要进行定位流的，文件流指针
    offset：
        偏移量，结合第三个参数。
        负：表示向前偏移
        正：表示向后偏移
    whence：
        SEEK_SET 基于文件开头定位 新光标的位置=文件开头+offset(>=0)
        SEEK_CUR 基于当前位置定位 新光标的位置=当前位置+offset(可正可负)
        SEEK_END 基于文件末尾定位 新光标的位置=文件末尾+offset(可正可负)
    返回值：
        成功返回实际偏移的数量，失败返回-1，同时errno被设置
    //return lseek(stream->fileno,offest,whence);
*/
```

## 示例：fopen+fread+fseek+ftell

```c++
#include<iostream>

// int fseek(FILE *stream,int offset,int whence)
// {

//     return lssek(stream->_fileno,offset,whence);
// }

int main()
{
    FILE* file_ptr = fopen("fflush.cpp","r");

    char buffer[1024]={0};
    // 每次读写之前，先确定光标（定位流），【第一次读，不指定光标位置，默认从文件开头开始读取】
    fread(buffer,1,10,file_ptr);//#include<i
    //读取文件中前10个字节的内容
    std::cout << buffer << std::endl;

    //光标定位到：文件开头的位置偏移1个字节
    fseek(file_ptr,1,SEEK_SET);
    //从光标定位的位置开始，【每个元素大小为1，读取十个元素】
    fread(buffer,1,10,file_ptr);//include<io
    std::cout << buffer << std::endl;

    //从光标后来定位的位置开始，【每个元素大小为1，读取十个元素】
    fread(buffer,1,10,file_ptr);        
    std::cout << buffer << std::endl;   
    //stream>   
    //
    //i
    std::cout<<ftell(file_ptr)<<std::endl;//记录当前光标的偏移量
    
    fseek(file_ptr,0,SEEK_END);// 将光标移动到文件末尾
    std::cout<<ftell(file_ptr)<<std::endl;// 输出文件总字节数
    return 0;
}
```



## ==获取光标偏移量ftell==

获取此时光标的偏移量是多少（也可以理解为此时已经从文件头移动了多少个字节的数据）

```c
#include <stdio.h>
long ftell(FILE *stream);
/*
    作用：
    	ftell是用来计算当前光标到文件头的距离（有多少个字节）
    stream：
    	需要统计开头到当前位置的字节数的，文件流指针。
    返回值：
    	成功返回文件头到当前位置的字节数，失败返回-1，同时errno被设置
*/
```

## 示例：fopen+fseek+ftell

```c++
#include<iostream>

int main()
{
    FILE * file_ptr = fopen("fflush.cpp","r");

    fseek(file_ptr,0,SEEK_END); // 将光标移动到文件末尾
    std::cout << ftell(file_ptr) << std::endl;// 输出文件总字节数
    return 0;
}
```

## ==重置光标位置rewind==

```c
#include <stdio.h>
void rewind(FILE *stream);
/*
    作用：
    	将光标定位到文件头
    stream：
        需要将光标定位到文件头，文件流指针
        使用该函数等价fseek(stream,0,SEEK_SET)一样
*/
```

## 示例：fopen+fseek+ftell+rewind

```c++
#include<iostream>

int main()
{
    FILE * file_ptr = fopen("fflush.cpp","r");

    fseek(file_ptr,0,SEEK_END); // 将光标移动到文件末尾
    std::cout << ftell(file_ptr) << std::endl;// 输出文件总字节

    rewind(file_ptr);// 将文件指针重置到文件开头
    std::cout << ftell(file_ptr) << std::endl;// 输出 0

    fclose(file_ptr);
    return 0;
}
```



# 文件出错/文件结束标志 `clearerr` `feof` `ferror`

`EOF` ： `End Of File` 文件结束的标志，

- 在 C/C++ 标准库中，EOF 通常被定义为 `-1`

- 它是一个**返回值**，表示"已经到达文件末尾"或"读取失败"
- 它**绝对不是**存储在文件内容中的特殊字节

<u>"操作系统和文件系统知道文件的精确大小，当读取位置达到文件大小时，标准库函数会返回 EOF 值来指示$\textcolor{red}{这一状态 }$"</u>

文件结束符只能被`clearerr()`清除

```c
FERROR(3) 			Linux
	Programmer's Manual
FERROR(3)
NAME
    
clearerr, feof, ferror, fileno - check and reset stream status
    
SYNOPSIS
#include <stdio.h>
void clearerr(FILE *stream);
/*
    @描述：
        清除stream指向的流中文件结束标志和出错标志的
    @stream：
        需要清除文件结束标志和出错标志的stream流
*/
int feof(FILE *stream);
/*
    @描述：
    	测试指向流中的文件结束标志，如果已设置就返回
    @stream
    	需要测试文件结束标志的流
    @return：
    	如果已设置就返回非0,即返回非0值 表示已到达文件末尾
    	未设置返回0，即返回0 表示未读到文件末尾
*/
int ferror(FILE *stream);
int fileno(FILE *stream);
```

```c
#include<iostream>
int main()
{   
    FILE * file_ptr = fopen("fflush.cpp","r");
    char buff[1024] = {0};
    while(!feof(file_ptr))
    {
        fread(buff,1024,1,file_ptr);
        std::cout<<buff;
    }
    
    fclose(file_ptr);
    std::cout<<std::endl;
    return 0;
}
```

```
#include<iostream>
#include<cstring>
int main()
{
    FILE *file_ptr = fopen("fflush.cpp","r");
    if(!file_ptr) 
    {
        std::cerr << "Failed to open file" << std::endl;
        return 1;
    }

    char buff[1024] = {0};

    //将实际读取个数给到bytesRead
    size_t bytesRead;
    bytesRead = fread(buff, 1, sizeof(buff), file_ptr);
    std::cout.write(buff, bytesRead)<<std::endl; // 精确输出读取的字节数
    std::cout<<bytesRead<<std::endl;


    fclose(file_ptr); 
    std::cout<<std::endl;
    return 0;
} 
```



# 格式化的输入输出

**格式化**：按照特定格式进行输入/输出

**分为两类参数**：

- **格式化字符串**：
  - 告知用户按照哪种格式进行输入/输出，怎么输入/输出。
  - 格式化字符串中字符分为三类：
    - 非转义字符/普通字符
      - `A B C ... 1 2 3... ! ? , ...` 必须要按格式化中的样子输入。精准输入
    - 格式化字符
      - `%d %s %c %f %o` ... 占位符，表示接收一个指定类型的数据
        - %d ：表示接收一个整型数据 `scanf("%d", &num);`
        -  %s ：表示接收一个字符串数据  `scanf("%s", str);`
        -  %c ：表示接收一个字符数据scanf("%c", &ch);
        - %f ：表示接收一个浮点数 `scanf("%f", &score);`
        - %o ：表示接收一个整型并以八进制形式打印  `scanf("%o", &Octal);`
        - %x：表示接受一个整形并以十六进制形式打印 `scanf("%x",%hex)`
    - `转义字符`
      -  `\n \t \a \x89 \0777 ...` 通过反斜杠转义的不是原含义的字符
      -  空白字符： `\n \t \a \r ...` 
      - 非空白字符： `\x89 \0777 ...`
- 格式化参数
  - 要与格式化字符串中占位字符的类型和数量要一致

**注意：格式化字符串中的每一个格式化字符，就对应一个地址，所以在输入的时候，参数要与格式化字 符的类型和数量相匹配，然后需要提供地址。**

## 格式化输入 scanf族：==scanf/sscanf/fscanf==

`scanf/sscanf/fscanf...`

### scanf- ==标准输入解析函数==

**在C语言中，scanf函数是一个用于从标准输入（通常是键盘）读取格式化数据的标准库中的输入函数。它的名称源自“scan formatted”，意思是格式化扫描。**

#### - 功能：从键盘读取并解析数据

**scanf函数根据指定的格式字符串来读取输入，并将读取的值存储到对应的变量中。**

```c
int scanf(const char *format, ...);
```

scanf 什么时候结束输入？

- scanf 从 stdin 的缓冲区中获取输入
  - 当该输入的都输入完了。
    - `scanf("abcd%d %cbcd",&a,&c); `
    - `abcd123Abcd`
  - 用户输入失败
    - `scanf(abcd%d %cbcd",&a,&c);` 
    - `ABCD` // `scanf`就会停止匹配，结束

- #### scanf 返回成功匹配到的变量个数。

````c
#include <stdio.h>

int main() {
    char name[50];
    int age;
    
    printf("请输入姓名和年龄：");
    scanf("%s %d", name, &age);  // 从键盘读取
    
    printf("姓名：%s，年龄：%d\n", name, age);
    return 0;
}


请输入姓名和年龄：张三 18
姓名：张三，年龄：18
````



### fscanf - ==文件输入解析函数==

用于从**文件**中按照指定格式读取数据。我们可以把它理解为“**文件版的 `scanf`**”。

#### - 功能：直接从文件解析数据

- `scanf(“格式控制”, &变量)` 是从**标准输入（通常是键盘）**读取。
- `fscanf(文件指针, “格式控制”, &变量)` 是从**你指定的文件**读取。

```c++
#include <stdio.h>
int fscanf(FILE *stream, const char *format, ...);
/*
    作用：
    	从指定的来源，接受输入数据
    stream：
    	输入源，文件流指针
    format：
    	格式化字符串
    ...：
    	可变参数
    	
    int： 返回值。代表成功读取并赋值的数据项的数量。如果读取失败或在匹配任何数据之前就到了文件末尾，则返回 EOF。
*/
eg：
FILE *fp = fopen("1.txt"."r");
int r,a;char c;
fscanf(fp,"%d%d%c",&r,&a,&c);
```

`fscanf` **读取文件内容的时候是不会主动偏移光标的。**

```c++
#include<iostream>

int main()
{
    // 姓名：张三，年龄：18,专业：上海major
    FILE * file_ptr = fopen("1.txt","r");

    char name[1024] = {0};
    char major[1024] = {0};
    int age = 0;
    fscanf(file_ptr,"姓名：%s 年龄：%d 专业：%s",name,&age,major);
    std::cout<< "姓名："<<name<<" 年龄："<<age<<" 专业："<<major<<std::endl;
    fclose(file_ptr);
    return 0;
}
```

### sscanf- ==字符串输入解析函数==

**`sscanf` 的全称是 **String Scan Formatted**。**

#### - 功能：从内存中的字符串解析数据

**解析**一个字符串，并根据你提供的**格式说明**，将字符串中的内容提取出来，存放到相应的变量中。

可以把它理解为 `scanf` 函数的**“字符串版本”**。

- `scanf`：从**标准输入流**（通常是键盘）读取数据。
- `sscanf`：从一个你**指定的字符串**中读取数据。

```c++
#include <stdio.h>
int sscanf(const char *str, const char *format, ...);
/*
    作用：
    	从指定的来源，接受输入数据
    str：
    	输入源
    format：
    	格式化字符串
    ...：
    	参数
*/
```

````c++
#include <iostream>
#include <cstdio>

int main() {
    const char *str = "123456asdsa4564asd";
    int r = 0, a = 0;
    char text1[100] = {0}, text2[100] = {0}; // 用来存储字符串
    
    // 修改格式字符串，使用扫描集 %[a-zA-Z] 来匹配字母
    int result = sscanf(str, "%d%[a-zA-Z]%d%[a-zA-Z]", &r, text1, &a, text2);
    
    std::cout << "成功匹配了 " << result << " 个项目" << std::endl;
    std::cout << "第一个数字: " << r << std::endl;
    std::cout << "中间文本: " << text1 << std::endl;
    std::cout << "第二个数字: " << a << std::endl;
    std::cout << "最后文本: " << text2 << std::endl;
    
    // 组合输出完整内容
    std::cout << "完整内容: " << r << text1 << a << text2 << std::endl;
    
    return 0;
}
````

### 示例：sscanf和fgets函数

```c++
#include<iostream>
int main()
{
    FILE * file = fopen("1.txt","r");
    char name[1024] = {0};
    char major[1024] = {0};
    int age = 0;
    char buffer[1024] = {0};
    while(fgets(buffer,1024,file))
    {
        sscanf(buffer,"姓名：%s 年龄：%d 专业：%s\n",name,&age,major);
        std::cout<<name<<" ："<<age<<" ："<<major<<std::endl;

    }
    std::cout<<"--------"<<std::endl;

    return 0;
}

张三 ：18 ：上海major
张三 ：19 ：上海major
张三 ：20 ：上海major
张三 ：21 ：上海major
张三 ：22 ：上海major
--------
```



## 格式化输出 fprintf、sprintf

### fprintf - ==文件写入解析函数==

将格式化的数据写入到指定的文件流中。

适合向各种输出流（屏幕、文件等）写入格式化数据

```c++
#include <stdio.h>
int fprintf(FILE *stream, const char *format, ...);
/*
    stream:
    	需要输出到的文件流指针
    format：
    	输出格式字符串
    ...：
    	参数
    返回值	
    	成功写入的字符数
*/
```

### 示例：

#### C风格

```c
#include <stdio.h>

int main() {
    int age = 25;
    double salary = 8000.50;
    char name[50] = "张三";  // 增大缓冲区
    
    // 输出到标准输出（屏幕）
    fprintf(stdout, "姓名: %s, 年龄: %d, 工资: %.2f\n", name, age, salary);
    
    // 输出到标准错误
    fprintf(stderr, "这是一条错误信息\n");
    
    // 从用户获取输入（使用C语言的scanf）
    printf("请输入姓名、年龄和工资: ");
    scanf("%49s %d %lf", name, &age, &salary);  // 限制字符串长度
    
    // 输出到文件
    FILE *file = fopen("output.txt", "a+");
    if (file != NULL) {
        fprintf(file, "数据写入文件: %s %d %.2f\n", name, age, salary);
        fclose(file);
        printf("数据已成功写入文件!\n");
    } else {
        fprintf(stderr, "无法打开文件!\n");
        return 1;
    }
    
    return 0;
}
```

#### C++风格

```c++
#include <iostream>
#include <fstream>
#include <string>

int main() {
    int age = 25;
    double salary = 8000.50;
    std::string name = "张三";
    
    // 输出到标准输出
    std::cout << "姓名: " << name << ", 年龄: " << age << ", 工资: " << salary << std::endl;
    
    // 输出到标准错误
    std::cerr << "这是一条错误信息" << std::endl;
    
    // 从用户获取输入
    std::cout << "请输入姓名、年龄和工资: ";
    std::cin >> name >> age >> salary;
    
    // 输出到文件
    std::ofstream file("output.txt");
    if (file.is_open()) {
        file << "数据写入文件: " << name << " " << age << " " << salary << std::endl;
        file.close();
        std::cout << "数据已成功写入文件!" << std::endl;
    } else {
        std::cerr << "无法打开文件!" << std::endl;
        return 1;
    }
    
    return 0;
}

#include<iostream>

int main()
{
    char name[1024] = {0};
    int age = 0;
    char major[1024] = {0};

    FILE * file = fopen("1.txt","a+");

    std::cout<<"请输入姓名、年龄和专业"<<std::endl;
    std::cin >> name >> age >> major;
    
    fprintf(file,"姓名：%s 年龄：%d 专业：%s\n",name,age,major);

    return 0;
}
```

### sprintf - ==字符串写入解析函数==

用于将格式化的数据写入到字符数组中（字符串）

```c++
#include <stdio.h>
int sprintf(char *str, const char *format, ...);
/*
    str:
    	需要输出到的字符指针。
    format：
    	输出格式字符串
    ...:
    	参数
*/
char buf[256]={0};
int a = 1;char c = 'a';
sprintf(buf,"%d-----%c",a,c);
```

### 示例：

#### C风格

```c
#include <stdio.h>

int main() {
    char buffer[100];
    int year = 2024;
    int month = 3;
    int day = 20;
    char event[] = "编程学习";
    
    // 将格式化数据写入字符串
    sprintf(buffer, "日期: %d年%d月%d日, 事件: %s", year, month, day, event);
    
    printf("生成的字符串: %s\n", buffer);
    return 0;
}
```

#### C++风格

```c++
#include<iostream>
int main()
{
    char name[1024] = {0};
    int age = 0;
    char major[1024] = {0};
    char buffer[1024] = {0};

    FILE * file = fopen("1.txt","a+");

    std::cout<<"请输入姓名、年龄和专业"<<std::endl;
    std::cin >> name >> age >> major;
    
    fprintf(file,"姓名：%s 年龄：%d 专业：%s\n",name,age,major);//将姓名、年龄和专业写入file文件中

    sprintf(buffer, "姓名：%s 年龄：%d 专业：%s\n",name,age,major)//姓名、年龄和专业写入字符串buffer中

    return 0;
}
```

# cerr 和 stderr

## C语言中的 stderr

### 基本概念
`stderr`是C语言标准库中预定义的文件指针，用于输出错误信息。

```c
#include <stdio.h>

int main() {
    // 使用stderr输出错误信息
    fprintf(stderr, "错误：文件打开失败！\n");
    
    // 与stdout对比
    printf("这是正常输出\n");        // 输出到stdout
    fprintf(stderr, "这是错误输出\n"); // 输出到stderr
    
    return 0;
}
```

### 特点
- **无缓冲**：立即输出，不等待缓冲区满
- **独立通道**：与标准输出(stdout)分离
- **默认输出到控制台**：但可以重定向

## C++中的 cerr

### 基本概念
`cerr`是C++标准库中的标准错误流对象，定义在`<iostream>`头文件中。

```cpp
#include <iostream>

int main() {
    // 使用cerr输出错误信息
    std::cerr << "错误：除零操作！" << std::endl;
    
    // 与cout对比
    std::cout << "这是正常输出" << std::endl;  // 输出到标准输出
    std::cerr << "这是错误输出" << std::endl;  // 输出到标准错误
    
    return 0;
}
```

## 关键区别

### 1. 缓冲机制
```cpp
#include <iostream>
#include <cstdio>
#include <unistd.h>  // for sleep

int main() {
    // cout - 有缓冲（通常行缓冲）
    std::cout << "cout信息（可能不会立即显示）";
    
    // cerr - 无缓冲
    std::cerr << "cerr信息（立即显示）";
    
    // C中的对比
    printf("printf信息（可能不会立即显示）");
    fprintf(stderr, "stderr信息（立即显示）");
    
    sleep(2);  // 等待2秒，观察输出时机
    std::cout << std::endl;  // 此时cout缓冲区才刷新
    
    return 0;
}
```

### 2. 重定向示例
在命令行中，可以分别重定向标准输出和标准错误：

```bash
# 将标准输出重定向到output.txt，错误输出仍显示在控制台
./program > output.txt

# 将错误输出重定向到error.txt，标准输出仍显示在控制台
./program 2> error.txt

# 分别重定向
./program > output.txt 2> error.txt

# 合并重定向
./program > both.txt 2>&1
```

## 最佳实践

1. **使用场景**：
   - `cerr/stderr`：错误信息、警告、调试信息
   - `cout/stdout`：正常程序输出、结果

2. **性能考虑**：
   - 频繁的错误输出可能影响性能（无缓冲）
   - 生产环境中考虑使用日志系统

3. **可读性**：
   - 错误信息应该清晰、具体
   - 包含足够的上下文信息

## 总结

| 特性   | C++ cerr          | C stderr                   |
| ------ | ----------------- | -------------------------- |
| 头文件 | `<iostream>`      | `<stdio.h>`                |
| 类型   | 流对象            | 文件指针                   |
| 缓冲   | 无缓冲            | 无缓冲                     |
| 用法   | `cerr << "error"` | `fprintf(stderr, "error")` |
| 重定向 | 支持              | 支持                       |

# 关于C++中的标准库的使用

```c++
#include <fstream>
ofstream //文件写操作 内存写入存储设备
ifstream //文件读操作，存储设备读区到内存中
fstream //读写操作，对打开的文件可进行读写操作
```

在 `fstream` 类中，成员函数 `open()` 实现打开文件的操作，从而将数据流和文件进行关联，通过 `ofstream`,`ifstream`,`fstream` 对象进行对文件的读写操作

````c++
函数：ope()
public member function
void open (const char * filename,ios_base::openmode mode = ios_base::in |
ios_base::out );
void open(const wchar_t *_Filename,ios_base::openmode mode= ios_base::in |
ios_base::out,int prot = ios_base::_Openprot)；
参数：   filename 操作文件名
		mode 打开文件的方式
		prot 打开文件的属性 //基本很少用到，在查看资料时，发现有两种方式
````

````c++
os::in 为输入(读)而打开文件
ios::out 为输出(写)而打开文件
ios::ate 初始位置：文件尾
ios::app 所有输出附加在文件末尾
ios::trunc 如果文件已存在则先删除该文件
ios::binary 二进制方式
````

# 作业：

将标准IO文件操作封装成一个类。

