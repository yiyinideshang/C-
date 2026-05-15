# Linux运用领域

- 个人桌面运用
  - 所谓个人桌面，其实就是类似windows系统一样。和个人计算机是一样的。

- Linux作为企业级服务器应用
  - 免费开源
  - 稳定性极强且有强大的社区支撑
  - 使用自由
  - 有海量应用
  - 安全
- 嵌入式方面的引用
  - 因为Linux开源、功能强大、稳定、灵活且具有极大的伸缩性。

# Linux版本

Linux的发行版本：

- ubuntu 
- centos 
- redhat 
- debai
- kali ...

# Ubuntu的设置

## 终端（命令行）terminal 

终端（shell）输入命令来控制系统的。 

**打开终端**

1. 在ubuntu的任务栏有一个终端图标可以直接点击打开
2. 按下快捷键：ctrl+alt+t可以打开终端 
3. 在任何地方右键，点击在此处打开终端 
4. 在一个终端的界面下，按下快捷键：ctrl+shift+n可以新建一个终端

终端是**用户**和**内核**之间的沟通的桥梁。它接受用户输入的指令，然后把指令交给内核，内核执行指令， 终端等待内核执行完毕。把执行完毕之后处理结果反馈给用户。

# 设置共享目录

**设置共享目录目的**：为了实现两个操作系统之间文件互通

设置方法：

- VMwarePlayer设置方式：
  - 在VMware的的左上角有一个叫做player的按钮，旁边有一个倒三角，点他！！
  - 再点击 管理--->虚拟机设置 选项 
  - 在弹出的窗口里面，点击 选项 这个选项卡 
  - 找到共享文件夹选项 
  - 将右侧的文件夹共享属性更改为：总是启用 点击右下方的 添加 按钮
  - 根据共享文件夹向导进行共享文件夹的添加 
    - 主机路径：在window下的文件夹路径（需要共享的文件夹） 
    - 名称：windows的文件夹在linux里面叫什么名字 
  - 点击确定
  - 点击确定 
- VMwareworkstation共享目录设置方式： 
  - 点击 上方菜单栏 的 虚拟机 中的 设置 选项
  - 在弹出的窗口里面，点击 选项 这个选项卡
  - 找到共享文件夹选项
  - 将右侧的文件夹共享属性更改为：总是启用 
  - 点击右下方的 添加 按钮 
  - 根据共享文件夹向导进行共享文件夹的添加 
    - 主机路径：在window下的文件夹路径（需要共享的文件夹） 
    - 名称：windows的文件夹在linux里面叫什么名字 点击确定 点击确定
  - 点击确定
  - 点击确定

# ==--------------------==

# 前期配置

## **手动挂载共享文件夹**：

sudo /usr/bin/vmhgfs-fuse .host:/ /mnt/hgfs -o subtype=vmhgfsfuse,allow_other

`sudo /usr/bin/vmhgfs-fuse .host:/ /mnt/hgfs -o subtype=vmhgfsfuse,allow_other`

- 手动挂载有个弊端：虚拟机关机重启之后，需要重新挂载

## **自动进行共享文件夹的挂载**：

- `sudo vim /etc/fstab` 
- 按下 `o` 
- 将此指令复制到终端上： `.host:/ /mnt/hgfs fuse.vmhgfs-fuse allow_other,uid=1000,gid=1000,umask=022 0 0`
  -  粘贴：鼠标滚轮（鼠标中键） 
- 按下 `esc 键` 
- 输入 `:` 然后再输入 `wq` 回车就可以了

## VsCode的远程连接Linux系统

**第一步**：在 VsCode 的扩展中下载一个插件 

- 插件名称： Remote-SSH

**第二步**：检查Linux有没有安装SSH服务

- 检查存不存在: which sshd
- 检查是否在运行：ps - e | grep ssh
  - 如果上述指令有显示    `954 ?        00:00:00 sshd `则说明有
- 如果没有SSH服务：
  - 安装SSH服务： sudo apt-get install openssh-server
- 如果没有启动SSH服务:
  - 启动SSH服务： sudo service sshd restart

**第三步**：生成秘钥

- 在 Windows 下打开 cmd 
  - 执行指令： ssh-keygen
  - 一直按回车等待结束
- 秘钥生成完毕后，复制秘钥地址C:\Users\Lenovo/.ssh/id_ed25519到虚拟机
  - 将 C盘->用户目录->.ssh目录下的`id_rsa.pub` 文件复制到**虚拟机的** `计算机->home->用户目录- >.ssh `下;
  - 如果 linux 中没有 .ssh 文件夹，那么请打开显示隐藏文件选项，并创建该文件
- 在虚拟机中将复制的`id_rsa.pub` 文件中的内容，复制到` .ssh`文件夹中的`authorized_keys` 中.
  - 或者将`id_rsa.pub` **文件更名**为`authorized_keys `

**第四步**:在`VScode`中新建远程连接

- 点击右侧的加号，新建一个远程连接
- 请输入： ssh Linux用户名@Linux的ip地址 回车;ssh yishang@192.168.48.130
  - 在虚拟机中点击有线设置，找到`IPV4`的地址进行复制;`192.168.48.130`
  - ![微信图片_20250820203106](D:\Typora\typora_work\1Linux基础\2Linux基础操作\微信图片_20250820203106.png)
  - ![微信图片_20250820203114](D:\Typora\typora_work\1Linux基础\2Linux基础操作\微信图片_20250820203114.png)

- 点击加号，输入ssh yishang@192.168.48.130如图所示

  ![微信图片_20250820204407](D:\Typora\typora_work\1Linux基础\2Linux基础操作\微信图片_20250820204407.png)

- 点击第一个，添加主机

![微信图片_20250820204639](D:\Typora\typora_work\1Linux基础\2Linux基础操作\微信图片_20250820204639.png)

![微信图片_20250820204755](D:\Typora\typora_work\1Linux基础\2Linux基础操作\微信图片_20250820204755.png)

- 点击连接输入密码123456

![微信图片_20250820204916](D:\Typora\typora_work\1Linux基础\2Linux基础操作\微信图片_20250820204916.png)

- 完成远程连接

![微信图片_20250820204946](D:\Typora\typora_work\1Linux基础\2Linux基础操作\微信图片_20250820204946.png)

---

# Linux常用操作

## 常用文件系统

**文件系统**：对于文件的一套管理、组织、操作的软件。

- `ext3` 
- `ext4 `
- `FAT32 `
- ``NTFS``
- .......

![微信图片_20250820210055](D:\Typora\typora_work\1Linux基础\2Linux基础操作\微信图片_20250820210055.png)

## Linux的文件

Linux的文件组织：**树状结构**

![微信图片_20250820205950](D:\Typora\typora_work\1Linux基础\2Linux基础操作\微信图片_20250820205950.png)

- **绝对路径**：
  - 所有以根目录为起点的路径，称为： 绝对路径 (**完整路径**) （起点是固定是 **根目录**）

- **相对路径**：
  - 不以根目录为起点的路径，称为： 相对路径 (**不完整路径**) （起点是不固定的）

**注意：在Linux下一切皆文件。**

# Linux终端

==快捷键ctrl+alt+t==

Linux终端前缀部分（当前终端的工作信息）：

**yishang@yishang-virtual-machine:~$** 

**yishang@yishang-virtual-machine:~/桌面$** 

- `yishang`:表示当前**用户名**
- @**分隔符** 用于将用户名和后面的内容进行分割
- `yishang-virtual-machine`：表示当前计算机的名字(**系统名字**)
- `:`分割符 用于分割计算机名和后面的内容
- ~/桌面：表示当前终端的工作路径，表示当前终端所处`家目录下的ClassCodes(桌面)`目录
- `$`：分隔符 用户将工作路径和用户输入的指令进行分割

# Linux路径

- `~ `：表示当前用户的**家目录**（当前用户的目录）
  - 在家目录下面当前用户权利是非常大的。
- `/` ：表示根目录、Linux系统所有文件/文件夹的起点、所有的目录都是从根目录开始
  - 归属于`root` （超级管理员）用户
- `. `：当前目录、表示当前所属的工作目录
- `..` ：上级目录、相对路径，表示相对于当前而言的上一级目录

**注意：`可以通过 # 和 $ 来区分此时是操作的超级用户还是普通用户。`$ 就是普通用户， # 表示超级用户**

# vi/vim编辑器

vi/vim是Linux系统中最基本的、也最常用、功能最强的的文本编辑器。 

打开方式： 在终端上使用指令：

-  `vi`或者`vim` 打开一个空的编辑器 
- 打开一个指定文件： vi/vim 文件名.扩展名 打开文件

# vi/vim两种模式

- **编辑模式**：
  - 键盘上输入的所以字符都是作为文件的内容去使用
- **命令模式**：
  - 键盘上输入的字符作为命令使用

**==注意：编辑器一启动默认就是命令模式。==**

## 让vi左下角显示insert图标的方法

- 终端输入：sudo vi /etc/vim/vimrc.tiny

- 点击方向键，将光标移动到 “set  compatible” 的 “compatible” 的c字母上面：

- 点击esc键进入‘’一般命令模式‘’

- 然后点击键盘上的i字母键，进入“编辑模式”，输入no两个字母，将 “set  compatible” 修改为 “set  nocompatible”
- 之后点击esc键，进入“一般命令模式”，在“一般命令模式”下，点击：wq！回车，注意是冒号、wq两个字母再加一个感叹号，然后回车。
- 这样就保存修改并退出vi编辑器了。

### 查看是否修改成功

终端输入：cat /etc/vim/vimrc.tiny 

set compatible已经被修改成set nocompatible了，说明你的sudo vi /etc/vim/vimrc.tiny命令已经成功修改了/etc/vim/vimrc.tiny文件。这样，在下次使用vi编辑器的时候，左下角熟悉的insert图标又重新出现了：

# **命令模式切换至编辑模式**

- i ： insert 进入编辑模式之后、不会移动
-  I ： Insert 进入编辑模式之后、光标会移动至行首 
- o ： open 向下开路、在光标所在行的下一行，新开一行。 
- O ： Open 向上开路、在光标所在行的上一行、新开一行。 
- a ： append 追加、进入编辑模式之后、光标会往后移动一个字符追加 
- A ： Append 追加、进入编辑模式之后、光标会移动到行尾追加

# **编辑模式切换至命令模式**

- `esc`

==**注意：区分是编辑模式还是命令模式的话，可以看一下终端的左下角有没有一个 `--插入--` 的标记**==

# 命令模式下命令 

- 退出/保存
  - :q ： `quit` 退出、请注意这条命令只能在文件没有进行修改的情况使用、才能正常退出。
  - :q! ：强制退出、不保存 
  - :wq ： write quit 保存并退出 :w ： write 保存
  -  :x ： 保存并退出 
- 删除/修改
  - x ：删除光标所在位置的字符 
  - dd ：删除光标所在行的字符
  -  ndd ： number delete n所表示的就是一个数字，表示把光标所在行及往下n行内容全部删除 
    - 这个命令所删除的内容，**实际被保存剪切板**里面 
  - 小写p ： print 将剪切板中的内容粘贴到**当前行的后面**
  -  大写 P ： Print 将剪切板中的内容粘贴到当前行的前面 
  - yy ：复制当前行，把当前行的内容拷贝到剪切板中 
  - nyy ： number n所表示的就是一个数字，表示把光标所在行及往下n行内容全部复制到剪切板
  -  r ： replace 替换光标所在的那一个字符，按下r再按下要替换的那个字符
  -  R ： Replace 替换光标及之后的多个字符 （按下 ESC 退出替换模式） 
  - u ： undo 撤销操作 
- 搜索 
  - 指令格式： `:/需要搜索的内容` 
  -  n ： next 找到下一个匹配的内容
  -  N ： prev 找到上一个匹配的内容
  - 注意：**第一次查询不会定位到内容处**，需要先使用一次 n/N

- 替换
  -  s ： substitute 替换 
  - 指令格式： `:{作用范围}s/{目标}/{替换内容}/{替换标志}`
    - 作用范围： 作用范围不指定、默认在当前行进行查找 `目标` 
    - 如果是在全文查找 `%` 
    - 如果是在指定范围查找 `选择区域` 
      - `2,5` 表示从第二行开始到第五行范围内去查找 `目标` 
      - `.,+2` 表示从当前行开始往下两行的范围查找 `目标` 
    - 替换标志：
      - `g ： global` 全局替换 及所有行都会进行替换
      -  `i ： ignore` 忽略大小写
  - 将当前行的第一个 目标 进行替换。如果当前行存在多个 目标 则只会替换其中一个。

- 显示行号
  - ==`:set nu` 显示行号==

**注意：在没有文件名情况下** 

-  在使用 `vi/vim` 的时候可以通过指定文件名来创建文件

- 在保存的时候： `w/wq 文件名.扩展名` 保存并创建

  

# 文件

- **文件属性**： `inode` 是唯一标识符，是用来表示一个文件存不存在的东西(类似身份证)
  -  文件名 
  - 文件大小
  -  文件类型 
  - ... 
- **文件内容**：
  - inode ：索引节点，文件的字节数大小，拥有者..... 
  - 文件存储硬盘上、操作系统把硬盘氛围两部分：
    -  一个节点区：存储文件 inode 信息 
    - 一个数据区：存储文件数据内容 

表面上，用户通过文件名打开文件。实际上系统通过 inode 节点信息，找到该文件的数据存储的区域， 读取数据。 

- 作业： 熟悉Vi/Vim指令

# 目前已有虚拟机的镜像位置

````c
"D:\浏览器下载\ubuntu-22.04.5-desktop-amd64.iso"
"D:\Ubuntu16\ubuntu-16.04.6-desktop-amd64.iso"
"D:\kalikali\kali-linux-2025.3-installer-amd64.iso"
"D:\Windows10\cn_windows_10_business_editions_version_1909_updated_jan_2020_x64_dvd_b3e1f3a6.iso"
````

# vmtools工具使用的镜像位置

```c
"C:\Program Files (x86)\VMware\VMware Workstation\linux.iso"
"C:\Program Files (x86)\VMware\VMware Workstation\windows.iso"
```

# 📒 初次配置 WARP 完整笔记（Ubuntu Linux 版）

### 1️⃣ 安装

```bash
# 添加公钥
curl -fsSL https://pkg.cloudflareclient.com/pubkey.gpg | sudo gpg --dearmor -o /etc/apt/trusted.gpg.d/cloudflare-warp.gpg

# 添加软件源
echo "deb https://pkg.cloudflareclient.com/ $(lsb_release -cs) main" | sudo tee /etc/apt/sources.list.d/cloudflare-warp.list

# 更新并安装
sudo apt update
sudo apt install cloudflare-warp
```

> **NO_PUBKEY 报错修复：** `sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys <缺失的KEY>`

### 2️⃣ 注册与连接（避开超时/卡死）

关键：**强制使用 TCP 协议**

```bash
# 注册（新版命令）
warp-cli registration new --accept-tos

# 🔥最重要的一步：改用 TCP 连接（避免卡在 Connecting）
warp-cli settings set protocol tcp

# 启动服务并连接
sudo systemctl restart warp-svc
warp-cli connect
```

> **IPC 通信超时修复：**
>
> ```bash
> sudo systemctl stop warp-svc
> sudo rm -rf /var/lib/warp-svc/
> sudo systemctl start warp-svc
> ```

### 3️⃣ 解除 DNS 劫持（⚡钢铁避坑）

WARP 能连上 ≠ 能上 GitHub！必须清理残留的 DNS 劫持：

```bash
# 清理 hosts
sudo sed -i '/github.com/d' /etc/hosts

# 配置系统 DNS 为 1.1.1.1
sudo nano /etc/systemd/resolved.conf
# → 修改为：DNS=1.1.1.1 1.0.0.1

# 重启解析服务并清缓存
sudo systemctl restart systemd-resolved
sudo resolvectl flush-caches # 刷新 DNS
```

### 4️⃣ Firefox 设置（必看 🔥）

网通但 Firefox 打不开？

- 设置 → 网络设置 → **无代理**
- 隐私与安全 → **关闭 DNS over HTTPS**

### 5️⃣ 日常使用命令（你要求的 🔁）

```bash
## 关闭 WARP
warp-cli disconnect

## 重新开启 WARP
warp-cli connect

## 刷新 DNS（当连上后解析依然异常时使用）
sudo resolvectl flush-caches
```

### 6️⃣ 完全卸载（一劳永逸）

```bash
warp-cli disconnect
sudo systemctl stop warp-svc
sudo systemctl disable warp-svc
sudo apt remove --purge cloudflare-warp
# 恢复 resolved.conf 中的 DNS 设置
sudo systemctl restart systemd-resolved
```

---

### 💡 核心避坑口诀（背下来）

- **安装卡在公钥？** → 手动添加密钥
- **连接一直 Connecting？** → TCP 协议 + 重启服务
- **GitHub 解析成 127.0.0.1？** → 修改 resolved.conf + 重启 DNS
- **火狐打不开？** → 关闭代理 + 关掉 DoH

