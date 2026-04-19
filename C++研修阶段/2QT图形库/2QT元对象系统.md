# QT核心

Qt框架的核心可以大致分为以下两个部分：

**Qt的核心特性**：

-  元对象系统 
- 信号与槽 
- 对象树 
- 事件系统

# **Qt的核心模块：**

- ==**Qt Core**：==
  
  - 提供了基本的**非GUI功能**，包括字符串处理、文件和目录操作、事件处理、多线程支持、定时 器等。
- ==**Qt GUI：**==
  
  - 用于创建丰富的图形用户界面，提供了窗口、布局、绘图、事件处理、输入处理等基本的图形 界面功能。
  
- ==**Qt Widgets：**==
  
  - 提供了一套丰富的用户界面控件，如按钮、文本框、列表框、表格、菜单等，可以用于构建传 统的桌面应用程序
- **Qt Network：**
  - 提供了网络编程相关的类和功能，包括`HTTP`、`TCP`、`UDP`、`FTP`等协议的支持，可以进行网络 通信和数据传输。
  
- **Qt SQL**：

  - 提供了数据库访问的 `API` ，支持主流的关系型数据库，如 `MySQL` 、 `SQLite` 、 `PostgreSQL` 等。

- `Qt Multimedia：`

  - 用于处理多媒体内容，包括音频、视频的播放和录制，支持多种格式和编解码器。

- **Qt Quick**：

  - 提供了一种声明性的语言（ `QML` ）和相应的 C++ `API` ，用于创建现代化的用户界面，支持动 画、效果、多点触摸等。

- **Qt QML**：（`Qt Meta-Object Language`）

  - 一种基于声明性语法的编程语言，用于构建跨平台的用户界面。是Qt框架的一部分，开发富 有交互性和动态性的应用程序界面。

- **`Qt WebEngine`：**

  - 基于 `Chromium` 的`Web`引擎，用于在应用程序中嵌入`Web`内容，支持 `HTML5` 、 `CSS` 、 `JavaScript` 等。

- **Qt XML：**

  - 提供了XML解析和生成的功能，用于处理和操作XML格式的数据。

- **Qt Bluetooth**：

  - 提供了蓝牙通信的功能，可以在应用程序中实现蓝牙设备的连接和数据交互。

- **Qt Concurrent：**

  - 提供了一套用于并行编程的工具和类。它简化了多线程和并行任务的处理，使开发者能够更轻 松地编写并行代码。

- ## OpenGL和OpenCV

  - **QT OpenGL**（Open Graphics Library）：
    - 提供了在应用程序中使用 `OpenGL` 和 `OpenGL ES` 进行 3D 图形渲染的功能。它允许开发者在Qt 应用程序中创建和显示 OpenGL 场景，以及与图形进行交互。
    - 是基于C的**图形库**，主要用于三维图形的渲染。
  - **OpenCV**  (Open Source Computer Vision Library):
    - OpenCV主要是提供图像处理和视频处理的基础算法库，还涉及一些机器学习的算法。比如你想实现视频的降噪、运动物体的跟踪、目标（比如人脸）的识别这些都是CV的领域
    - 是基于C的**计算机视觉库**，主要用于计算机视觉的处理。
  - 前者专注于从采集到的视觉图像中获取信息，是用机器来理解图像；后者是用机器绘制合适的视觉图像给人看。

- **QT Test：**

  - 提供了单元测试框架和模拟鼠标和键盘事件的功能。它可以帮助开发者编写和执行单元测试， 以确保代码的正确性和稳定性。此外，Qt Test还提供了与 Visual Studio 和 KDevelop 等集 成开发环境的整合支持。  

# ==-------------------==

# 知乎：Qt中的元对象系统

**由于C++的RTTI机制只能提供有限的类型信息，于是Qt构建了自己的元对象系统（Meta-Object）。**使用该系统的基类QObject所创建的派生类对象，可以在运行期获取该对象的类名、父类名、枚举类型以及有哪些成员变量、有哪些成员函数等信息。基于这些信息，Qt实现了强大的信号槽机制。

## 1. 不完美的C++

![img](https://pic3.zhimg.com/v2-28a5a95816ce7c333e198d6c1f35cef4_1440w.jpg)

假设有一个形状基类Shape和三个派生类：代表正方形的Square类、代表三角形的Triangle类和代表圆形的Circle类。我们知道基类类型的指针可以指向派生类的对象，如下代码所示。虽然都是Shape *类型的指针，但是调用draw()函数时却是调用各自的派生类方法。所有的这些特性都是依赖于C++的多态性机制。

```cpp
Shape *shape1 = new Square;
Shape *shape2 = new Triangle;
Shape *shape3 = new Circle;
```

### 1.1 为什么需要RTTI？

一般情况下我们并不需要使用RTTI机制，C++的继承、多态性就能够解决大多数问题。**但是在编写程序调试器、对象I/O库时就特别需要知道类的信息**。

多态性机制并不能解决这样的场景。假设用一个容器保存这三个指针，此时想更改容器中“圆形”对象的颜色。但是容器中的每一个元素都是Shape *类型的数据，从表面上看是无法判断所指对象的类型，自然就找不到哪个元素是Circle类型的。**由于C++是静态类型语言，有关类的信息只在编译期被使用，编译后就不再保留，因此程序运行时无法获取类的信息。这时就需要使用「运行期类型信息」，即RTTI（Run-Time Type Information）**。一般拥有程序运行时保存对象类型信息能力的语言，我们就称该语言支持RTTI。C++标准定义了dynamic_cast和typeid两个关键字用于支持RTTI机制。

dynamic_cast的作用是将指向基类对象的指针转换为指向派生类对象的指针，如果转换失败则返回NULL。因此，dynamic_cast唯一的功能就是判断一个对象具有哪些类型。例如有4个类GrandFather、Father、Son和GrandSon，右边的类分别是左边类的派生类。此时有一个类型为GrandFather *的指针p，为了判断p所指对象是否具有Son的类型，可以使用语句Son *son = dynamic_cast<Son*>(p)。如果返回的son不为NULL，则p所指的对象具有Son类型。同理我们也可以得到该对象是否具有Father、GrandSon类型，

但是仅仅知道这些是不够的。因为有时候某个指针所指的对象行为不正常，如果无法知道所指对象的实际类型，就必须审查该基类及其所有派生类的代码。反过来，如果我们能够知道该对象的实际类型，那就只需要审查该类型的代码即可。typeid的作用是返回类型的名字。因此，typeid的功能就是在dynamic_cast的范围内进一步的确定指针所指对象的实际类型。

### 1.2 有限的RTTI能力

完整的描述一个类型需要很多信息，例如类的名字、有哪些父类、有哪些成员变量、有哪些成员函数、哪些是public的、哪些是private的、哪些是protected的等等。有时候一个工程项目可能包含成千上万个类，完整的保存这些信息将会消耗大量的内存资源。为了节省内存，C++标准约定typeid只能返回类名。因此，仅靠dynamic_cast和typeid两个关键字提供的类型信息实在有限。

**即使仅提供有限的类型信息，RTTI的实现仍然耗费了很大的时间和存储空间，这就会降低程序的性能**。因此，早期的标准委员会并没有将该特性纳入C++中。后来是出于以下的考虑才加入的：（1）RTTI可以作为一个编译选项提供给开发人员。如果不需要用到这个特性，可以手动禁用这个特性。（2）编译器只需要做很少的改动就可以实现RTTI。

另一方面，虽然C++定义了dynamic_cast和typeid两个关键字，但并没有说明如何实现这两个关键字。这就造成了不同的编译器的实现不同，更别说提供RTTI功能的库千差万别。**由此导致的最大问题就是程序的可移植性差，项目之间无法完美兼容**。

为了解决这些问题，Qt没有采用C++的RTTI机制，却提供了更为强大的元对象（Meta-Object）系统机制来实现动态获取类型信息。

## 2. 强大的Qt元对象系统

Qt元对象系统的强大在于“**即使编译器不支持RTTI，我们也能动态获取类型信息**”。例如在任何时候调用QMetaObject::className()函数都会返回类的名称。由于程序运行时保留了类型信息，那么自然就可以进行父子类之间的动态转换。qobject_cast()相比dynamic_cast()强制转换安全得多，而且速度更快。因此，对于QObject派生类之间的转换，推荐使用qobject_cast()。如下所示：

```cpp
QObject *obj = new QWidget;
QWidget *widget = qobject_cast<Qwidget *>(obj);
```

由于元对象系统提供了完整的动态类型信息，利用这些信息Qt构建了**信号槽机制**和**动态属性系统**。其中，信号槽机制是核心功能。

### 2.1 信号槽机制

**信号槽机制是Qt最大的特色功能，用于实例对象之间的通信**。信号和槽都是成员函数，绑定一个对象的信号和另一个对象的槽函数，当前者发射出信号时，与之相对应的会触发后者的槽函数，这样两个对象间就完成了一次通信过程。如下所示：

```cpp
class ObjectA : public QObject
{
    Q_OBJECT
public:
    explicit ObjectA(QObject *parent = nullptr);

    void setValue(const int& value)
    {
        emit valueChange(value);
    }

signals:
    void valueChange(const int& v);
};

class ObjectB : public QObject
{
    Q_OBJECT
public:
    explicit ObjectB(QObject *parent = nullptr);

public slots:
    void on_receive(const int& v)
    {
        qDebug() << "Receive value:" << v;
    }
};

ObjectA objA;
ObjectB objB;
connect(&objA, &ObjectA::valueChange, &objB, &ObjectB::on_receive);
objA.setValue(666);
```

**在没有信号槽机制的时代，C++对象间的交互一般使用回调函数来实现**。使用某对象时，用指针指向另一个对象的函数，这个函数就称为回调函数。使用回调函数有个弊端，当某个对象被多个对象通信时，需要一个容器来存放多个对象的回调函数。维护这个容器使得代码编写效率低、扩展性弱。

基于Qt元对象系统扩展的信号槽机制，使得处理对象间通信变得更加高效、灵活。

### 2.2 动态属性系统

利用元对象系统所提供的动态类型信息，Qt还构建了一套强大的属性系统。**由于元对象系统的特点，这就保证了Qt属性系统是独立于编译器和平台的**。不仅如此，我们还可以使用`Q_PROPERTY()`宏来定义编译期的静态属性，使用`setProperty`()函数动态添加属性。

在C++中是没有属性概念的，只有成员变量。因为面向对象的思想是抽象封装，属性是类给外部展示的特性。而成员变量属于类的内部信息，直接暴漏出去就破坏了封装性，因为使用者可以对类特性进行直接修改。而属性将取值、赋值的细节进行了封装，外部只能使用它而不能控制它。

通常在界面插件开发、QML中使用属性系统，例如在Qt Creator的设计界面中，控件都是以属性的方式暴露给外部使用者。

![img](https://pica.zhimg.com/v2-b626925ee6c2b33fcf156db5f1512ad8_1440w.jpg)

## 3. 使用元对象系统的3个条件

使用元对象系统需要满足三个条件：

- 只有**QObject派生类**才可以使用元对象系统特性。
- 在类声明前使用**Q_OBJECT()**宏来开启元对象功能。
- 使用**Moc工具**为每个QObject派生类提供实现代码。

# ==----------------------==

# 元对象系统、信号与槽 

## 元对象系统基本概念

元对象系统是QT对原有c++的一些**扩展**，主要是为**实现信号和槽机制**运行时类型信息和动态属性系统等 而引入的，**信号与槽的机制是QT的核心机制**。

- ## **使用元对象系统的功能的三个条件**

  - 该类必须继承自 `QObject` 类 - **所有QT原生框架的基类**

  - 必须在类声明的私有区域添加 `Q_OBJECT` 宏，该宏用于**启动元对象特性**，然后便可使用动态特性、信号和槽等功能了

  - 元对象编译器( `moc` )为每个 `QObject` 的子类，提供实现了元对象特性所必须的代码

QT中的元对象系统 `Q_OBJECT` 并不是`C++`标准代码，因此在使用时需要`QT`的 `MOC` （元对象编译器）进行 预处理， `MOC` 会在编译时期读取C++代码中的特定宏（如 `Q_OBJECT` ），再由标准的C++编译器进行重新 编译。

- QT moc ->预处理(moc读取C++代码中的特定宏) ->C++重新编译(moc处理完成后交给C++编译器重新)编译

## Moc工具

Qt中有一个工具**专门为包含 `Q_OBJECT` 宏的类**来**生成额外的元对象代码**的。

这个工具叫`moc`全称是 `Meta-Object Compiler(`**元对象编译器**)，它是一个工具(类似于 `qmake`)。

该工具读取并分析 `C++`源文件，若发现一个或多个包含了 `Q_OBJECT` 宏的类的声明，则会生成另外一个 包含了 `Q_OBJECT` **宏实现代码**的 **C++源文件(**该源文件通常名称为 `moc_*.cpp` ) 。

这个新的源文件要么被`#include` 包含到类的源文件中，要么被编译键接到类的实现中(通常是使用的此种 方法)。

这个源文件中包含了用于**存储信号**和**槽函数指针**的结构、实现信号发射和槽调用的函数等内容。

**注意：新文件不会“替换”掉旧的文件，而是与原文件一起编译。**

## 元对象

**元对象**是描述另一个对象结构的对象，通常用于**获取对象的类型信息、属性和方法**，特别是在Qt等框架中具有重要作用。

- ## 元对象的定义

**元对象（Meta Object）是一个用于描述其他对象的结构的对象**。它提供了关于对象的类型、属性、方法等信息。在编程中，元对象系统允许开发者在运行时查询和操作对象的特性，而不需要在编译时知道这些信息。

- ## 在Qt中的应用

在Qt框架中，元对象系统是其核心特性之一，主要通过 `QObject` 类及其派生类实现。以下是元对象系统的一些关键功能：

1. **信号与槽机制**：元对象系统支持信号和槽的概念，使得对象之间可以进行无缝的通信。当一个对象的状态发生变化时，它可以发出信号，其他对象可以连接到这个信号并执行相应的槽函数。
2. **属性系统**：元对象系统允许对象具有属性，这些属性可以在运行时被访问和修改。属性系统提供了一种方便的接口，使得对象的状态可以被直接操作。
3. **运行时类型信息**：通过元对象，开发者可以在运行时获取对象的类名、父类名、成员函数和成员变量等信息。这对于调试和动态类型识别非常有用。

## `Q_OBJECT`宏

`Q_OBJECT` 的使用：**必须要在类中定义元对象系统** `Q_OBJECT` 宏才能使用（在类定义时，如果没有指定 `public` 或者 `private` ，则默认为 `private`(私有)） 。

程序运行时，`moc`会扫描此类，并生成元对象信息，包括但不限于类名、父类、属性、信号、槽函数 等；

- ## `Q_OBJECT` 的特性：

  - **类型信息**：Qt使用元对象系统来存储关于对象的信息，如类名和父类。 

  - **属性系统**：支持动态的属性机制，允许在运行时查询和修改对象的属性。 

  - **信号和槽的动态连接**：元对象系统允许在运行时创建和解除信号与槽之间的连接。

`setProperty` 设置注册的动态属性。

## 反射机制

`reflection` 模式(反射模式或反射机制)：是指**在运行时，能获取任意一个类对象的所有类型信息、属 性、成员函数等信息的一种机制。**

### Qt 具体实现反射机制的方法

`QMetaObject` ：类描述了 `QObject` 及其派生类对象的所有元信息，该类是 Qt 元对象系统的核心类， 因此可以说 `QMetaObject` 类的对象是 Qt 中的元对象。

`QMetaObject` 类对整个类对象进行描述，比如要获取成员函数的函数名，其代码如下

```c++
QMetaMethod qm = metaObject->method(1); //获取索引为 1 的成员函数
qDebug()<<qm.name()<<"\n"; //输出该成员函数的名称。
```

**项目创建**：

![屏幕截图 2025-12-28 163614](D:\Typora\typora_work\C++研修阶段\2QT图形库\屏幕截图 2025-12-28 163614.png)

![屏幕截图 2025-12-28 163754](D:\Typora\typora_work\C++研修阶段\2QT图形库\屏幕截图 2025-12-28 163754.png)

![屏幕截图 2025-12-28 164045](D:\Typora\typora_work\C++研修阶段\2QT图形库\屏幕截图 2025-12-28 164045.png)

**添加新文件**：

![屏幕截图 2025-12-28 164201](D:\Typora\typora_work\C++研修阶段\2QT图形库\屏幕截图 2025-12-28 164201.png)

**选择C++模版：**

C++ Class 包含 C++源文件 、 C++头文件

C++ Source File 只包含C++源文件

C+++ Header File  只包含C++头文件

![屏幕截图 2025-12-28 164345](D:\Typora\typora_work\C++研修阶段\2QT图形库\屏幕截图 2025-12-28 164345.png)

**为模版起名**：

![屏幕截图 2025-12-28 164636](D:\Typora\typora_work\C++研修阶段\2QT图形库\屏幕截图 2025-12-28 164636.png)

**创建完毕**：

- 源文件

![屏幕截图 2025-12-28 164905](D:\Typora\typora_work\C++研修阶段\2QT图形库\屏幕截图 2025-12-28 164905.png)

- 头文件

![屏幕截图 2025-12-28 165151](D:\Typora\typora_work\C++研修阶段\2QT图形库\屏幕截图 2025-12-28 165151.png)

- pro

![屏幕截图 2025-12-28 165105](D:\Typora\typora_work\C++研修阶段\2QT图形库\屏幕截图 2025-12-28 165105.png)



**引入QT核心模块：**

![屏幕截图 2025-12-28 165330](D:\Typora\typora_work\C++研修阶段\2QT图形库\屏幕截图 2025-12-28 165330.png)

**使用元对象：**

![屏幕截图 2025-12-28 165814](D:\Typora\typora_work\C++研修阶段\2QT图形库\屏幕截图 2025-12-28 165814.png)

**小技巧：按住alt + 回车，可以给出提示，选择第三个**



**创建C++ Source File 源文件main:**

![屏幕截图 2025-12-28 170417](D:\Typora\typora_work\C++研修阶段\2QT图形库\屏幕截图 2025-12-28 170417.png)

**在main.cpp中编译并运行：**

![屏幕截图 2025-12-28 171753](D:\Typora\typora_work\C++研修阶段\2QT图形库\屏幕截图 2025-12-28 171753.png)



#### 解决Qt项目在编译时无法找到依赖文件

![屏幕截图 2025-12-28 171345](D:\Typora\typora_work\C++研修阶段\2QT图形库\屏幕截图 2025-12-28 171345.png)

**QMAKE_PROJECT_DEPTH = 0**

观察到`D:\QTproject\MetaObject\build\Desktop_Qt_6_5_3_MSVC2019_64bit-Debug\debug`

文件夹下面有以下内容：

![屏幕截图 2025-12-28 171703](D:\Typora\typora_work\C++研修阶段\2QT图形库\屏幕截图 2025-12-28 171703.png)

#### 使用 Qt 反射机制的条件

- 需要继承自 QObject 类，并需要在类之中加入 Q_OBJECT 宏。
- 注册**成员函数**：若希望普通成员函数能够被反射，需要在函数声明之前加入**Q_INVOKABLE** 宏。
- 注册**成员变量**：若希望成员变量能被反射，需要使用 **Q_PROPERTY** 宏。

注册成员函数/变量

- 函数声明之前加入**Q_INVOKABLE** 宏。

  - ```c++
    Q_INVOKABLE int sum();
    ```

    - 标记该成员函数可以通过元对象系统调用

    - 可以在QML中直接调用此函数

    - 可以通过`QMetaObject::invokeMethod()`动态调用

    - 注意：如果没有`Q_INVOKABLE`，普通的成员函数不能在QML中调用

-  变量声明之前加入**Q_PROPERTY** 宏。

  - ```c++
    Q_PROPERTY(type name
    READ getter
    WRITE setter
    NOTIFY signalName	：属性变化时发出的信号
    RESET resetFunction ：重置属性的函数
    STORED true/false   ：是否持久化存储
    DESIGNABLE true/false ：是否在设计器中可见
    SCRIPTABLE true/false ：是否在脚本中可用
    USER bool：是否是用户可编辑属性          
    CONSTANT：常量属性
    FINAL：不可被子类重写) 
    ```

  - ```c++
    Q_PROPERTY(int age READ getAge WRITE setAge)
    ```

    - 声明一个可以通过元对象系统访问的属性
    - 语法：`Q_PROPERTY(type name READ getFunction WRITE setFunction)`
    - 这里的属性名为`age`
    - `READ getAge`：指定读取函数
    - `WRITE setAge`：指定写入函数

```c++
#ifndef METAOBJECT_H
#define METAOBJECT_H

#include <QObject>

//使用元对象系统：第一条件继承Qobject类
class MetaObject :public QObject
{
    //使用元对象系统：第二条件使用Q_OBJECT宏
    Q_OBJECT
public:
    MetaObject();
    Q_INVOKABLE int sum();
    int getAge() const;
    void setAge(int a)
    {
        age = a;
    }
private:
    Q_PROPERTY(int agre READ getAge WRITE setAge);
    int age;
};

#endif // METAOBJECT_H
```

#### Qt 反射机制实现原理

Q_OBJECT 宏展开之后有一个虚成员函数 `metaObject()` ，该函数会返回一个指向 `QMetaObject` 类型的指针

```c++
    virtual const QMetaObject *metaObject() const; \
```

Qt 的 `moc`(元对象编译器) 会完成以下工作

- 为 Q_OBJECT 宏展开后所声明的成员函数的成生实现代码 
- 识别Qt中特殊的关键字及宏，比如识别出Q_PROPERTY宏、Q_INVOKABLE宏、slot、signals等

#### `qobject_cast` 函数

位于头文件："`qobject.h`"   是一个**内联模版函数**

```c++
template <class T>
inline T qobject_cast(QObject *object)
{
    typedef typename std::remove_cv<typename std::remove_pointer<T>::type>::type ObjType;
    static_assert(QtPrivate::HasQ_OBJECT_Macro<ObjType>::Value,
                    "qobject_cast requires the type to have a Q_OBJECT macro");
    return static_cast<T>(ObjType::staticMetaObject.cast(object));
}
```

该函数类似于 C++中的 `dynamic_cast` ，但执行速度比 `dynamic_cas` t 更快，且不需要C++的 `RTTI` (运行阶段类型识别)的支持，但 `qobject_cast` 仅适用于 `QObject` 及其派生类。

主要作用是把源类型 `QObject` 转换为尖括号中的目标类型 `DesType` (或者子类型)，并返回指向目标类 型的指针，若转换失败，则返回 0。或者说源类型 `QObject` 属于目标类型 `DestType` (或其子类型)，则 返回指向目标类型的指针，否则返回 0。

```c++
DestType* qobject_cast<DestType*>(QObject *p);
```

使用 qobject_cast 的条件：**目标类型 `DestType` 必须继承(直接或间接)自 `QObject` ，并使用 Q_OBJECT 宏。**

## QMetaObject 类

QMetaObject 类：描述一个对象的元信息。

- **`indexOfMethod`**

  ```c++
  int indexOfMethod(const char *method) const;
  ```

  - 获取一个成员函数，成功返回函数的索引号，否则返回-1。

  - ```c++
    void f(int a,int b)
    {}
    xx.indexOfMethod("f(int,int”)");
    //以下形式都不是正确的形式，“f(int a, int b)”、“void f(int, int)”、 “f”、"void f"等。
    ```

- **`indexOfSignal`**

  ```c++
  int indexOfSignal(const char * s) const;
  ```

  - 返回信号 s 的索引号，否则返回-1，若指定的函数存在，但不是信号，仍返回-1。

- **`indexOfConstructor`**

  ```c++
  int indexOfConstructor(const char *c) const;
  ```

  - 返回构造函数 c 的索引号，否则返回-1

- **`constructorCount`**

  ```c++
  int constructorCount() const ;
  ```

  - 返回构造函数的数量。

- **`methodCount`**

  ```c++
  int methodCount(int i)const;
  ```

  - 返回函数的数量，包括基类中的函数、信号、槽和普通成员函数。

- **`constructor`**

  ```c++
  QMetaMethod constructor(int i)const;
  ```

  - 返回指定索引 i 处的构造函数的元数据。

- **`methodOffset`**

  ```c++
  int methodOffset() const;
  ```

  - 返回父类中的所有函数的总和，也就是说返回的值是该类中的第一个成员函数的索引位置。

- **`method`**

  ```c++
  QMetaMethod method(int i) const;
  ```

  - 返回指定索引 i 处的函数的元数据

- **`className`**

  ```c++
  const char* className() const;
  ```

  - 获取类的名称

- **`superClass`**

  ```c++
  const QMetaObject* superClass() const;
  ```

  - 返回父类的元对象，若没有这样的对象则返回 0。

- **`inherits`**

  ```c++
  bool inherits(const QMetaObject* mo) const;
  ```

  - 若该类继承自父类描述的类型，则返回 true，否则返回 false。类被认为继承自身



## QMetaMethod 类

位于：”`qmetaobject.h`“

`QMetaMethod` 类：在 Qt 的反射机制中用于描述类的成员方法的元信息。

- **`MethodType`**

  ```c++
  enum MethodType{Method, Signal, Slot, Constructor}
  ```

  - 此枚举用于描述函数的类型，即：普通成员函数(`Method`)、信号(`Signal`)、槽(`Slot`)、构造函数 (`Constructor`)。

- **`Access`**

  ```c++
  enum Access{Private, Protected, Public}
  ```

  - 此枚举主要用于描述函数的访问级别(私有的、受保护的、公有的)

- **`methodSignature`**

  ```c++
  QByteArray methodSignature() const;
  ```

  - 返回函数的签名(qt5.0)

- **`methodType`**

  ```c++
  MethodType methodType() const;
  ```

  - 返回函数的类型(信号、槽、成员函数、构造函数)

- **`parameterCount`**

  ```c++
  int parameterCount() const
  ```

  - 返回函数的参数数量

- **`parameterNames`**

  ```c++
  QList<QByteArray> parameterNames() const;
  ```

  - 返回函数参数名称的列表。

- **`parameterType`**

  ```c++
  QByteArray parameterType() const;
  ```

  - 获取以一个参数的类型

- **`access`**

  ```c++
  Access access() const;
  ```

  - 返回函数的访问级别(私有的、受保护的、公有的)

- **`typeName`**

  ```c++
  const char * typeName() const;
  ```

  - 返回函数的返回类型的名称。

# 作业：

 创建一个对象，然后将成员变量和函数注册反射机制，利用元对象系统，获取对象元信息。