# Qt图形库概述

**图形化框架**

Qt是一个跨平台的C++应用程序开发框架。

它由Qt Company（前身为 `Nokia` ）开发，用于开发图形用户界面、嵌入式系统、以及其他应用程序， 支持 Windows 、 `macOS` 、 Linux 、 Android 和 `iOS` 等多个操作系统。

Qt框架提供了丰富的功能和工具，包括图形用户界面设计、数据库操作、网络编程、文件处理等。它还 包括了一个广泛的类库，开发者可以使用这些类来加速应用程序的开发过程。Qt还具有良好的文档和社 区支持，因此广泛用于各种类型的应用程序开发，包括桌面应用、移动应用、嵌入式系统等。

# Qt的主要发展阶段

- 1991年： `Haavard Nord` 和 `Eirik Chambe-Eng` 创建了Qt工具集。 
- 1994年： Qt被 `Nokia` 收购，成为 `Nokia` 的子公司。 `Nokia` 推动了Qt的跨平台特性，使其能够在 不同的操作系统上运行。 
- 2008年： Qt的所有权由 `Nokia` 转移到了 `Qt Software` （之后更名为`Qt Company`）。Qt Company继续发展Qt，将其打造成一个跨平台的应用程序开发框架。 
- 2011年： Qt 的开发遭遇了一些波折，包括 `Nokia` 的战略变化和Qt的开发者社区担忧Qt的未来。 这一时期，Qt的发展由社区驱动。
-  2012年： `Qt Project` 成立，该项目由Qt的爱好者和开发者组成，致力于Qt的开发和推广。 
- 2014年： `Digia` 宣布收购Qt商业版的所有权，成为Qt的新所有者。 
- 2016年： `The Qt Company` 宣布Qt的开源版本将采用 GPLv3 协议。Qt 5.6发布，引入了许多新特 性和改进。 2017年： Qt 5.9发布，增加了对 3D 图形的支持和`Qt Quick Controls 2`。 
- 2018年： Qt 5.11发布，引入了 `Qt Quick 2D Rendere r`，提供了在没有 `OpenGL` 支持的系统上运 行 Qt Quick 应用的能力。
-  2019年： Qt 5.12发布，引入了 `Qt for WebAssembly` ，使得开发者可以将Qt应用程序编译成 `WebAssembly` 格式，可以在现代Web浏览器中运行。 
- 2020年： Qt 6发布，这是Qt的一个重大版本，引入了许多新特性和改进，包括模块化架构、 C++17 支持、图形 API 的改进等。
-  2021年： Qt 6.2发布，引入了 `Qt Quick 3D` ，使得开发者可以更轻松地在Qt应用中添加 3D 图 形。

# Qt的优势

Qt拥有许多优势，使其成为一个受欢迎的应用程序开发框架。以下是一些Qt的主要优势：

- 跨平台性： Qt是一个跨平台框架，可以在多个操作系统上运行，包括 Windows 、 macOS 、 Linux 、 Android 和 iOS 。这意味着开发者可以编写一份代码，然后在不同的平台上部署应用程 序，减少了开发的工作量。 
- 丰富的类库： Qt提供了丰富的类库，涵盖了图形用户界面、网络、数据库、文件处理、XML处理 等多个领域。这些类库可以帮助开发者加速应用程序的开发过程，减少了从头开始编写代码的需 求。
-  易用性： Qt提供了直观且易用的 API ，使得开发者能够快速上手，并且能够高效地开发功能丰富 的应用程序。 
- 强大的社区和支持： Qt拥有一个庞大的开发者社区和支持团队，开发者可以在社区中获取帮助、 分享经验，并且有大量的文档、示例和教程可供参考。
- 灵活性： Qt是一个非常灵活的框架，允许开发者使用C++编程语言进行开发，同时也提供了Qt Quick，这是一个基于 QML （Qt Meta-Object Language）的框架，用于快速开发移动应用和嵌入 式应用的用户界面。 
- **开源和商业许可**： Qt提供了开源版本（遵循 LGPL 协议），同时也提供了商业版本，开发者可以根 据项目的需求选择合适的许可类型。
-  现代化： Qt不断更新和演进，保持与时俱进，支持最新的C++标准，提供了现代化的开发体验。 
- 高性能： Qt框架经过优化，能够提供高性能的应用程序，同时它还提供了内置的工具和 API ，用 于性能分析和调优。

总的来说，Qt是一个功能强大、易用性高、跨平台性好、支持广泛的应用程序开发框架，适用于各种规 模和类型的项目。

# Qt特点

- 广泛用于开发GUI程序，也可用于开发非GUI程序。
  - `GUI = Graphical User Interface`
  - 也就是基于窗口的应用程序开发。
- 有丰富的 API
  - Qt 包括多达 250 个以上的 C++ 类
  - 可以处理正则表达式。
- 支持 2D/3D 图形渲染，支持 OpenGL
- Qt给程序猿提供了非常详细的官方文档
- 支持XML，Json
- 框架底层模块化， 使用者可以根据需求选择相应的模块来使用
- 可以轻松跨平台
  - 和Java的跨平台方式不同
  - 在不同的平台使用的是相同的上层接口，但是在底层封装了不同平台对应的API（暗度陈仓）。

# Qt版本迭代

**Qt 6 是Qt框架的一个重大版本，引入了许多新特性和改进。在Qt 6发布之前，Qt 5系列也是非常常用 的，特别是Qt 5.12和Qt 5.15版本。**

Qt的一些主要版本：

- Qt 1.x 系列： 最早的版本，于1995年发布。
-  Qt 2.x 系列： 1999年发布，引入了许多新特性。
-  Qt 3.x 系列： 2001年发布，引入了跨平台的支持和更多功能。
-  Qt 4.x 系列： 2005年发布，引入了Qt的现代外观和更多功能，也是第一个支持 `Windows Vista` 的版本。 
- Qt 5.x 系列： 2012年发布，引入了 `Qt Quick` 框架，使得开发者能够更容易地创建现代化的用户 界面。
- Qt 5.6、5.9、5.12和5.15等版本是该系列的一些重要版本。 
- Qt `6.x` 系列： 2020年发布，引入了模块化架构、C++17支持、图形 `API` 的改进等新特性。

# Qt项目案例

- KDE桌面环境： KDE是一个使用Qt构建的开源桌面环境，它是Linux系统中最受欢迎的桌面环境之 一。 
- `Autodesk Maya` ： Autodesk Maya 是一款专业的 3D 计算机图形应用程序，用于建模、动画、仿 真和渲染。它的用户界面是使用Qt构建的。 
- `VirtualBox` ： VirtualBox 是一款由Oracle开发的开源虚拟机软件，它的用户界面也是使用Qt开 发的。
-  `VLC 媒体播放器`： VLC 是一个流行的开源媒体播放器，它的Qt界面版本被广泛用于不同平台。
-  `Telegram 桌面客户端`： Telegram 是一个知名的即时通讯应用，它的桌面客户端使用Qt进行开 发。
-  `Autodesk AutoCAD` ： AutoCAD 是一款专业的计算机辅助设计（CAD）软件，它的Mac版本使用 Qt构建用户界面。 
- `Adobe Photoshop Elements` ： Adobe Photoshop Elements 是Adobe公司的一款图像编辑软 件，它的某些版本的用户界面是使用Qt构建的。
-  `MATLAB Simulink` ： Simulink 是 MATLAB 的一部分，用于进行多域仿真和模型驱动的设计，它 的用户界面使用了Qt技术。

# Qt的安装使用

## Qt的安装

### Windows 系统安装流程

- 下载 Qt 安装程序 
  - 访问 Qt 官方网站（https://www.qt.io/download-open-source）。
  -  点击 “ Download the Qt Online Installer ” 按钮下载在线安装程序。
-  运行安装程序 
  - 双击运行下载的安装程序文件。 
- 登录或注册 Qt 账号 
  - 在安装过程中，选择登录已有的 Qt 账号，或者注册一个新账号。 
- 选择安装组件
  -  在 “Installation Folder” 页面，选择 Qt 的安装路径。 
  - 点击 “Next” 进入 “Select Components” 页面。选择要安装的 Qt 版本、组件和工具。建议选 择 “Qt [版本号]”、“ MinGW [版本号]”Qt Creator（集成开发环境）。
-  接受许可协议并安装 
  - 阅读并接受 Qt 的许可协议，然后点击 “Install” 开始安装。

### Linux 系统安装流程

- 下载 Qt 安装包 
  - 访问 Qt 官方网站（https://www.qt.io/download-open-source）。
  -  对于 Ubuntu 系统，选择 “ Qt for Linux/X11 - Offline Installer ”。 
- 打开终端并进入下载目录 
  - 使用快捷键 Ctrl + Alt + T 打开终端。 
  - 使用 cd 命令进入下载 Qt 安装包的目录。
-  添加执行权限并运行安装程序
  -  在终端中，输入以下命令为安装包添加执行权限： chmod +x [安装包文件名] 。
  -  执行安装程序，输入 ./[安装包文件名] 。如 ./qt-opensource-linux-x64-5.15.2.run 。
-  安装向导 
  - 安装程序启动后，将显示安装向导。按照向导的提示进行操作。 
  - 在 “Installation Folder” 页面，选择 Qt 的安装路径。默认路径通常是 /opt/qt5.15.2 （版本 号可能不同），可以根据自己的需求修改。
  -  在 “Select Components” 页面，选择要安装的 Qt 版本、组件和工具。
  - 选择 “Qt [版本号]”、 “Qt Creator” 阅读并接受 Qt 的许可协议，然后点击 “Install” 开始安装。

# Qt的项目创建

- 打开 QtCreator ,文件->新建文件或项目
- 项目->Application->Qt console Application（不带界面的应用程序） 
- 修改项目名称，修改项目存储路径 选择项目构建工具
  - `qmake`
    - **专属性**：`qmake` 是 Qt 框架自带的项目构建工具，专门为 `Qt` 项目设计。
    -  **配置文件**：使用 qmake 的项目通常有一个名为 `pro` 的配置文件（例如 `project.pro` ），它定义了项目的各种设置，如源文件、头文件、库依赖等。 
    - **简单性**： `qmake` 的语法相对简单，对于小型到中型的 Qt 项目来说， qmake 是一个快速 上手的选择。
    -  **局限性**： `qmake` 的灵活性不如 `CMake` ，对于复杂的项目或需要跨平台高度自定义的构 建过程， `qmake` 可能不够用。
  - `cmake`
    - **通用性**： `CMake` 是一个跨平台的安装（编译）工具，不仅适用于 `Qt` 项目，也适用于其 他 C/C++ 项目。 
    - **配置文件**： `CMake` 使用 `CMakeLists.txt` 文件来配置项目，该文件以编程的方式定义 了构建过程。
    -  **灵活性**： `CMake` 提供了丰富的命令和模块，可以用于创建复杂的构建逻辑，支持高级的 查找库、生成编译器标志等功能。 
    - **社区支持**：由于 `CMake` 在开源社区中的广泛使用，它有着丰富的文档和社区支持。
  - `qbs`
    - **现代化**：`qbs`（`Qt Build System`）是 Qt 公司推出的一个现代化构建工具，旨在提供更 简单、更灵活的构建过程。
    -  **配置文件**： `qbs` 使用基于 `JSON` 的配置文件，这使得配置文件的结构化程度更高。
    -  **模块化**：`qbs` 支持模块化构建，可以更容易地重用构建逻辑。 
    - **集成性**： `qbs` 与 `Qt Creator IDE` 集成得很好，提供了无缝的开发体验。
- 选择构建套件
  - `Desktop` 桌面应用
  - `WebAssembly` 浏览器应用 
  - `Android` 安卓应用

# Qt的pro工程管理文件

在Qt项目中， `.pro` 文件是一个项目文件，用于配置和管理`Qt`应用程序的构建过程。这个文件通常是由 Qt的`qmake`工具使用的，`qmake`会根据 `.pro` 文件的内容来生成适用于不同平台和编译器的构建脚本。

- `TARGET` ： 指定生成的可执行文件的名称。 
- `TEMPLATE` ： 指定项目的类型，例如应用程序（ app ）、库（ lib ）等。 
- `SOURCES` ： 指定源代码文件的列表。 
- `HEADERS` ： 指定头文件的列表。 
- `LIBS` ： 指定需要链接的库。
-  `QT` ： 指定项目所需的Qt模块。 
- `DEFINES` ： 定义预处理宏。
-  `QMAKE_CXXFLAGS` ： 指定编译器选项。 
- `win32/unix/macx` ： 平台特定的配置，用于针对不同的操作系统设置特定的编译选项。

```properties
# 注释行，以#开头

# 定义项目名称
TARGET = MyApplication

# 指定项目类型（应用程序、库等）
TEMPLATE = app

# 指定源代码文件
SOURCES += main.cpp \
		   myclass.cpp
# 指定头文件
HEADERS += myclass.h

# 指定需要链接的库
LIBS += -lmylibrary

# 指定项目所需的Qt模块（例如Widgets、Network等）
QT += widgets network

# 预处理宏定义
DEFINES += MY_DEFINE

# 编译器选项
QMAKE_CXXFLAGS += -std=c++11

# 指定目标平台（例如Windows、Linux、macOS等）
win32 {
    # Windows特定配置
    LIBS += lws2_32
}

unix {
	# Unix（包括Linux和macOS）特定配置
	LIBS += -pthread
}

macx {
	# macOS特定配置
}
```

# Qt命名规范

Qt开发中通常遵循一些命名规范，以提高代码的可读性和可维护性。以下是一些常见的Qt开发命名规 范：

- 类名命名规范： 类名使用驼峰命名法（Camel Case），例如： `MyClass` , `MainWindow` 。 
- 变量名命名规范： 变量名使用驼峰命名法，但通常以小写字母开头，例如： `myVariable` , `buttonClicked` 。
-  常量名命名规范： 常量名通常使用大写字母和下划线（大写蛇形命名法），例如： `MAX_SIZE` , `PI_VALUE` 。
-  函数名命名规范： 函数名使用驼峰命名法，以小写字母开头，例如： calculateArea() , `setWindowTitle`() 。
-  信号和槽函数名命名规范： 信号和槽函数使用驼峰命名法，但通常以`signal`和`slot`结尾，例如： `buttonClickedSignal`() , `saveFileSlot`()。 
- 枚举类型命名规范： 枚举类型使用驼峰命名法，以大写字母开头，例如： `ColorType` , `ButtonType` 。 
- 命名空间命名规范： 命名空间使用驼峰命名法，以大写字母开头，例如： `MyNamespace` , `QtWidgets` 。 
- 文件名命名规范： 文件名通常使用小写字母，单词之间用下划线分隔，例如： `main_window.cpp` , `my_class.h` 。
-  目录名命名规范： 目录名通常使用小写字母，单词之间用下划线分隔，例如： `widgets`, `utilities_functions` 。 
- 私有成员变量前缀： 私有成员变量通常以小写字母m_开头，后面跟随驼峰命名法，例如： m_privateVariable 。

# Qt Creator

- 编写Qt程序默认使用的一款 IDE

# Qt Creator常用快捷键

## ==常规操作快捷键：==

- Ctrl + N ： 新建文件。 
- Ctrl + O ： 打开文件。
-  Ctrl + S ： 保存文件。
-  Ctrl + P ： 打印文件。 
- Ctrl + Z ： 撤销上一步操作。 
- Ctrl + Shift + Z 或 Ctrl + Y ： 重做上一步操作。 
- Ctrl + X ： 剪切所选内容。
-  Ctrl + C ： 复制所选内容。 
- Ctrl + V ： 粘贴剪切板上的内容。 
- Ctrl + F ： 查找。 
- Ctrl + H ： 替换。

## 编辑快捷键：

- Ctrl + I ：格式化文本。 
- F2 ： 跳转到下一个错误或警告。 
- Ctrl + / ： 注释或取消注释所选行。

## 导航快捷键：

- Ctrl + K ：跳转到定义。 
- Ctrl + E： 打开最近的文件。 
- Ctrl + Shift + E： 打开最近的项目。
-  Ctrl + F12： 显示当前文件的类或函数列表。
-  F4： 跳转到包含光标所在文本的定义。
-  Ctrl + Shift + F4： 返回上一个查看的位置。

## ==项目和构建快捷键：==

- Ctrl + R： 构建项目。
-  Ctrl + B： 构建并运行项目。
-  Ctrl + D： 调试项目。
-  Ctrl + Shift + B： 构建所有项目。 
- Ctrl + T： 运行单元测试。
-  F5： 重新运行应用程序。

# 

# 密码

QT密码：zhang123