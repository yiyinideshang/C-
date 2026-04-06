# 汇华Linux操作系统及应用

改变目录`cd`，创建目录`mkdir`

# 相对路径，绝对路径

相对路径：从当前路径开始

绝对路径：从根目录开始

# `cd`

```bash
#直接输入cd,进入主目录
cd
```



# 外部命令，内部命令

Linux命令有内部（内建）（内置）命令和外部命令之分，内部命令和外部命令功能基本相同，但有些细微差别。

所谓的内部和外部其实是相对Shell自身来讲。Linux系统为了提高系统运行效率，将经常使用的轻量的命令在系统启动时一并加载这些命令到内存中供Shell随时调用，这部分命令即为内部命令。

反之，系统层调用的较重的命令只有当被调用时才会硬盘加载的这部分命令即为外部命令。

## 内部命令

- 内部命令包含在Shell中。

内部命令实际上是[**Shell程序**]的一部分，由 Shell 软件内部进行实现的命令，其中包含的是一些比较简单的`linux`系统命令，这些命令由shell程序识别并在shell程序内部完成运行，通常在`linux`系统加载运行时shell就被加载并驻留在系统内存中。内部命令是Shell本身的重要组成部分。内部命令嵌入在Shell程序中，并不单独以磁盘文件的形式存在于磁盘上。内部命令是写在bash源码里面的，其执行速度比外部命令快，因为解析内部命令shell不需要创建子进程，它们都运行在 Shell 进程当中。比如：`exit，history，help，echo，fg，cd、source、export、time`等。

## 外部命令

- 像文件一样存在与磁盘上。

外部命令是`linux`系统中的实用程序部分，是一个独立的外部可执行程序，因为实用程序的功能通常都比较强大，所以其包含的程序量也会很大，在系统加载时并不随系统一起被加载到内存中，而是在需要时才将其调用内存。通常外部命令的实体并不包含在Shell中，但是其命令执行过程是由Shell程序控制的。当外部命令被调用时，本质就是调用了另外一个程序，首先 Shell 会创建子进程，然后在子进程当中运行该程序。Shell程序管理外部命令执行的路径查找、加载存放，并控制命令的执行。外部命令是在bash之外额外安装的，通常放在/bin，/usr/bin，/sbin，/usr/sbin……等等。可通过“echo $PATH”命令查看外部命令的存储路径。常见外部命令比如：`/bin/ls、vi、tee、tar`等。

## 判断某个命令为外部命令或内部命令

- `type`命令用于显示指定命令的类型，判断给出的指令是内部指令还是外部指令。例如：

- `which` 命令的作用是在 `$PATH` 环境变量指定的目录中搜索**外部命令**（即可执行文件），并返回第一个匹配到的路径。

````bash
yishang@yishang-virtual-machine:~$ type -a ls
ls 是 "ls --color=auto" 的别名
ls 是 /usr/bin/ls
ls 是 /bin/ls
yishang@yishang-virtual-machine:~$ which ls
/usr/bin/ls
````

这说明：ls是一个**外部命令**，命令的路径为/usr/bin/ls。

```bash
yishang@yishang-virtual-machine:~$ type -a cd
cd 是 shell 内建
```

这说明：cd是shell内嵌命令。

# 查看命令的源代码：

解压好`coreutils-7.6`后进入该文件夹下的`src`文件夹

# `PuTTY`下载安装教程｜SSH远程登录配置与使用指南

# 绝对路径和相对路径

# 超级终端`sudo`

# 拷贝文件`cp`

# 虚拟机：

桥接网络：想要和主机一样，有一个独立的IP地址，主机和虚拟机之间可以进行交流、

NAT网络：



硬盘（`SCSI`）

`CD`/`DVD`(`SATA`)

# 手动分区

- 主分区

- 扩展分区（从`sda5`开始）---由扩展分区继续分割出来的分区，被称为逻辑分区(Logical)
  - 逻辑分区

**手动分区时，遵循以下原则**：

| 类型                                        | 挂载点 | 分区大小       |
| ------------------------------------------- | ------ | -------------- |
| 引导分区 `/dev/sda1`                        | /boot  | `512M`         |
| 系统分区 `/dev/sda5`                        | /      | `10G`          |
| 交换分区 `/dev/sda6`（Windows上的虚拟内存） | swap   | 物理内存的两倍 |
| 个人文件分区 `/dev/sda7`                    | /home  | 根据情况分配   |

==第二章==

sun公司的`unix`操作系统：`Solaris` ---世界上最大的服务器、UNIX系统供应商、开发JAVA、UNIXc称为：`Solaris` ——河北师范大学购买的服务器

Linux操作系统诞生：1991 年10月 5日

Linux系统的四个主要部分：内核、shell、文件系统和应用程序。

# shell

# Linux文件系统

`EXT4`

# Linux 发行版本

- `Ubuntu`
- `Red Hat Linux`
- `Debian Linux` 
- `红旗 Linux`
- `CentOS`
- `kali`

# `uname`命令

- *-a* 或 *--all*：显示所有信息，包括内核名称、主机名、操作系统版本、处理器类型和硬件架构等。
- *-m* 或 *--machine*：显示处理器类型。
- *-n* 或 *--nodename*：显示主机名。
- *-r* 或 *--release*：显示内核版本号。
- *-s* 或 *--`sysname`*：显示操作系统名称。
- *-v*：显示操作系统的版本。

```bash
zhanghaoboo@zhanghaoboo-virtual-machine:~$ uname -a
Linux zhanghaoboo-virtual-machine 4.15.0-45-generic #48~16.04.1-Ubuntu SMP Tue Jan 29 18:03:48 UTC 2019 x86_64 x86_64 x86_64 GNU/Linux
zhanghaoboo@zhanghaoboo-virtual-machine:~$ uname -r
4.15.0-45-generic

```



# 创建用户

创建用户的命令

````bash
sudo adduser 用户名
````

设置密码：

```bash
sudo passwd testuser
```

# 删除用户

# 获取用户列表

```bash
cat /etc/passwd
```

# 转移当前工作用户

```bash
#转移到yishang用户的当前工作路径
su yihsang
#转移到yishang用户的主目录，-后面两个空格
su -  yishang

```

给root用户设置密码：

```bash
sudo passwd root
```

````bash
zhanghaoboo@zhanghaoboo-virtual-machine:/bin$ cd
zhanghaoboo@zhanghaoboo-virtual-machine:~$ sudo passwd root
[sudo] zhanghaoboo 的密码： 
输入新的 UNIX 密码： 
重新输入新的 UNIX 密码： 
passwd：已成功更新密码
zhanghaoboo@zhanghaoboo-virtual-machine:~$ su -  root
密码： 

root@zhanghaoboo-virtual-machine:~# 
root@zhanghaoboo-virtual-machine:~# su -  zhanghaoboo 
zhanghaoboo@zhanghaoboo-virtual-machine:~$ 
````

`root`用户想到哪个用户直接到，不需要密码

# 普通用户添加到root权限组

# 第五章

```bash
id 用户名
```

`gid`：基本组

`groups`:组、群组

```bash
/dev/sr0       iso9660  1625600 1625600       0  100% /media/zhanghaoboo/Ubuntu 16.04.6 LTS amd64
zhanghaoboo@zhanghaoboo-virtual-machine:/var/log$ id zhanghaoboo 
uid=1000(zhanghaoboo) gid=1000(zhanghaoboo) 组=1000(zhanghaoboo),4(adm),24(cdrom),27(sudo),30(dip),46(plugdev),113(lpadmin),128(sambashare)

```

- 这说明当前这个用户在 sudo群组中，可以执行sudo命令

```bash
zhanghaoboo@zhanghaoboo-virtual-machine:/var/log$ id user1 
uid=1001(user1) gid=1001(user1) 组=1001(user1)

```

# 文件权限

在 Linux 中，文件权限通常分为三组：**拥有者 (User)**、**所属组 (Group)**、**其他用户 (Other)**，每组权限由 **r(读)**、**w(写)**、**x(执行)** 三个标志组成。

如:`rw- rw- r-- `   这种权限组合的八进制表示为 **664**

- **`rw-`**（拥有者）：可读、可写，但不可执行 	4(读) + 2(写) = **6**
- **`rw-`**（所属组）：可读、可写，但不可执行          **6**
- **`r--`**（其他用户）：仅可读，不可写、不可执行    4(读) = **4**

# 文件编辑器

https://zhuanlan.zhihu.com/p/476183747

在 Linux 系统中，Vim、`nano` 和 `gedit` 都是常用的文本编辑器，但它们在设计理念、操作方式、适用场景等方面有着显著的区别。下面从几个关键维度进行对比：

---

### 1. **Vim —— 强大的模式化编辑器**
- **界面**：基于终端（命令行）的编辑器，没有图形界面。
- **操作方式**：**模式化编辑**。Vim 有普通模式、插入模式、可视模式等。在普通模式下，按键对应各种编辑命令（如 `dd` 删除行，`yy` 复制行），需要按 `i` 进入插入模式才能输入文本。这种设计使得手指无需离开键盘即可高效操作，但**学习曲线较陡**，初学者容易因不知如何退出而困扰（著名的 `:q!` 梗）。
- **适用人群**：程序员、系统管理员、喜欢键盘流操作的用户。特别适合在远程服务器（无图形界面）上编辑配置文件或代码。
- **优点**：极其高效、可扩展性强（支持插件）、占用资源少、几乎在所有 Linux 发行版中默认安装（或可通过 `vim` 包安装）。
- **缺点**：入门难度高，需要记忆大量快捷键和命令。

### 2. **`nano` —— 简单直观的终端编辑器**
- **界面**：同样基于终端，但界面底部会显示常用快捷键提示（如 `^X` 退出，`^O` 保存）。
- **操作方式**：**非模式化**。启动后直接进入文本输入状态，无需切换模式。所有功能通过组合键（Ctrl + 字母）完成，操作逻辑类似图形界面编辑器的快捷键。
- **适用人群**：Linux 新手、需要快速编辑配置文件的用户、不喜欢复杂操作的人。
- **优点**：上手极快，无需学习即可使用；几乎所有主流 Linux 发行版都默认预装；适合在终端环境下进行简单文本修改。
- **缺点**：功能相对基础，对于复杂编辑任务（如批量替换、宏录制）不如 Vim 强大。

### 3. **`gedit` —— 轻量级图形界面编辑器**
- **界面**：基于图形窗口（需要桌面环境，如 GNOME），提供菜单栏、工具栏、标签页等 GUI 元素。
- **操作方式**：完全符合现代图形软件的习惯，鼠标和键盘均可操作。支持语法高亮、代码折叠、插件扩展（如终端嵌入、文件浏览器）等功能，但默认配置下功能仍偏简单。
- **适用人群**：桌面 Linux 用户（如 `Ubuntu` 默认 GNOME 桌面），习惯用鼠标操作、希望获得所见即所得体验的人。
- **优点**：界面友好，零学习成本；支持多标签页、打印、撤销/重做等高级功能；可通过插件增强为轻量级 `IDE`。
- **缺点**：依赖图形环境，无法在纯终端或远程 SSH 会话中使用；资源占用比终端编辑器高。

---

### **对比总结**
| 特性         | Vim                        | nano                       | gedit                           |
| ------------ | -------------------------- | -------------------------- | ------------------------------- |
| **界面类型** | 终端                       | 终端                       | 图形界面                        |
| **操作模式** | 模式化（命令/插入等）      | 无模式，直接输入           | 无模式，图形化交互              |
| **学习难度** | 高                         | 低                         | 极低                            |
| **适用场景** | 编程、服务器配置、高效编辑 | 快速修改配置文件、新手入门 | 桌面环境下的文本编辑            |
| **默认安装** | 通常预装（或可选）         | 几乎所有发行版预装         | 取决于桌面环境（如 GNOME 自带） |
| **功能扩展** | 插件丰富（如插件管理器）   | 有限（可通过配置调整）     | 支持插件（如语法检查、补全）    |

**一句话总结**：
- **Vim**：瑞士军刀，强但需学习；
- **`nano`**：小刀，简单直接；
- **`gedit`**：家用工具，舒适便捷。

选择哪个取决于你的使用环境（终端还是桌面）以及你对编辑效率的追求。

# vi和vim

# 日志系统

# 重启命令

```bash
sudo reboot
```

# APT管理软件

==参见==：https://geek-blogs.com/blog/linux-install-apt/#mu4-lu4

## 1. 更新包缓存

- 更新软件包列表:

```bash
sudo apt update
#或者使用 Debian方式
sudo apt-get update
```

- **何时使用**：修改软件源后、安装新软件前、定期更新系统时。

## 2. 升级已安装的软件包:

```bash
sudo apt upgrade  # 升级所有可升级的软件包（不删除旧包，不处理依赖冲突）
sudo apt full-upgrade  # 升级并自动处理依赖冲突（可能删除旧包，谨慎使用）
```

- **示例**：升级指定软件包 `nginx`：

```bash
sudo apt upgrade nginx
```

## 3. 安装软件包

```bash
sudo apt install <包名>  # 安装指定软件包
sudo apt install <包1> <包2> ...  # 同时安装多个软件包
sudo apt install ./软件包.deb  # 安装本地 .deb 文件（需手动处理依赖）

#示例：安装文本编辑器 vim：
sudo apt install vim
```

- **选项**：
  - `-y`：自动回答“yes”（跳过确认，适合脚本）：`sudo apt install -y vim`。
  - `--no-install-recommends`：仅安装必要依赖（减少冗余）：`sudo apt install --no-install-recommends vim`。

## 4. 卸载软件包

```bash
sudo apt remove <包名>  # 卸载软件包（保留配置文件）
sudo apt purge <包名>   # 彻底卸载（删除配置文件）

#示例：彻底卸载 nginx：
sudo apt purge nginx
```

## 5. 清理无用依赖

```bash
sudo apt autoremove  # 卸载不再被依赖的孤立包（推荐定期执行）
sudo apt autoclean   # 清理已过期的下载缓存（/var/cache/apt/archives/）
sudo apt clean       # 清理所有下载缓存（释放磁盘空间）
```

## 6. 查看软件包信息

```bash
apt show <包名>  # 显示软件包详细信息（版本、依赖、描述等）
apt list --installed  # 列出所有已安装的软件包
apt list --upgradable  # 列出所有可升级的软件包
```

## 7.  搜索软件包

```bash
apt search <关键词>  # 搜索名称或描述包含关键词的软件包

#示例：搜索“文本编辑器”相关包：
apt search text editor
```

## 8. 固定软件包版本（Pin）

通过配置优先级，强制 APT 从指定仓库安装特定版本的软件包（避免被其他仓库的高版本覆盖）。

1. 创建优先级文件 `/etc/apt/preferences.d/my-pin.pref`：

   ```bash
   Package: nginx
   Pin: release o=nginx
   Pin-Priority: 900  # 优先级高于其他仓库（默认 500）
   ```

2. 查看包优先级：

   ```bash
   apt policy nginx  # 显示 nginx 各版本的来源和优先级
   ```

## 9. 锁定/解锁软件包版本

禁止特定软件包被升级：

```bash
sudo apt-mark hold <包名>  # 锁定版本
sudo apt-mark unhold <包名>  # 解锁版本
```

- **示例**：锁定 `python3` 版本：

  ```bash
  sudo apt-mark hold python3
  ```

## 10. 下载软件包（不安装)

```bash
apt download <包名>  # 下载 .deb 文件到当前目录（用于离线安装）
```

## 11. 检查软件包依赖关系

```bash
apt depends <包名>  # 查看指定包的依赖项
apt rdepends <包名>  # 查看依赖于指定包的其他包（反向依赖）
```

## 12. APT 使用最佳实践

### 1. 定期更新系统

```bash
sudo apt update && sudo apt upgrade -y  # 每周至少执行一次，修复安全漏洞
```

### 2. 谨慎使用 `full-upgrade`

`full-upgrade` 可能因依赖冲突删除软件包，建议先执行 `apt upgrade`，仅在 `upgrade` 提示依赖问题时使用 `full-upgrade`，并**仔细检查将要删除的包**。

### 3. 管理软件源时保持谨慎

- 仅添加**可信的第三方仓库**（如官方 PPA、知名组织仓库），避免恶意软件。
- 不同发行版的仓库（如 Ubuntu 和 Debian）**不可混用**，可能导致依赖混乱。
- 添加仓库后，务必导入其 GPG 公钥（避免“GPG 错误”）。

### 4. 避免使用 `sudo apt install -y` 盲目确认

`-y` 选项会自动回答“yes”，可能在不知情的情况下安装大量依赖或删除文件。建议首次执行时不带 `-y`，确认操作无误后再添加。

### 5 定期清理系统

```bash
sudo apt autoremove && sudo apt autoclean  # 清理无用依赖和过期缓存，释放磁盘空间
```

### 备份关键配置文件

修改 `/etc/apt/sources.list` 或重要软件包配置前，先备份：

```bash
sudo cp /etc/apt/sources.list /etc/apt/sources.list.bak
```

# 全屏

- 需要先安装好`vmtools`
- 按住`ctrl + alt +enter`

# 长期支持版本

- **`Ubuntu 24.04 LTS`(代号:`Noble Numbat`)是一个长期支持版（）**，于2024年4月25日正式发布，官方支持至少到2029年5月31日。它提供5年的主流安全支持，并可以通过`Ubuntu Pro`订阅获得额外5年的扩展安全支持

- **`Ubuntu 22.04 LTS`（代号:`Jammy Jellyfish`）是一个长期支持版本**，官方提供5年的支持周期，适合追求稳定性的用户和企业环境。其支持将持续到2027年4月，期间将提供安全更新和错误修复。

# `pwd`

`print working directory`：用于显示当前工作目录的绝对路径。

- 在 Bash 中，`pwd` 通常是一个**内置命令**（shell builtin），同时系统中也有一个独立的可执行文件（如 `/bin/pwd`）。
- 可以通过 `type pwd` 查看：

```bash
type pwd
#pwd 是 shell 内建

```

# 关机和重启

`shutdown` 、`halt`、`reboot`、`init`

**使用权限**：系统管理者。

## `shutdown`

### 语法

```bash
shutdown [-t seconds] [-rkhncfF] time [message]
```

**参数说明**：

- -t seconds : 设定在几秒钟之后进行关机程序。
- -k : 并不会真的关机，只是将警告讯息传送给所有使用者。
- -r : 关机后重新开机。
- -h : 关机后停机。
- -n : 不采用正常程序来关机，用强迫的方式杀掉所有执行中的程序后自行关机。
- -c : 取消目前已经进行中的关机动作。
- -f : 关机时，不做 fsck 动作(检查 Linux 档系统)。
- -F : 关机时，强迫进行 fsck 动作。
- time : 设定关机的时间。
- message : 传送给所有使用者的警告讯息。

### 实例

立即关机

```bash
# shutdown -h now
```

指定 10 分钟后关机

```bash
# shutdown -h 10
```

重新启动计算机

```bash
# shutdown -r now
```

## `halt`

若系统的 runlevel 为 0 或 6 ，则Linux halt命令关闭系统，否则以 shutdown 指令（加上 -h 参数）来取代。

使用权限：系统管理者。

### 语法

```
halt [-n] [-w] [-d] [-f] [-i] [-p]
```

**参数说明**：

- -n : 在关机前不做将记忆体资料写回硬盘的动作
- -w : 并不会真的关机，只是把记录写到 /var/log/wtmp 文件里
- -d : 不把记录写到 /var/log/wtmp 文件里（-n 这个参数包含了 -d） -f : 强迫关机，不呼叫 shutdown 这个指令
- -i : 在关机之前先把所有网络相关的装置先停止
- -p : 当关机的时候，顺便做关闭电源（poweroff）的动作

### 实例

关闭系统

```bash
# halt
```

关闭系统并关闭电源

```bash
# halt -p
```

关闭系统，但不留下纪录

```bash
# halt -d
```

## `reboot`

Linux reboot命令用于用来重新启动计算机。

若系统的 runlevel 为 0 或 6 ，则重新开机，否则以 shutdown 指令（加上 -r 参数）来取代

### 语法

```
reboot [-n] [-w] [-d] [-f] [-i]
```

**参数**：

- -n : 在重开机前不做将记忆体资料写回硬盘的动作
- -w : 并不会真的重开机，只是把记录写到 /var/log/wtmp 档案里
- -d : 不把记录写到 /var/log/wtmp 档案里（-n 这个参数包含了 -d）
- -f : 强迫重开机，不呼叫 shutdown 这个指令
- -i : 在重开机之前先把所有网络相关的装置先停止

### 实例

重新启动

```bash
# reboot
```

## `init`

==参见：==https://www.runoob.com/linux/linux-comm-init.html

# 帮助手册

# 远程登录

==参见==：https://zhuanlan.zhihu.com/p/1981774145554494290

# 一些目录作用

### `/etc`

- **作用**：存放系统**配置文件**。
- **内容**：几乎所有系统级别的配置文件都位于此目录下，例如网络配置、用户账户信息、软件包配置等。
- **特点**：通常只有超级用户（root）有写权限，普通用户只有读权限。

------

### `/bin`

- **作用**：存放**基本用户命令**（可执行二进制文件）。
- **内容**：包含系统启动、维护以及所有用户（包括单用户模式）都必需的命令，如 `ls`、`cp`、`mv`、`bash` 等。
- **特点**：独立于其他分区，即使 `/usr` 未挂载也能运行。

------

### `/sbin`

- **作用**：存放**系统管理命令**（二进制文件）。
- **内容**：主要用于系统管理员（root）执行的维护和管理命令，如 `fdisk`、`ifconfig`、`reboot` 等。
- **特点**：普通用户一般无法执行，或者执行时需要使用 `sudo`。

------

### `/home`

- **作用**：普通用户的**家目录**。
- **内容**：每个普通用户在此目录下都有一个与其用户名同名的子目录（如 `/home/alice`），用于存放个人文件、配置等。
- **特点**：通常独立分区，方便备份和迁移。

------

### `/root`

- **作用**：**超级用户（root）的家目录**。
- **内容**：root 用户的个人文件、配置、脚本等。
- **特点**：位于根目录下而非 `/home` 内，是为了保证系统在 `/home` 未挂载或出现故障时，root 仍能正常登录。

------

### `/boot`

- **作用**：存放**引导加载程序所需的文件**。
- **内容**：包括 Linux 内核（如 `vmlinuz`）、初始内存盘（`initrd` 或 `initramfs`）、引导器配置文件（如 GRUB 的相关文件）等。
- **特点**：通常独立分区，位于磁盘开头，以确保系统引导时能被 BIOS/UEFI 访问。

------

### `/dev`

- **作用**：**设备文件目录**。
- **内容**：包含系统识别到的所有硬件设备（如硬盘 `/dev/sda`、终端 `/dev/tty`、随机数生成器 `/dev/random` 等）的抽象文件。
- **特点**：这些文件在 Linux 中遵循“一切皆文件”的思想，用户可以通过读写这些文件来与设备交互。

------

### `/mnt`

- **作用**：**临时挂载点**。
- **内容**：通常为空，用于临时挂载其他文件系统（如光盘、USB 设备、网络共享等）。
- **特点**：管理员可以在此目录下创建子目录（如 `/mnt/cdrom`）并挂载设备，使用完毕后再卸载。相比之下，`/media` 更常用于自动挂载的可移动设备。

------

这些目录共同构成了 Linux 系统的基本文件结构，了解它们的作用有助于更好地管理和维护系统。

# ls -l常见颜色

| 颜色                 | 文件类型                                       |
| :------------------- | :--------------------------------------------- |
| **蓝色**             | 目录                                           |
| **绿色**             | 可执行文件（有执行权限的普通文件）             |
| **浅蓝色**（青色）   | 符号链接（软链接）                             |
| **红色**             | 压缩包（`.tar`、`.gz`、`.zip` 等）或损坏的链接 |
| **黄色**             | 设备文件（如 `/dev/sda`）                      |
| **粉色/紫色**        | 图片、音频、视频等多媒体文件（取决于配置）     |
| **白色/灰色**        | 普通文件                                       |
| **红色背景**（闪烁） | 无效的链接（链接目标不存在）                   |
| **黑色背景+红色**    | setuid 文件（如 `passwd`）                     |

# 硬链接和软链接

 [汇华Linux操作系统及应用.md](汇华Linux操作系统及应用.md) 

https://www.qianwen.com/share/chat/c3c7e16f26764471acf6616a1b9e6b8a
