# 3种System V进程间通信通信

3种System V进程间通信通信：**共享内存**、**信号量**、**消息队列**

# 共享内存

共享内存是进程间通信一种方式，多个进程共享一段内存，“ **共享内存** ”。由于多个进程共享了同一段内 存,这个段内存既是你的也是我的。也就是你往这个内存里面写入数据，实际上就相当于往我的内存里面 写入数据。比起其他 `IPC` 方式（ `pipe fifo message..` ）少拷贝操作，相对而言共享内存的效率高于 其他的。

共享内存的生存期：**随内核持续性**

实现方式：

- 在内核中开辟一块共享内存，其他的进程通过 “ **映射** ” 方式，获取这个段共享内存的引用（指针）

- 进程 `P1` 可以映射这段内存，同时其他的进程（如： `P2`..`Pn` ）也可以映射这段内存， `P1` 往内存里 面写入数据，实际就是往 `P2`..`Pn` 进程中写入数据，反之亦然。

`System V` 和 `POSIX` 两套方法

# System V 共享内存的API

`System V`(`msg/shm/sem`)操作流程：

### 创建或者是打开一个IPC 设施ftok() - 预定餐馆（key

`ftok` ：创建或者是打开一个 `IPC` 设施( `msg/shm/sem` )也就是一个 `System V` `IPC` 对象的" **钥 匙** "（ `KEY` ）

```c++
FTOK(3) 			Linux Programmer's Manual
	FTOK(3)
NAME
	ftok - convert a pathname and a project identifier to a System V IPC key
SYNOPSIS
    
#include <sys/types.h>// 一个基础系统数据类型定义头文件
#include <sys/ipc.h>// 提供 IPC 的通用基础设施
    
// key_t :IPC 键类型（在 <sys/ipc.h> 和 <sys/types.h>中 都有定义）
key_t ftok(const char *pathname, int proj_id);
/*
    @描述
    	创建或者是打开一个IPC设施(msg/shm/sem)也就是一个System V IPC对象的"钥匙"（KEY
    @pathname：
    	一个在文件系统中路径名（这个路径必须要存在）
    @proj_id:
    	int类型整数，一般用工程的代号。
    @return：
        成功生成一个唯一的System V的IPC设施的KEY
        失败返回-1，同时errno被设置
*/
```

### 创建或打开System V 共享内存shmget() -实际订座(ID

`shmget` ：通过 `ftok` 获取到的 `IPC` 设施的钥匙来创建或打开一个 `System V` 共享内存

```c++
SHMGET(2)  		Linux Programmer's Manual
SHMGET(2)
NAME
	shmget - allocates a System V shared memory segment
SYNOPSIS
    
#include <sys/ipc.h>// 提供 IPC 的通用基础设施
#include <sys/shm.h>// 提供共享内存特定的功能
    
int shmget(key_t key, size_t size, int shmflg);
/*
    @描述：
    	shmget 用来在内核中创建或打开一个SystemV的共享内存
    @key：
    	System V的IPC设施的KEY，一般由ftok返回
    @size：
    	字节单位，指定共享内存区域的字节大小。如果是创建必须指定，如果是打开可设置size为0
    @shmflg：
        标志位：
            1）创建：IPC_CREAT | 权限位
            2）打开：0
    @return：
        成功返回共享内存区域的ID号，这个ID就唯一标识了这个共享内存
        失败返回-1，同时errno被设置
*/
```

### 映射/解映射 shmat,shmdt -其他人来到该桌

**映射**：把内核或者设备的文件中的一段内存**映射到**进程的地址空间去，用**进程的一个指针**，去访问这段 **内存**。

#### 1. `shmat` - 共享内存连接（映射）函数

`shmat` ：通过拿到的共享内存 ID 映射共享内存  --》》连接（attach）

**功能**：将 `shmget` 创建的共享内存段映射到调用进程的地址空间中，使得进程可以像访问普通内存一样读写这块共享区域。

```c++
SHMOP(2)          Linux Programmer's Manual
SHMOP(2)
NAME
	shmat, shmdt - System V shared memory operations
SYNOPSIS


#include <sys/types.h>//一个基础系统数据类型定义头文件
#include <sys/shm.h>// 提供共享内存特定的功能
    
void *shmat(int shmid, const void *shmaddr, int shmflg);
/*
    @描述：
    	用来映射一段System V的共享内存
    @shmid:
    	要映射的那个共享内存的id号，一般由shmget返回,
    	用来 指定你要连接哪一块共享内存。
    @shmaddr：
        指定要映射到的进程的哪个地址上去
        一般为NULL，由系统自个决定。
    @shmflg：
        标志位：
            SHM_RDONLY 只读，如果没设置这个标志，则默认为读写权限
            读写：0 --》》表示使用默认行为（读写权限，并由系统选择地址）
    @return：
        成功返回一个 void * 类型的指针，指向共享内存段在当前进程地址空间中的起始地址
        返回映射成功的首地址，
        失败返回NULL，同时errno被设置。
*/
```

#### 2. `shmdt` - 共享内存断开连接（解映射）函数

`shmdt`:---》》断开（detach）

**功能**：将之前通过 `shmat` 映射的共享内存段从当前进程的地址空间中分离（解映射）。

```c++
#include <sys/types.h>//一个基础系统数据类型定义头文件
#include <sys/shm.h>// 提供共享内存特定的功能

int shmdt(const void *shmaddr);
/*
    @描述：
    	解除一段共享内存的映射
    @shmaddr：
    	需要解除映射的内存首地址。是shmat 成功返回的地址
    @return：
    	成功返回0，失败返回-1，同时errno被设置
 */
```

- `shmdt` **并不会删除**共享内存段本身。它只是断开了进程与该段内存的连接。
- 共享内存段的生命周期是由 `shmctl` 函数（特别是 `IPC_RMID` 命令）控制的。即使所有进程都断开了连接，只要没有被显式删除，该内存段依然存在，直到系统重启。
- 进程终止时，会自动断开所有已连接的共享内存（相当于自动调用了 `shmdt`）

`<sys/types.h>` 是一个**基础系统数据类型定义头文件**

### 其他操作 shmctl

`shmctl` ：对于共享内存的操作的。

```c++
SHMCTL(2) Linux Programmer's
Manual SHMCTL(2)
NAME
shmctl - System V shared memory control
SYNOPSIS
#include <sys/ipc.h>
#include <sys/shm.h>
int shmctl(int shmid, int cmd, struct shmid_ds *buf);
/*
    @描述：
    	对于共享内存的操作
    @shmid：
    	需要进程控制操作的共享内存id号
    @cmd：
    	command 操作命令，不同的命令第三个参数是不一样的。
    IPC_RMID:
        删除指定的共享内存区域
        如果命令为IPC_RMID的话，第三个参数为：NULL
    @buf：
    	由第二个参数来决定第三个参数
    @return：
    	成功返0，失败返-1，同时errno被设置。
*/
```

### 示例：共享内存_W.cpp shm_w.cpp

```c++
#include<iostream>
#include <sys/types.h>//一个基础系统数据类型定义头文件
#include<sys/ipc.h>//提供 IPC 的通用基础设施
#include <sys/shm.h>// 提供共享内存特定的功能

#define PATHNAME "/home/yishang"
#define PROJECT 20241106
int main()
{
    //创建IPC设施
    key_t ipc_shm_key = ftok(PATHNAME,PROJECT);
    if(ipc_shm_key == -1)
    {
        perror("ftok1");
        return -1;
    }

    // 创建共享内存
    int ipc_shm_id = shmget(ipc_shm_key,4,0);
    if(ipc_shm_id == -1)
    {
        perror("shmget失败2");
        return -1;
    }

    std::cout << "共享内存创建成功，ID: " << ipc_shm_id << std::endl;

    //映射共享内存到进程地址空间
    int *ipc_shm_ptr = (int*)shmat(ipc_shm_id,NULL,0);
    if(ipc_shm_ptr == (int*)-1)
    {
        perror("shmat失败");
        shmctl(ipc_shm_id, IPC_RMID, NULL);
        return -1;
    }
    //初始化共享内存区域
    *ipc_shm_ptr = 0;
    std::cout<<"写数据："<<std::endl;
    while(1)
    {
        std::cin>>*ipc_shm_ptr;
        if(*ipc_shm_ptr == -1)
        {
            break;
        }
    }

    //解除共享内存映射
    shmdt(ipc_shm_ptr);

    //删除共享内存
    shmctl(ipc_shm_id,IPC_RMID,NULL);
    std::cout << "共享内存已清理" << std::endl;

    return 0;
}
```

### 示例：共享内存_R.cpp shm_r.cpp

```c++
#include<iostream>
#include <sys/types.h>//一个基础系统数据类型定义头文件
#include<sys/ipc.h>//提供 IPC 的通用基础设施
#include <sys/shm.h>// 提供共享内存特定的功能

#define PATHNAME "/home/yishang"
#define PROJECT 20241106
int main()
{
    //创建IPC设施
    key_t ipc_shm_key = ftok(PATHNAME,PROJECT);
    if(ipc_shm_key == -1)
    {
        perror("打开失败1");
        return -1;
    }

    // 打开共享内存
    int ipc_shm_id = shmget(ipc_shm_key,4,0);
    if(ipc_shm_id == -1)
    {
        perror("打开失败2");
        return -1;
    }
    
	std::cout << "共享内存创建成功，ID: " << ipc_shm_id << std::endl;
    
    //映射共享内存到进程地址空间
    int *ipc_shm_ptr = (int*)shmat(ipc_shm_id,NULL,0);
	if(ipc_shm_ptr == (int*)-1)
    {
        perror("shmat失败");
        shmctl(ipc_shm_id, IPC_RMID, NULL);
        return -1;
    }
    //读共享内存区域
    std::cout<<"读数据："<<std::endl;
    int old_num = 0;
    while(1)
    {
        if(old_num != *ipc_shm_ptr)//该内容不能与上次内容相同，如果相同不打印
        {
            std::cout << *ipc_shm_ptr<<std::endl;
            old_num = *ipc_shm_ptr;
        }
        if(*ipc_shm_ptr == -1)
        {
            break;
        }
    }

    //解除共享内存映射
    shmdt(ipc_shm_ptr);
    return 0;
}
```

# 共享内存需要用到的三个头文件

| 头文件          | 主要作用             | 关键内容                                        |
| :-------------- | :------------------- | :---------------------------------------------- |
| `<sys/types.h>` | **基础系统类型定义** | `pid_t`, `uid_t`, `off_t`, `size_t`, `key_t` 等 |
| `<sys/ipc.h>`   | IPC 基础架构         | `ftok()`, `IPC_CREAT`, 通用 IPC 命令            |
| `<sys/shm.h>`   | 共享内存具体实现     | `shmget()`, `shmat()`, `shmdt()`, `shmctl()`    |

# ==--------------==

# 信号量 semaphore

有两个以上的任务（进程/线程）并发的实体，去访问同一个共享资源（硬件上，软件上）的时候，那么 要保证访问的这个共享资源是有序访问，如果不是有序访问有可能造成不可预知后果。

````c++
// P1：
void readContent()
{
    // 1.txt
    char buffer[1024]={0};
    read(fd,buffer,1024);
    ....
}
// P2：
void readContent()
{
    // 1.txt
    char buffer[1024]={"123"};
    write(fd,buffer,3);
    ....
}
````

有两个实例（任务），调用 `readContent` 函数，那么读出最后的值是多少。有可能是 123 也有可能是其 他值。

其他值不是我们想要的，所以我们要保证多个实例能够有序的去访问，就需要对共享资源进行某种保 护，以便实例可以有序的访问，避免竞争

分析： 

- 并发---->竞争---->共享资源的非法访问 ---->程序行为异常...

解决方法：

-  能不能不用并发？
  -  显然不行 
- 在保留并发前提下，“ **避免竞争** ” ===> 访问共享资源的时候，严格串行！！！！

## 信号量机制

- 信号量是个什么玩意？ 
- 信号量的作用是什么？
- 为什么要用到信号量？ 
- 信号量是怎么达到目的的？

### 信号量是个什么玩意？

信号量（`semaphore`）是一种用于提供不同进程的间或一个进程内部不同线程间的**同步**的一种机制。

核心机制：**计数器**

- 进程/线程：任务，并发的实体 。
- 同步：并发实体间，相互等待相互约束的，有序的，有条件的访问。

信号量就是为了保护共享资源，让共享资源有序的访问的一种**机制**。

- **共享资源**：任何可以被多个进程或线程同时使用的东西，比如一块共享内存、一个文件、一个硬件设备等。
- **核心问题**：如果多个进程/线程不加控制地同时访问一个共享资源，可能会导致数据错乱、结果不一致等问题（这被称为**竞态条件**）。

**信号量目标：为了保护共享资源，使其能够被有序访问。**信号量就是解决这个问题的“交通警察”。

信号量是我们程序界最高尚的一种东西，因为它不是为了自己存在而存在，是为了别人而存在的。（它 保护的对象，共享资源）“ **保镖** ”

### 什么时候使用信号量？

- 有保护对象的时候，才需要信号量
- 首先搞清楚，谁需要保护，保护谁？
-  一个被保护对象，需要一个信号量。

### 如何来保护？

“ **保护** ”是指，让这个被保护对象（ 共享资源 ）有序的访问。如： **同步**（协调执行顺序）和**互斥**

- **共享资源**：任何可以被多个进程或线程同时使用的东西，比如一块共享内存、一个文件、一个硬件设备等。【大家都访问的资源。】

信号量机制，其实是程序员之间的一种**约定**，用来保护共享资源的。比如说进程A和进程B，都要访问一 个互斥设备，那么我们可以使用一个信号量来表示能不能访问该设备，然后每个进程访问该设备的时 候，先去访问信号量，如果能访问设备就把信号量设置为“ `NO` ”，访问完毕之后再将信号量设置为 “ `YES` ”。

在访问共享资源的时候，先去判断，共享资源是否能够访问。

- 能访问：你就获取到了该信号量（ 变成不可访问 ），则进入能访问之后的代码。 
- 不能访问： `wait` 直到信号量变成：能访问。

访问共享资源的代码区域叫做：**临界区**

- `LOCK` **上锁** 
  - **操作共享资源的代码** 
- `UnLOCK` 解锁

### 信号量是如何实现的？

“ 信号量 ”：大家都可以访问的一个整数。

一个**进程/线程**可以在某个信号量上执行以下三种操作：

- 创建 ( `create` ) 一个信号量：这还要求调用者指定信号量的初始值

  - 初始值表示该信号量保护的共享资源，可以同时被多少个任务访问。
  - `sem --> =5` 表示此刻有5个进程或者线程去同时访问它所保护的共享资源。
  - `sem -->= 1` 表示此刻有一个进程或者线程可以去访问它所保护的共享资源。
  -  “ **互斥信号量** ”

- 等待（ `wait` ）一个信号量

  - 1. 当一个进程想要使用共享资源时，它需要先检查(测试)信号量的值。如其值 <= 0，那么会等待( **阻塞** )。

  - 2. 一旦信号量的值大于0，表示资源可用。进程会将信号量的值**减1**（这个操作通常被称为 **P操作** 或 **等待**），然后去使用资源，`继续往下执行临界区代码`。

  - 3. 如果信号量的值等于0，表示资源正在被占用。这个进程必须**睡眠等待**，直到有其他进程释放资源。

  - 4.  当一个进程用完资源后，它会对信号量执行 **V操作**（或叫 **发信号**），将信号量的值**加1**。如果有其他进程在等待，这个操作会唤醒其中一个。

  - 其函数实现如下	

    ```c++
    while(semaphore_value <= 0)
    {
    	// wait block .... 卡死在这里
    }
    semaphore_value--; // 表示该进程/线程获取到了信号量
    // 下面的代码就是对于获取到该信号了之后，对共享资源的操作。
    ```

  - 上述操作必须是 “ 原子操作 ”：不允许有两个及以上的进程同时操作。

  - P操作： `proberen` （ 尝试 ）荷兰语

    -  `down/lock` 上锁

- 释放一个信号量：该操作将信号量的值+1

  4. 当一个进程用完资源后，它会对信号量执行 **V操作**（或叫 **发信号**），将信号量的值**加1**。如果有其他进程在等待，这个操作会唤醒其中一个。

  - 其函数实现类型如下

    ```c++
    semaphore_value++; // 原子操作
    ```

  - V操作： `verhogen` （ 增加 ）荷兰语 

    - `up / unlock` 解锁

信号量保护的目标是通过如下方式实现：

- 在临界区的**前面**加上一个：**P操作** 
- 在临界区的**后面**加上一个：**V操作**

# Linux内核信号量的具体实现： 

1. **`System V` 信号量**
   - **历史悠久**，是 UNIX 系统遗留下来的。
   - 功能强大但接口复杂。
   - 常用于**进程间**的同步。
2. **`POSIX` 信号量**
   - **现代标准**，接口更简单、更清晰。
   - 分为两种：
     - **有名信号量**：通过一个名字（通常是文件系统路径）来标识，可以用于**不相关进程**之间的同步。
     - **无名信号量**（基于内存的信号量）：存在于内存中（通常是在共享内存区），主要用于**线程之间**或**相关进程**（如父子进程）之间的同步。

**在现代编程中，除非有遗留代码需要维护，否则推荐使用 `POSIX` 信号量。**



# System V信号量 的大概流程

- ## `ftok` ：获取 `System V IPC` 设施对象的 `key` 

- ## `semget` ：在内核中创建或打开一个 `System V` 信号量

- ## `P/V` 操作

计数信号量集（计数信号量数组）：	

- 计数信号量： 
  - 该信号量的值可以是> 1 的值，它所保护共享资源允许多个任务同时访问它。 
  - 计数值 1 , 0 ===> 互斥信号量
- 互斥信号量：
  - 该信号量的值要么是1，要么是0，它所保护的共享资源同一时刻只能允许一个任务访问。

为什么`System V`要把信号量弄成一个信号量集(信号量数组)呢？

- `P(S1 & S2)` 的这种情况...

# 信号量需要用到的三个头文件

| 头文件          | 主要作用             | 关键内容                                        |
| :-------------- | :------------------- | :---------------------------------------------- |
| `<sys/types.h>` | **基础系统类型定义** | `pid_t`, `uid_t`, `off_t`, `size_t`, `key_t` 等 |
| `<sys/ipc.h>`   | IPC 基础架构         | `ftok()`, `IPC_CREAT`, 通用 IPC 命令            |
| `<sys/sem.h>`   | 信号量具体实现       | `semget()`, `semop()`,`semctl()`                |

# SystemV IPC 信号量API 函数

==参见open()函数==

 [socket.md](..\5网络编程\1.1socket套接字\socket.md) 

## `semget` ：用来创建或打开一个`System V`信号量集

```c++
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int semget(key_t key, int nsems, int semflg);
/*
    作用：
    	用来创建或打开一个System V信号量
    @key：
    	System V IPC设施的key，由ftok返回或使用 IPC_PRIVATE
    @nsems：
        要创建的信号量集中的信号量的数量
        如果我们不是创建而是打开一个已经存在的信号量集，此处参数可以为0，一旦创建完成一个信号量集，其信号量的个数就不能改变了。
    @semflg：
        标志位：
            创建：IPC_CREAT | 权限位
            打开：0
    @return：
        成功返回System V信号量集的id号
        失败返回-1，同时errno被设置
*/
```

**注意：在一个新创建的信号量集中的信号量的值，是不确定的。**

### flags参数

在 `semget()` 函数中，`semflg` 参数是一个**位掩码（`bitmask`）**，它通过控制信号量集的创建和行为，主要起到两方面作用：**决定是创建新信号量集还是获取已有的**，以及**设置访问权限**。

`semflg` 的标志主要分为以下两大类：

| 标志类型     | 标志位      | 作用                                                         | 说明                                                         |
| :----------- | :---------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| **创建控制** | `IPC_CREAT` | 如果指定的信号量集不存在，则创建它。                         | 如果对象已存在，则直接返回其ID。                             |
|              | `IPC_EXCL`  | 与 `IPC_CREAT` 共同使用，**确保原子性地创建一个全新的信号量集**。 | 如果信号量集已存在，调用失败并返回 `EEXIST` 错误。这与文件 `open()` 的 `O_CREAT | O_EXCL` 作用完全一致。 |
| **权限模式** | 权限位      | 定义所有者、同组用户和其他用户的**读（`SEM_R`）**和**改（`SEM_A`）**权限。 | 这是 `semflg` 参数中**最低有效的9个比特位**，其格式和含义与 `open()` 函数的 `mode` 参数类似。 |

此外，还有一个特殊的键值 **`IPC_PRIVATE`**，它**并非一个标志位**，而是用于 `key` 参数。使用它时，无论该键值是否已存在，系统都保证创建一个**全新的、私有的**信号量集。不过需要注意，它名为 "private"，但实际上其他进程如果知道这个信号量集的ID，仍然可以访问它，因此主要用于父子进程间的通信。

| 错误码名称 | 数值 | 含义                      | 常见触发场景                                      |
| ---------- | ---- | ------------------------- | ------------------------------------------------- |
| **EEXIST** | 17   | 文件已存在（File exists） | 使用 `O_CREAT` 和 `O_EXCL` 创建文件时文件已存在。 |

### 如何组合使用

- **示例1：创建一个新的信号量集，如果已存在则获取它**

```c
#include <sys/sem.h>
#include <sys/stat.h> // 包含权限常量

key_t key = 0x1234; // 假设的键值
int num_sems = 1;   // 信号量集中包含1个信号量
// 标志：创建（如不存在） + 权限（所有者读写，即 0600）
int semflg = IPC_CREAT | S_IRUSR | S_IWUSR;
int semid = semget(key, num_sems, semflg);
```

- **示例2：原子性地创建一个新的信号量集（保证之前不存在）**

```c
// 标志：排他性创建 + 权限（所有者读写，即 0600）
int semflg = IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR;
int semid = semget(key, num_sems, semflg);
if (semid == -1 && errno == EEXIST) {
    // 错误处理：信号量集已存在
    perror("semget error");
}
```

`IPC_CREAT | IPC_EXCL` 的组合确保了操作的原子性，常用于避免多个进程同时初始化同一个信号量集带来的竞争条件

### 注意

- **权限掩码**：`semflg` 中指定的权限（如 `0666`）会受进程的 `umask` 影响。最终生效的权限是 `(semflg & ~umask)`。通常建议在 `semflg` 中使用八进制数（如 `0640`）或 `S_I` 系列的宏来定义权限。
- **错误码**：当 `semget` 调用失败时，可以通过检查 `errno` 来获取具体原因，常见的如 `EACCES`（权限不足）、`ENOENT`（没有该信号量集且未指定 `IPC_CREAT`）、`EEXIST`（指定了 `IPC_EXCL` 但对象已存在）等

## semctl ：控制信号量

```c++
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int semctl(int semid, int semnum, int cmd, ...);
/*
    作用：
        控制操作，设置或获取信号量集中某个或者某些信号量的值
    @semid：
    	信号量集的id号，由semget返回
    @semnum：
    	要操作的信号量集中的哪个信号量，就是信号量数组的下标，信号量在集合中的索引【从 0 开始 到 nsems-1】
    @cmd：
        command 命令，操作信号量集
        常用命令：
            GETVAL：获取第semnum哪个对应的信号量的值
            SETVAL：设置第semnum哪个对应的信号量的值
            GETALL：获取这个信号量集中的所有信号量的值
            SETALL：设置这个信号量集中的所有信号量的值
            IPC_RMID：删除这个信号量集
            ...UUUUUUUUUUUUUUUUUUUU9IIII
    @...：
        针对不同的命令号，第四个参数不一样
        cmd == GETVAL
        	第四个参数不需要
        cmd == SETVAL
        	第四个参数表示位要设置的信号量的值
        cmd == GETALL
        	第四个参数表示所有信号量的值，为：unsigned short vals[]
        ...
    @return:
        根据命令不同，返回值的含义不一样
        如：
            GETVAL 返回信号量的值
            正常情况成功返回0，失败返回-1
*/
```

## `semop` : System V 信号量的 PV 操作

```c++
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int semop(int semid, struct sembuf *sops, size_t nsops);
/*
    作用：
    	System V信号量的PV操作
    @semid：
    	需要PV操作的信号量集的id号
    @sops：
    	用来描述System V的PV操作
        struct sembuf
        {
            unsigned short sem_num;	// 信号量在集合中的索引
            		需要操作的信号量下标
            short sem_op; // 操作类型 
                    >0:表示V操作 解锁
                    =0:表示try 尝试是否会阻塞
                    <0:P操作，上锁
                    semval(信号量的值) == 原semval + sem_op
            short sem_flg;// 操作标志
                    0:
                        默认，如果P操作做不了，则会阻塞：死等
                    IPC_NOWAIT:非阻塞,不等待
                        如果是P操作做不了，直接走人，能获取就获取。
                    SEM_UNDO:
                        撤销
                        为了防止进程带锁退出。
                        if you set
                        SEM_UNDO这个标志，内核会额外记录该进程对信号量的所有PV操作，
                        然后再该进程退出的时候，会还原操作。
                        如：
                            // 正常情况
                            P 	V 	P 	V 	kill
                            -1 	+1 	-1 	+1 	0
                            // 带锁退出
                            P 	V 	P 	kill
                            -1 	+1 	-1 	(+1)
        }；
    @nsops：
    	第二个参数的数量
        struct sembuf 是对于信号量集的PV操作，但是信号量集中不一定只有一个信号量，所以PV操作也不止一个。
    @return：
    	成功返回0，失败返回-1，同时errno被设置
*/
```

```c
struct sembuf
{
	unsigned short int sem_num;
    short int sem_op;
    short int sem_flg;
}
```

### `sem_flg`标志

`IPC_NOWAIT` 和 `SEM_UNDO` 和之前讨论的 `IPC_CREAT`、`IPC_EXCL` 不同，它们不是在**创建** `IPC` 资源（如 `semget()`）时使用的，而是在**操作**这些资源时使用的。

| 特性         | `IPC_NOWAIT` (非阻塞)                                        | `SEM_UNDO` (进程终止时还原)                                  |
| :----------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| **核心作用** | 让信号量操作变成**非阻塞**模式 。                            | 为操作提供“**事务回滚**”机制，防止资源死锁 。                |
| **使用场景** | 当你不想让程序因为拿不到资源而“卡死”时使用。                 | 当你想避免进程崩溃导致资源（如互斥锁）永远被锁住时使用。     |
| **失败行为** | 如果操作无法立即执行（例如申请的资源不足），`semop()` 调用**立即返回 -1**，并设置 `errno` 为 `EAGAIN` 。 | 进程终止时，内核会自动“撤销”该进程在此信号量上做的所有操作，恢复原状。 |

### 💡 深入理解两者的工作机制

#### 1. `IPC_NOWAIT`：非阻塞操作

这个标志让信号量操作不再“死等”。

- **当`sem_op`为负数（申请资源）时**：如果信号量的当前值小于你想要减少的值（即资源不足），设置了 `IPC_NOWAIT` 的 `semop()` 调用会立刻返回失败，而不是让进程进入睡眠状态等待资源 。
- **当`sem_op`为0（等待到0）时**：如果信号量的当前值不为0，`semop()` 也会立刻返回失败，不会一直等着它变为0 。

#### 2. `SEM_UNDO`：进程终止时还原

这是 System V 信号量提供的一个极其强大的“防呆”机制。它通过为每个进程维护一个名为 `semadj`（信号量调整值）的内核结构来实现 。

可以把 `SEM_UNDO` 想象成给每次信号量操作都绑定了一个“记账本”：

- **当你的进程成功**执行了一个带 `SEM_UNDO` 的 P 操作（申请资源，`sem_op` < 0），内核就会在你的“账本”（`semadj`）上记下“欠了`n`个资源”。
- **当你的进程成功**执行了一个带 `SEM_UNDO` 的 V 操作（释放资源，`sem_op` > 0），内核就会在你的“账本”上记下“归还了`n`个资源”。

当你的进程无论是因为正常退出还是意外崩溃而终止时，内核会拿出这个“账本”，对每个信号量执行“多退少补”的操作 ：

- **如果你“欠了”资源**（`semadj`为负），内核会自动帮你释放，相当于进程在临终前替你执行了 V 操作。
- **如果你“多还了”资源**（`semadj`为正），内核会自动收回，相当于进程在临终前替你执行了 P 操作。

**一个经典的应用场景是二值信号量用作互斥锁**：如果一个进程在锁定资源后突然崩溃，如果没有 `SEM_UNDO`，这个锁就永远无法释放了，其他进程会永远等下去。而有了 `SEM_UNDO`，内核会在进程崩溃后自动释放这把锁，让系统恢复正常 。

### 🔍 补充一点：`IPC_NOWAIT` 在其他 IPC 机制中

`IPC_NOWAIT` 标志并不仅限于信号量，它在消息队列的接收函数 `msgrcv()` 中也有同样的“非阻塞”含义。当消息队列为空时，如果指定了 `IPC_NOWAIT`，`msgrcv()` 会立刻返回错误，而不是阻塞等待 

````c++
int semtimedop(int semid, struct sembuf *sops, size_t nsops,
const struct timespec *timeout);
/*
    作用：
    	限时等待
    @semid：
    	需要限时等待的信号量集的id
    @sops：
    	用来描述System V的PV操作
    @nsops：
        第二个参数的数量
        struct sembuf 是对于信号量集的PV操作，但是信号量集中不一定只有一个信号量，所以PV操作也不止一个。
    @timeout：
    	超时时间：
    struct timespec
    {
        long tv_sec; // 秒数
        long tv_nsec;// 纳秒
            1s ==> 1000ms
            1ms ==> 1000us
            1us ==> 1000ns
    }；
    如：
        struct timespec tv;
        tv.tv_sec = 5; // 五秒
        tv.tv_nsec = 0;
*/
````



# 示例：SystemV_Sem

````c
#include <sys/types.h> // 类型需要的头文件
#include <sys/ipc.h>   // 是IPC设施需要的头文件 
#include <sys/sem.h>   // 是信号量需要的头文件
#include <unistd.h>
#include <iostream> 
#include <sys/wait.h>
 
#define PATHNAME "/home/yishang"
#define PROJECT  20241106
 
int main()
{
    FILE * file_ptr = fopen("1.txt","w+");
    pid_t pid = fork();
 
    if(pid > 0)
    {
        //创建IPC设施
        key_t key = ftok(PATHNAME,PROJECT);
 
        // 创建信号量集
        int sem_id = semget(key,1,IPC_CREAT|0777);
 
        // 初始化信号量
        semctl(sem_id,0,SETVAL,1);
 
        // P操作
        struct sembuf sem_ctl;
        sem_ctl.sem_num = 0;  // 信号量的下标
        sem_ctl.sem_op  = -1; // 上锁操作
        sem_ctl.sem_flg = SEM_UNDO; // 撤销机制
        semop(sem_id,&sem_ctl,1);
 
        fwrite("123",1,3,file_ptr);
        fflush(file_ptr);
        
        //休眠一秒
        sleep(1);
        // 解锁
        sem_ctl.sem_num = 0;  // 信号量的下标
        sem_ctl.sem_op  = 1;  // 解锁操作
        sem_ctl.sem_flg = SEM_UNDO; // 撤销机制
        semop(sem_id,&sem_ctl,1);
 
        wait(NULL);
        semctl(sem_id,0,IPC_RMID,NULL);
        fclose(file_ptr);
    }
    else
    {
        key_t key = ftok(PATHNAME,PROJECT);
 
        // 打开信号量集
        int sem_id = semget(key,1,0);
 
        // P操作
        struct sembuf sem_ctl;
        sem_ctl.sem_num = 0;  // 信号量的下标
        sem_ctl.sem_op  = -1; // 上锁操作
        sem_ctl.sem_flg = SEM_UNDO; // 撤销机制
        semop(sem_id,&sem_ctl,1);
 
        rewind(file_ptr);
        char buff[1024]={0};
        fread(buff,1,3,file_ptr);
        std::cout << buff << std::endl;
 
        // 解锁
        sem_ctl.sem_num = 0;  // 信号量的下标
        sem_ctl.sem_op  = 1;  // 解锁操作
        sem_ctl.sem_flg = SEM_UNDO; // 撤销机制
        semop(sem_id,&sem_ctl,1);
        fclose(file_ptr);
    }
 
    return 0;
}
````

# ==------------------==

# POSIX信号量见：

[5线程2.md](D:\Typora\typora_work\4多线程与并发\5线程2.md)

# ==------------------==

# `exec`函数簇

==参见==

[socket.md](..\5网络编程\1.1socket套接字\socket.md) 

`exec`函数簇是一组函数，用于执行一个程序并替换当前进程的映像，这些函数属于`unistd.h`头文件中定义的`POSIX`标准，当调用`exec`函数成功时，当前进程的代码段、数据段、堆栈等将**被新程序**的相应部分**替换**，然后从新程序的入口点开始定义，进程ID不会改变，但进程的内存空间和属性将发生改变。

是一组用于执行新程序的UNIX/Linux系统调用，它们会用新的程序映像替换当前进程的代码段、数据段、堆和栈。

```c++
#include <unistd.h>

int execl(const char *path, const char *arg, ...);
int execlp(const char *file, const char *arg, ...);
int execle(const char *path, const char *arg, ..., char *const envp[]);
int execv(const char *path, char *const argv[]);
int execvp(const char *file, char *const argv[]);
int execvpe(const char *file, char *const argv[], char *const envp[]);
```

## `exec`函数`API`

### 1.`exec`l函数：

```c++
int execl(const char *path, const char *arg, ...);
```

- 作用：用新的程序替换当前进程电脑内存空间

- 参数：

  - `pathname`:要执行的程序的路径。

  - `arg`:及后续可变参数：依次为新程序的命令行参数，以`NULL`指针结束。

  - ```c++
    execl("/bin/ls", "ls", "-l", "-a", NULL);
    ```

#### 示例：

##### 示例1

英雄联盟客户端（`lol_client`） 启动英雄联盟游戏（`lol_game`） 的过程 可以 模拟`execl`函数的使用。

客户端 通过创建子进程 （fork()之后，通过execl函数）来 启动游戏程序，游戏结束后返回客户端

lol_cilent.cpp

```c++
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <cstdlib>

using namespace std;

// 模拟客户端功能
class LoLClient {
private:
    string username;
    bool loggedIn;

public:
    LoLClient() : loggedIn(false) {}
    
    void login(const string& user) {
        username = user;
        loggedIn = true;
        cout << "🎮 英雄联盟客户端: 用户 " << username << " 登录成功!" << endl;
    }
    
    void showMainMenu() {
        cout << "\n=== 英雄联盟客户端 ===" << endl;
        cout << "1. 开始匹配游戏" << endl;
        cout << "2. 查看个人资料" << endl;
        cout << "3. 退出客户端" << endl;
        cout << "=====================" << endl;
    }
    
    void startMatchmaking() {
        if (!loggedIn) {
            cout << "❌ 请先登录!" << endl;
            return;
        }
        
        cout << "\n🔍 正在寻找对局..." << endl;
        sleep(2);
        cout << "✅ 对局已找到! 进入英雄选择..." << endl;
        sleep(1);
        
        // 创建子进程来启动游戏
        launchGame();
    }
    
    void launchGame() {
        cout << "\n🚀 正在启动英雄联盟游戏..." << endl;
        
        pid_t pid = fork();
        
        if (pid == 0) {
            // 子进程 - 启动游戏
            cout << "🎯 子进程创建成功，准备启动游戏..." << endl;
            
            // 使用execl启动游戏程序
            // 假设游戏程序名为 "lol_game"
            execl("./lol_game", "lol_game", "--player", username.c_str(), 
                  "--map", "summoners_rift", "--mode", "ranked", NULL);
            
            // 如果execl失败，执行到这里
            cerr << "❌ 启动游戏失败!" << endl;
            exit(1);
            
        } else if (pid > 0) {
            // 父进程 - 客户端继续运行
            cout << "📱 客户端继续运行在后台..." << endl;
            cout << "💬 可以继续聊天、查看好友列表等" << endl;
            
            int status;
            waitpid(pid, &status, 0);  // 等待游戏结束
            
            cout << "\n🎮 游戏已结束，返回客户端界面" << endl;
            showGameResult(status);
            
        } else {
            cerr << "❌ 创建子进程失败!" << endl;
        }
    }
    
    void showGameResult(int status) {
        if (WIFEXITED(status)) {
            int exitCode = WEXITSTATUS(status);
            if (exitCode == 0) {
                cout << "🏆 恭喜! 游戏胜利!" << endl;
            } else {
                cout << "😞 游戏失败，再接再厉!" << endl;
            }
        }
    }
    
    void showProfile() {
        cout << "\n📊 玩家资料:" << endl;
        cout << "用户名: " << username << endl;
        cout << "等级: 30" << endl;
        cout << "排位: 黄金 II" << endl;
    }
};

int main() {
    LoLClient client;
    
    // 模拟登录
    client.login("召唤师Pro");
    
    // 模拟客户端主循环
    while (true) {
        client.showMainMenu();
        
        cout << "\n请选择操作 (1-3): ";
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1:
                client.startMatchmaking();
                break;
            case 2:
                client.showProfile();
                break;
            case 3:
                cout << "👋 退出英雄联盟客户端..." << endl;
                return 0;
            default:
                cout << "❌ 无效选择!" << endl;
        }
    }
    
    return 0;
}
```

lol_game.cpp

```c++
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <vector>

using namespace std;

// 模拟游戏功能
class LoLGame {
private:
    string playerName;
    string map;
    string gameMode;
    bool gameRunning;

public:
    LoLGame(int argc, char* argv[]) : gameRunning(true) {
        // 解析命令行参数
        for (int i = 1; i < argc; i++) {
            string arg = argv[i];
            if (arg == "--player" && i + 1 < argc) {
                playerName = argv[++i];
            } else if (arg == "--map" && i + 1 < argc) {
                map = argv[++i];
            } else if (arg == "--mode" && i + 1 < argc) {
                gameMode = argv[++i];
            }
        }
        
        cout << "\n🎮 === 英雄联盟游戏启动 === 🎮" << endl;
        cout << "玩家: " << playerName << endl;
        cout << "地图: " << map << endl;
        cout << "模式: " << gameMode << endl;
    }
    
    void initializeGame() {
        cout << "\n⏳ 游戏初始化中..." << endl;
        sleep(1);
        cout << "✅ 加载英雄数据..." << endl;
        sleep(1);
        cout << "✅ 加载地图资源..." << endl;
        sleep(1);
        cout << "✅ 连接游戏服务器..." << endl;
        sleep(2);
        cout << "🎯 游戏准备就绪!" << endl;
    }
    
    void gameLoop() {
        cout << "\n⚔️  === 游戏开始 === ⚔️" << endl;
        
        int gameTime = 0;
        vector<string> events = {
            "First Blood! - 玩家击杀了敌方英雄",
            "玩家摧毁了敌方防御塔",
            "玩家获得了巨龙buff",
            "团战爆发! 玩家完成双杀",
            "玩家推上了高地",
            "Victory! 摧毁了敌方水晶"
        };
        
        while (gameRunning && gameTime < 6) {
            cout << "⏰ 游戏时间: " << (gameTime + 1) * 5 << "分钟" << endl;
            cout << "📢 " << events[gameTime] << endl;
            
            // 模拟游戏逻辑
            sleep(2);
            gameTime++;
            
            // 随机决定是否提前结束（模拟投降）
            if (gameTime == 3 && rand() % 10 < 2) {
                cout << "\n🏳️ 敌方队伍发起了投降投票..." << endl;
                sleep(1);
                cout << "✅ 投降通过，游戏结束" << endl;
                gameRunning = false;
                exit(1);  // 游戏失败退出
            }
        }
        
        if (gameRunning) {
            cout << "\n🏆 游戏胜利!" << endl;
            exit(0);  // 游戏胜利退出
        }
    }
};

int main(int argc, char* argv[]) {
    // 创建游戏实例
    LoLGame game(argc, argv);
    
    // 初始化游戏
    game.initializeGame();
    
    // 运行游戏主循环
    game.gameLoop();
    
    return 0;
}
```

##### 示例2

**hello.cpp**

```c++
#include<iostream>
#include<unistd.h>

int main()
{
    std::cout<<"我的进程id号为："<<getpid()<<std::endl;
    std::cout<<"请输入任意字符来调用我："<<std::endl;
    std::string data;
    std::cin>>data;
    std::cout<<"hello,欢迎!"<<std::endl;
    return 0;
}
```

**execl.cpp**

```c++
#include<unistd.h>
#include<iostream>
#include<sys/wait.h>

int main()
{
    std::cout << "我的进程号为："<<getpid() <<std::endl;//获取当前进程的ID号
    pid_t pid = fork();
    if(pid == 0)
    {
        //让子进程变成其他程序去执行
        //使用execl将当前进程(execl.cpp)的代码段、数据段、堆栈等
        // 被新程序(./A -源文件)的相应部分替换
        execl("./hello","hello","",NULL);//成功不返回值
        std::cout << "我返回了" <<std::endl;//失败才会执行该语句
    }
    wait(NULL);
    std::cout<<"我作为父进程退出了"<<std::endl;
    return 0;
}
```

![execl](D:\Typora\typora_work\4多线程与并发\execl.png)

### 2.`execlp`函数：

```
int execlp(const char *file, const char *arg, ...);
```

- 作用：与`execl`类似，但会在环境变量`PATH`中搜索可执行文件。

- 参数

  - `file`:要执行的程序的文件名

  - `arg`:及后续可变参数：依次为新程序的命令行参数，以`NULL`指针结束。（同`execl`）

  - ```c++
    execlp("ls", "ls", "-l", "-a", NULL);
    ```

### 3.execle函数：

- 作用：功能同`execl`,但可以自己指定环境变量

- 参数：

  - `pathname`:同`execl`

  - arg及后序可变参数：同`execl`

  - `envp[]`:环境变量数组，以`NULL`指针结束。

  - ```c++
    char *env[] = {"HOME=/tmp", "PATH=/bin", NULL};
    execle("/bin/ls", "ls", "-l", NULL, env);
    ```

### 4.execv函数:

```c++
int execv(const char *path, char *const argv[]);
```

- 作用：与`execl`类似，但参数以数组的形式传递。

- 参数

  - `pahtname`:要执行的程序的路径

  - `argv[]`:包含命令行参数的字符指针数组，以`NULL`指针结束。

  - ```c++
    char *args[] = {"ls", "-l", "-a", NULL};
    execv("/bin/ls", args);
    ```

### 5.execvp函数：

````c++
int execvp(const char *file, char *const argv[]);
````

- 作用：与`execv`类似，会在环境变量`PATH`中搜索可执行文件。

- 参数：

  - `file`:要执行的程序的文件名

  - `argv[]`:同`execv`.

  - ```c++
    char *args[] = {"ls", "-l", "-a", NULL};
    execvp("ls", args);
    ```

### 6.execvpe函数：

```c++
int execvpe(const char *file, char *const argv[], char *const envp[]);
```

- 作用：同`execvp`,但可以自己指定环境变量
- 参数：
  - `file`:要执行的程序的文件名
  - `argv[]`:同`execv`
  - `envp[]`:环境变量数组，以`NULL`指针结束。

**这些函数执行成功后不会返回，如果返回则表名执行失败了**

# 作业： 

- 设计一个 **售票系统** 
  - 可以有五个售票窗口(进程) 
  - 每售出一张票就把票信息输出 **已售文件** 中
  -  票的信息从 **代售文件** 中读取
