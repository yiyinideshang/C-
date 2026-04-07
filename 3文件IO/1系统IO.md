# 文件系统

# “裸奔”层次：不带操作系统的编程

```
APP 应用层
-----------------
HardWare 硬件层
```

**特点**：

- 简单，粗暴应用层直接操作硬件(寄存器、IO口...)

**缺点**：

- 开发应用、要能搞懂原理图 
- 了解硬件的细节 
- 不能同时运行多个任务：“单任务运行”

# 带“OS”的编程

**操作系统**（英语：Operating System，缩写：OS）*是一种内置的程序，用来协作**计算机**的各种硬件，以与用户进行交互。常见有**Windows**，**macOS** 和开源的 **Linux**、**华为鸿蒙系统**。

**操作系统**`OS`：`Operating System `是直接运行在“**裸机**”的最基本的系统软件，任何软件/应用程序都 必须要在操作系统的支持下才能正常运行。

```
APP 应用层
-----------------
OS 操作系统
-----------------
Hardware 硬件层
```

**特点**：

- 开发应用、主要实现应用的逻辑实现
-  不需要关心底层硬件的实现细节
-  **可以支持多任务运行、并发**

**缺点**：

- **应用程序编程接口 **`API`（application interface） 太多了
- `API `的实现（功能）可能不是我们想要的

**常见的操作系统**：

-  windows` x86/xp/win8/win10/win11 `
- linux `ubuntu/centos/redhat/kali `
- mac
- IOS 
- Android 
- 鸿蒙 
- `unix`

`Linux`操作系统下进行开发应用软件，就是调用`Linux`操作系统为我们提供的系统的 `API` 函数接口去操作 硬件。或者是说用`Linux`提供的服务来完`Linux`下的一些开发。

# Linux文件系统

在`Linux`下面，**一切皆文件**。在`Linux` **下面操作任何东西**，其实都是通过**操作文件**来实现的，通过文件的 **接口**实现。

## 文件系统 

用来存储、组织、管理文件的一套方式、方法、协议以及软件的实现...

**文件**：

- 文件属性： `inode` （编号）； `inode` ----> 文件内容... 
- 文件内容：**文件的实际存储的内容**

文件在Linux下的操作过程:

- 硬链接（找文件名）----> inode

- Linux内核

  ```c++
  // 是用来描述一个文件的物理的inode信息
  // 系统如果识别到了一个文件的存在，就会为它在linux内核里面创建一个struct inode的一个文件
  对应的inode。
  struct inode{};
  /*
  用来描述一个已经打开了的文件信息的。
  文件状态偏移了（类似于文本光标）
  每个打开的文件都会对应一个struct file
  一个文件是可以被多个不同应用打开
  */
  struct file{}
  ```

- 操作过程： `sturct file --> struct inode --> 硬件上的inode --> 文件内容区域` 

`Lin	ux`为了屏蔽掉文件操作的一些具体实现细节，它会为进程创建一个“ **进程文件表项** ”

## 进程文件表项

每一个应用都会有一个**进程文件表项**，保存进程打开的文件的 `sturct file 结构体指针`，相当于是一 个 `struct file *` 的数组（集合），打开一个文件，只需要返回一个下标给我使用就可以了

```c++
struct file* []
打开一个文件，文件的struct file指针被分配到0下标，就可以使用0下标在进程文件表项中获取文件的
struct file指针使用了。
```

````c++
open // 打开一个文件
// 返回值就是打开的文件在进程文件表项中的下标
````

Linux提供系统操作函数： `open`、`write`、`read`、`close`... 以上这些函数都是称为：“**系统IO**”

**系统IO**：操作系统所提供的对于文件的**输入** **输出**的函数接口

# Linux系统IO的应用

**对于文件的操作大致**：

- 打开文件 
- 写入文件内容/读取文件内容
-  移动文件光标位置
- 关闭文件

## 打开文件：open

O_RDONLY 以只读方式打开

O_WRONLY 以只写方式打开

O_RDWR 以可读可写方式打开

```c
OPEN(2) 	System 	calls 	OPEN(2)

NAME
	open, creat - 打开和/或创建一个文件
    
SYNOPSIS 总览
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
	int open(const char *pathname, int flags);
	/*
        @描述：
        	打开一个文件
        
        @pathname：
        	需要打开的文件的名字（路径+文件名+扩展名）
        
        @flags：标志位
        	打开方式：
                O_RDONLY 以只读方式打开
                O_WRONLY 以只写方式打开
                O_RDWR 以可读可写方式打开
         
        @return：
            成功返回文件的struct file指针在进程文件表项中的下标（文件描述符），失败返
            回-1
            
	*/
	int open(const char *pathname, int flags, mode_t mode)
	/*
        @描述：
        	打开一个文件
        	
        @pathname：
        	需要打开的文件的名字（路径+文件名+扩展名）
        	
        @flags：标志位
        	打开方式：
                O_RDONLY 以只读方式打开
                O_WRONLY 以只写方式打开
                O_RDWR 以可读可写方式打开
                
        @mode：
        	当你使用了O_CREAT标志的时候，请你填写此参数
        	mode用来设定文件的权限的
        	当然最终文件权限取决umask和填写权限运算
        	
        @return 成功返回文件的struct file指针在进程文件表项中的下标，失败返回-1
    */
```

```c++
open函数返回一个整数（int）类型的文件描述符（file descriptor）。
如果打开文件成功，返回的值 通常是3，因为0、1、2分别被标准输入、标准输出、标准错误占用，下一个打开的文件描述符就是3，代表文件描述符。
如果失败，返回-1。
```

```c++
//这是open.cpp文件

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

int main()
{
    int file_id = open("1.txt",O_RDONLY);
    if(file_id == -1)
    {
        std::cout<<"文件打开失败"<<std::endl;
    }
    else
    {
        std::cout<<"文件打开成功"<<std::endl;
    }
}

//这是open_create.cpp文件
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

int main()
{
    int file_id = open("11.txt",O_RDONLY|O_CREAT,0777);
    if(file_id == -1)
    {
        perror("文件打开失败");
        return -1;
    }
    std::cout<<"文件打开成功"<<std::endl;
    return 0;
}


yishang@yishang-virtual-machine:~/文档/文件IO$ g++ open.cpp
yishang@yishang-virtual-machine:~/文档/文件IO$ ./a.out 
文件打开成功
yishang@yishang-virtual-machine:~/文档/文件IO$ g++ open_create.cpp 
yishang@yishang-virtual-machine:~/文档/文件IO$ ./a.out 
文件打开成功
```

## --------------

## 文件的读/写：read/write

### 读取文件内容:read

```c
READ(2) 	Linux 	Programmer's Manual 	READ(2)
NAME
	read - 在文件描述符上执行读操作
概述
	#include <unistd.h>
    
	ssize_t read(int fd, void *buf, size_t count);
	/*
        @描述：
        	从一个指定的文件描述符中（fd）读取count个字节的数据存储到buf所指向的内存空间中
        
        @fd：
        	file id 文件描述符（进程文件表项的下标），一般由open函数返回。需要是有效文件描述符
        
        @buf：
       		void * 类指针，用来存储读取到数据内容的内存空间的首地址指针

        @count：
        	个数，表示需要读取多少个字节
        
        @return：
            成功：实际读取到的字节数量
            失败：返回-1
	*/
```

### ==示例：读取文件内容read==

```c++
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<iostream>
#include<unistd.h>

int main()
{
    // fd = file_id
    int file_id = open("11.txt",O_RDONLY|O_CREAT,0777);
    if(file_id == -1)
    {   
        perror("文件打开失败");
        return -1;
    }
    std::cout<<"文件打开成功"<<std::endl;

    // 存储读取到的内容的空间地址
    char buffer[512]={0};
    int ret = read(file_id,buffer,512);
    std::cout<<"实际读取："<<ret<<"个字节"<<std::endl;
    std::cout<<"内容："<<buffer<<std::endl;

    return 0;
}

yishang@yishang-virtual-machine:~/文档/文件IO$ g++ read.cpp 
yishang@yishang-virtual-machine:~/文档/文件IO$ ./a.out 
文件打开成功
实际读取：9个字节
内容：123456789
```



### 写入内容到文件:write

```c
WRITE(2)	 Linux 	Programmer's Manual 	WRITE(2)
NAME
    write -在一个文件描述符上执行写操作
    
概述
    #include <unistd.h>
    
    ssize_t write(int fd, const void *buf, size_t count);
    /*
    @描述：
    	往一个指定的文件描述符中（fd）写入buf所指向的内存空间中的count个字节的数据
    	
    @fd：
   		file id 文件描述符，一般由open函数返回。需要是有效文件描述符
   		
    @buf：
    	void * 指针，用来存储写入数据的存储内存空间的首地址指针
    	
    @count：
    	个数，表示需要写入多少个字节
    	
    @return：
        成功：实际写入到的字节数量
        失败：返回-1
    */
```

### ==示例：写入内容到文件write==

```c++
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<iostream>
#include<unistd.h>

int main()
{
    // fd = file_id
    // int file_id = open("11.txt",O_RDWR|O_CREAT,0777);//这样会覆盖之前内容
    int file_id = open("11.txt",O_RDWR|O_APPEND);//这样只会追加内容

    if(file_id == -1)
    {   
        perror("文件打开失败");
        return -1;
    }
    std::cout<<"文件打开成功"<<std::endl;

    // 存储读取到的内容的空间地址
    char buffer[512]={"abcdefg"};
    int ret = write(file_id,buffer,7);
    std::cout<<"实际写入："<<ret<<"个字节"<<std::endl;
    std::cout<<"写入内容："<<buffer<<std::endl;

    return 0;
}

yishang@yishang-virtual-machine:~/文档/文件IO$ g++ write.cpp 
yishang@yishang-virtual-machine:~/文档/文件IO$ ./a.out 
文件打开成功
实际写入：7个字节
写入内容：abcdefg
    
//如果用这种方式int file_id = open("11.txt",O_RDWR|O_CREAT,0777);会将输入的内容覆盖到原来的内容上，即最开始是123456789，会变成abcdefg89
    
//如果用这种方式 int file_id = open("11.txt",O_RDWR|O_APPEND);不会将输入的内容覆盖，即最开始为123456789，会变成123456789abcdefg
```

## -------------

## 移动文件光标位置:lseek

有时候咱们想在文件中间某个指定的位置插入内容

```c++
#include <sys/types.h>
#include <unistd.h>

off_t lseek(int fd, off_t offset, int whence);
/*
    作用：
    	设置一个文件的光标偏移量
    返回值：
    	实际偏移字节数量，失败返回-1
    fd：
    	需要进行光标偏移的文件描述符。
    offset:
        off_t类型 光标需要偏移的字节数量。可以为正也可以为负
        如果为正数，则表示光标向文件末尾的方向偏移
        如果为负数，则表示光标向文件头的方向偏移
    whence：
    	SEEK_SET
            The file offset is set to offset bytes.
            从文件开头的位置进行偏移
            
   		SEEK_CUR
            The file offset is set to its current location plus off‐
            set bytes.
            从当前位置开始进行偏移。
            
    	SEEK_END
            The file offset is set to the size of the file plus off‐
            set bytes.
            从文件末尾开始进行偏移
            
    @return 成功返回实际偏移的字节数，失败返回-1
*/
```

==注意：如果偏移到了文件末尾了，还继续偏移那么会形成空洞，此时写入数据会把空洞填充NULL内容==

### ==示例：在文件指定光标位置写入内容lseek==

```c++
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<iostream>
#include<unistd.h>

int main()
{
    // fd = file_id
    // int file_id = open("11.txt",O_RDWR|O_CREAT,0777);//这样会覆盖之前内容
    int file_id = open("11.txt",O_RDWR);// O_RDWR 以可读可写方式打开

    if(file_id == -1)
    {   
        perror("文件打开失败");
        return -1;
    }
    std::cout<<"文件打开成功"<<std::endl;

    //向后偏移7个字节
    lseek(file_id,7,SEEK_SET);


    // 存储读取到的内容的空间地址
    char buffer[512]={"abcdefg"};
    int ret = write(file_id,buffer,7);
    std::cout<<"实际写入："<<ret<<"个字节"<<std::endl;
    std::cout<<"写入内容："<<buffer<<std::endl;

    return 0;
}

//当11.txt文本中有内容123456789时

yishang@yishang-virtual-machine:~/文档/文件IO$ g++ lseek.cpp 
yishang@yishang-virtual-machine:~/文档/文件IO$ ./a.out 
文件打开成功
实际写入：7个字节
写入内容：abcdefg
    
//执行以上内容，使11.txt内容变成1234567abcdefg
```

## 关闭文件:close

```c
CLOSE(2) 	Linux Programmer's Manual	 CLOSE(2)

NAME 名字
	close - 关闭一个文件描述符
	
SYNOPSIS 总览
	#include <unistd.h>
	
	int close(int fd);
	/*
        @fd : 需要关闭的文件描述符
        @return 成功返回0，失败返回-1
	*/
```

```c++
    close(file_id);
```

## -------------

## umask

**umask**，全称是 **User File Creation Mask**（用户文件创建掩码）。它是一个权限掩码，用于决定新创建的文件或目录的**默认权限**。

简单来说：

- **文件** 的默认最大权限是 `666` （即 `rw-rw-rw-`，读写权限，没有执行权限）。
- **目录** 的默认最大权限是 `777` （即 `rwxrwxrwx`，读、写、执行权限）。

`umask` 的作用就是从这些最大权限中“扣除”或“屏蔽”掉一些权限，从而得到实际的默认权限。

**核心逻辑：最终权限 = 最大默认权限 - umask 值**

------

### 计算方式

`umask` 值通常是一个三位或四位的八进制数（例如 `022`, `002`, `077`）。最常用的是三位。

我们以最常见的 `umask 022` 为例，看看它是如何计算出一个新文件和目录的权限的。

**a) 对于文件（最大权限 `666`）:**

1. 将最大权限 `666` 和 umask 值 `022` 都转换为二进制（方便理解按位操作）：
   - `666` -> `110 110 110`
   - `022` -> `000 010 010`
2. 进行 **“与非”** 操作（`AND NOT`）。实际计算中，更简单的方法是使用**减法**：
   - `666` (rw-rw-rw-)
   - `-022` (----w--w-)
   - `=644` (rw-r--r--)

所以，创建的新文件权限是 `644`（**所有者可读可写，所属组和其他人只读**）。

**b) 对于目录（最大权限 `777`）:**

1. 同样进行转换：
   - `777` -> `111 111 111`
   - `022` -> `000 010 010`
2. 进行减法：
   - `777` (rwx rwx rwx)
   - `-022` (---- w-- w-)
   - `=755` (rwx r-x r-x)

所以，创建的新目录权限是 `755`（**所有者可读可写可进入(执行)**，**所属组和其他人可读可进入**）。



**注意： `umask` 在代码中使用不会影响系统的设定，并且 `open` 创建的权限是全部都有。**

**重要提示**：文件默认没有执行权限（`x`）是出于安全考虑。你不能偶然创建一个自动就可执行的文件。目录则需要执行权限（`x`）才能进入（`cd`）该目录。

```
UMASK(2) 	Linux Programmer's Manual 	UMASK(2)
NAME
    umask - set file mode creation mask
    
SYNOPSIS
    #include <sys/types.h>
    #include <sys/stat.h>
    
    mode_t umask(mode_t mask);
    /*
        @描述：
       		指定umask的掩码值
        @mask：
        	指定文件的掩码
        @return：
        	返回上一个文件的掩码
    */
```

```c++
yishang@yishang-virtual-machine:~/文档/文件IO$ umask 0022
yishang@yishang-virtual-machine:~/文档/文件IO$ umask
0022
yishang@yishang-virtual-machine:~/文档/文件IO$ touch 0777 12.txt
yishang@yishang-virtual-machine:~/文档/文件IO$ ls -l
总计 44
-rw-r--r-- 1 yishang yishang     0  9月 14 17:04 0777
-rwxrwxr-x 1 yishang yishang    14  9月 14 16:20 11.txt
-rw-rw-r-- 1 yishang yishang     0  9月 14 17:04 12.txt
-rw-rw-r-- 1 yishang yishang     0  9月 14 14:54 1.txt
-rwxrwxr-x 1 yishang yishang 16792  9月 14 16:20 a.out
-rw-rw-r-- 1 yishang yishang   760  9月 14 17:02 lseek.cpp
-rw-rw-r-- 1 yishang yishang   300  9月 14 15:40 open.cpp
-rw-rw-r-- 1 yishang yishang   327  9月 14 15:47 open_create.cpp
-rw-rw-r-- 1 yishang yishang   571  9月 14 15:57 read.cpp
-rw-rw-r-- 1 yishang yishang   691  9月 14 16:08 write.cpp
    
-rw-r--r-- 1 yishang yishang     0  9月 14 17:04 0777
-rw-rw-r-- 1 yishang yishang     0  9月 14 17:04 12.txt	   
```

# 作业

 实现一下文件之间的拷贝

-  相当于把A文件的内容拷贝到B文件（空内容）中

```c++
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<iostream>
#include<unistd.h>

int main()
{
    int fd_a = open("A.txt",O_RDONLY);// 以只读方式打开源文件A.txt
    if(fd_a == -1)//通常open成功打开返回的是值3
    {
        // 使用perror输出错误信息（包含错误原因）
        perror("文件打开失败");
        return -1;
    }
    std::cout<<"文件打开成功"<<std::endl;
	
    // 创建缓冲区用于存储从文件读取的数据
    // 缓冲区大小为2字节，初始化为0
    char buffer[2]={0};//bufffer用来存储从A.txt中读取的内容
    // 以只写方式打开目标文件B.txt，如果文件不存在则创建
    // 0666: 设置文件权限（rw-rw-rw-），实际权限会受到umask影响
    int fd_b = open("B.txt",O_WRONLY|O_CREAT,0666);

    int ret;//ret用来表示存储到的个数，可能为2(正常从A中读取两个字节)，可能为1(当文件总个数不是2的倍数的时候，为1表示最后读取的那个字节)，也可能为0(当文件总个数为2的倍数时，表示全部读取完了)
    
    while((ret = read(fd_a,buffer,sizeof(buffer)))>0)//每次从A.txt中最多读取2个字节的内容存储到buffer，ret表示存储的个数
    {
         // 使用ret确保只写入实际读取的字节数
        write(fd_b,buffer,ret);//将存储了这ret个 个数 的buffer内容写入fd_b中
    }
    close(fd_a);
    close(fd_b);
    return 0;
}
```





# 链接

### 核心比喻：文件和它的“名字”

你可以把**一个文件**想象成两个部分：
1.  **文件本身（inode+数据块）**：文件存储在硬盘上的实际数据块。这对应一个叫 **inode** 的数据结构。inode 是文件的“身份证”，记录了文件的元数据（权限、所有者、大小、创建时间、**数据块位置**等），但不包含文件名。
2.  **文件名**：我们人类用来标识和访问文件的一个标签。

**目录** 本质上就是一个表格，它将我们使用的**文件名**映射到系统内部的 **inode 号码**。

**软链接**可以理解为Windows中的快捷方式，**硬链接**可以理解为在源文件基础上复制的新文件。

![屏幕截图 2025-09-14 133235](D:\Typora\typora_work\3文件IO\屏幕截图 2025-09-14 133235.png)

---



### 1. 软链接（又称符号链接）(Soft Link)

#### 是什么？(含义)
**软链接（Soft Link 或 Symbolic Link）更像 Windows 中的“快捷方式”**。它是一个独立的文件，这个文件的内容很特殊——它存储的是**另一个文件的路径**。

#### 如何创建？(语法)
使用 `ln` 命令时加上 `-s`（symbolic）参数：
```bash
ln -s <源文件/目录> <链接名>
```
示例：
```bash
ln -s original.txt softlink.txt
ln -s /some/long/path/my_dir shortcut_to_dir
```

#### 特点与行为：
*   **依赖性**：软链接依赖于源文件。如果你删除了源文件，软链接就会变成“断链”或“悬空链接”（`dangling link`），点击它时会报错`“No such file or directory”。`
*   **可以跨文件系统**：因为它存储的是路径字符串，所以可以轻松链接到不同分区或甚至网络共享（`NFS`）上的文件。
*   **可以链接目录**：这是软链接一个非常常见的用途，比如 `/var/www/html` 可能是指向 `/home/user/www/` 的一个软链接。
*   **权限不同**：软链接有自己独立的权限（通常是 `lrwxrwxrwx`，所有用户都有读、写、执行权限），但最终的操作权限由它指向的源文件决定。
*   **inode 不同**：软链接是一个独立的文件，所以它有自己的 inode 号和数据块（里面存着目标路径）。

#### 示例：

```bash
ln -s B A
#为源文件B创建一个软链接A(给B创建一个快捷方式A)
```

- 文件A和文件B inode 节点虽然不一样，但是文件A的内容就是文件B的路径 
- 读取文件A的时候，系统会自动将访问者导向文件B 
- 因此，无论打开文件A还是打开文件B，最终读取到的内容都是文件B的内容 
- 这种时候/情况，**文件A称为文件B的 软链接** ( `soft Link `) 
- 意味着文件A指向了文件B，删除文件A是不会对文件B有影响的，但是**如果删除文件B就会对文件A 产生影响**，文件A无法打开(依赖性)

![屏幕截图 2025-09-14 120540](D:\Typora\typora_work\3文件IO\屏幕截图 2025-09-14 120540.png)

---

### 2. 硬链接（Hard Link）

#### 是什么？(含义)

**硬链接（Hard Link）就是一个文件名指向同一个 inode 的另一个入口。** 你可以把它理解为文件的“别名”或“备用名”。

创建硬链接后，这两个（或多个）文件名完全平等，都直接指向硬盘上的同一份数据。

#### 如何创建？(语法)

使用 `ln` 命令（不加 `-s` 参数）：

```bash
ln <源文件> <链接名>
#ln B A
#为源文件创建一个硬链接
```

示例：

```bash
ln original.txt hardlink.txt
```

现在，`original.txt` 和 `hardlink.txt` 指向同一个 inode 和同一份数据。

#### 特点与限制：

*   **等同性**：所有硬链接的地位完全相同。删除任何一个（包括最初的那个“源文件”），只要还有至少一个硬链接存在，文件的数据就不会被真正删除。只有当一个文件的**所有硬链接都被删除**后，系统才会释放其 inode 和数据块空间。
*   **无法跨文件系统**：因为 inode 编号只在同一个文件系统内是唯一的。你不能为一个在 `/dev/sda1` 上的文件创建一个指向 `/dev/sda2` 的硬链接。
*   **无法链接目录**：为了防止在目录树中创建循环引用，导致系统工具（如 `find`, `tar`）陷入无限循环，绝大多数系统不允许普通用户创建目录的硬链接（超级用户可能可以，但极其不推荐）。
*   **inode 相同**：你可以用 `ls -i` 命令查看文件的 inode 号，会发现硬链接和源文件的 inode 号完全相同。

#### 示例：

```bash
ln B A
#为源文件B创建一个硬链接A
```

- 一般情况下，文件名和 inode 节点是一一对应的关系，也就是每一个 inode 节点对应一个文件名 

- 但是 linux 系统允许多个文件名指向同一个 inode 节点，意味着可以用不同的文件名访问相同的 内容

- 对于这个文件内容的进行修改，会影响其他文件名访问那个文件内容，但是**删除一个文件名，不影响其他文件名的使用的**。这个链接称为 **硬链接**

![屏幕截图 2025-09-14 121302](D:\Typora\typora_work\3文件IO\屏幕截图 2025-09-14 121302.png)

---

==注意：不要在共享目录下使用，因为window没有链接文件==

### 对比表格

| 特性             | 硬链接                                   | 软链接（符号链接）                          |
| :--------------- | :--------------------------------------- | :------------------------------------------ |
| **本质**         | 是同一个 inode 的另一个名字              | 是一个存储了路径的特殊文件                  |
| **inode 号**     | 与源文件相同                             | 与源文件**不同**（自己是新文件）            |
| **跨文件系统**   | **不支持**                               | **支持**                                    |
| **链接目录**     | **不支持**（通常）                       | **支持**                                    |
| **删除源文件**   | **不影响**，数据仍可通过其他硬链接访问   | **失效**，变成“断链”                        |
| **文件大小**     | 与源文件相同（他们本就是同一个文件）     | 很小（只存储目标路径的字符长度）            |
| **权限**         | 始终与源文件一致（他们本就是同一个文件） | 通常是 `777`，但实际权限由源文件决定        |
| **`ls -l` 显示** | 看起来像普通文件（无特殊标识）           | 显示为 `lrwxrwxrwx`，并用 `->` 指出目标路径 |

### 实际应用场景

*   **硬链接**：
    *   **日志轮转（Log Rotation）**：系统工具（如 `logrotate`）经常使用硬链接。它先创建一个当前日志文件的硬链接作为备份，然后清空原始文件继续写入。这样既保证了备份日志的完整性（数据不变），又不影响新日志的写入，且没有复制数据的开销。
    *   **节省空间备份**：一些备份工具（如 `rsync`）可以用 `--link-dest` 选项创建硬链接，来避免重复备份没有更改过的文件。

*   **软链接**：
    *   **软件版本管理**：例如，`/usr/bin/python` 可能是指向 `/usr/bin/python3.9` 的软链接。要切换默认版本时，只需更改链接指向的目标即可。
    *   **简化长路径**：为深层目录创建一个在主目录下的软链接，方便快速访问。
    *   **动态库管理**：`/usr/lib` 目录下，`libc.so.6` 通常是指向 `libc-2.31.so` 等具体版本库文件的软链接。

### 如何查看和删除链接？

*   **查看**：使用 `ls -l` 命令。软链接会明确显示出来。
    ```bash
    $ ls -li
    1024 -rw-r--r-- 2 user user 123 Apr 10 10:00 original.txt
    1024 -rw-r--r-- 2 user user 123 Apr 10 10:00 hardlink.txt    # inode相同
    1025 lrwxrwxrwx 1 user user  12 Apr 10 10:01 softlink.txt -> original.txt
    ```
    （第一列是 inode 号）

*   **删除**：**无论是硬链接还是软链接，都用 `rm` 命令删除**。
    
    ```bash
    rm hardlink.txt
    rm softlink.txt
    ```
    删除链接本身，不会影响其他硬链接或源文件（除非这是最后一个硬链接）。

