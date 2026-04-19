# 1. 信号和槽 概述

> 信号槽是 Qt 框架引以为豪的机制之一。所谓信号槽，实际就是观察者模式(发布-订阅模式)。当某个事件发生之后，比如，按钮检测到自己被点击了一下，它就会发出一个信号（signal）。这种发出是没有目的的，类似广播。如果有对象对这个信号感兴趣，它就会使用连接（connect）函数，意思是，将想要处理的信号和自己的一个函数（称为槽（slot））绑定来处理这个信号。也就是说，当信号发出时，被连接的槽函数会自动被回调。这就类似观察者模式：当发生了感兴趣的事件，某一个操作就会被自动触发。

- 核心思想:回调

事件的捕捉和信号的发送都是由Qt框架内部实现的

## 1.1 信号的本质

信号是由于用户对窗口或控件进行了某些操作，导致窗口或控件产生了某个特定**事件**，这时候Qt对应的窗口类会发出某个信号，以此对用户的挑选做出反应。

因此根据上述的描述我们得到一个结论 – 信号的本质就是事件，比如：

- 按钮单击、双击

- 窗口刷新

- 鼠标移动、鼠标按下、鼠标释放

- 键盘输入

那么在Qt中信号是通过什么形式呈现给使用者的呢？

- 我们对哪个窗口进行操作, 哪个窗口就可以捕捉到这些被触发的事件。
- 对于使用者来说触发了一个事件我们就可以得到Qt框架给我们发出的某个特定信号。
- 信号的呈现形式就是函数， 也就是说某个事件产生了， Qt框架就会调用某个对应的信号函数， 通知使用者。

**在QT中信号的发出者是某个实例化的类对象，对象内部可以进行相关事件的检测。**

## 1.2 槽的本质

在Qt中**槽函数是一类特殊的功能的函数**，在编码过程中**也可以作为类的普通成员函数来使用**。之所以称之为槽函数是因为它们还有一个职责就是对Qt框架中产生的信号进行处理。

> 举个简单的例子：
>
> ```c++
> 女朋友说：“我肚子饿了！”，于是我带她去吃饭。
> ```

上边例子中相当于女朋友发出了一个信号， 我收到了信号并其将其处理掉了。

| 实例对象 | 角色       | 描述                             |
| -------- | ---------- | -------------------------------- |
| 女朋友   | 信号发出者 | 信号携带的信息: 我饿了           |
| 我       | 信号接收者 | 处理女朋友发射的信号: 带她去吃饭 |

**在Qt中槽函数的所有者也是某个类的实例对象。**

## 1.3 信号和槽的关系

在Qt中信号和槽函数都是独立的个体，本身没有任何联系，但是由于某种特性需求我们可以将二者连接到一起，好比牛郎和织女想要相会必须要有喜鹊为他们搭桥一样。在Qt中我们需要使用`QOjbect`类中的`connect`函数进二者的关联。

连接信号和槽的`connect()`函数原型如下, 其中`PointerToMemberFunction是`一个指向函数地址的指针

```c++
QMetaObject::Connection QObject::connect(
    	const QObject *sender, PointerToMemberFunction signal, 
        const QObject *receiver, PointerToMemberFunction method, 
		Qt::ConnectionType type = Qt::AutoConnection);
参数:
  - sender:   发出信号的对象
  - signal:   属于sender对象, 信号是一个函数, 这个参数的类型是函数
              指针, 信号函数地址
  - receiver: 信号接收者
  - method:   属于receiver对象, 当检测到sender发出了signal信号, 
              receiver对象调用method方法，信号发出之后的处理动作
 
//  参数 signal 和 method 都是函数地址, 因此简化之后的 connect() 如下:
connect(const QObject *sender, &QObject::signal, 
        const QObject *receiver, &QObject::method);
```

# 2. 标准信号槽使用

## 2.1 标准信号/槽

在Qt提供的很多标准类中都可以对用户触发的某些特定事件进行检测, 因此当用户做了这些操作之后, 事件被触发类的内部就会产生对应的信号, 这些信号都是Qt类内部自带的, 因此称之为标准信号。

同样的，在Qt的很多类内部为我了提供了很多功能函数，并且这些函数也可以作为触发的信号的处理动作，有这类特性的函数在Qt中称之为标准槽函数。

系统自带的信号和槽通常如何查找呢，这个就需要利用帮助文档了，比如在帮助文档中查询按钮的点击信号，那么需要在帮助文档中输入`QPushButton`

- `Public Slots`槽函数
- `Signals`信号

如果当前类的帮助文档中没有显示Signals信号,则我们应该从当前类的父类中去看看他继承下来了哪些信号,如`QPushButton`的`Contents`中只有`Public Slots`而 没有显示  `Signals`信号,因此我们从它的父类`QAbstractButton`中的`Contents`中查找到`Signals`信号

## 2.2 使用

> 功能实现: 点击窗口上的按钮,关闭窗口
>
> - 按钮:信号发出者		 	->`QPuhButton`类型
> - 窗口:信号的接受者和处理者      ->`QWidget`类型

在`mainwindow.ui`界面添加一个`Buttons`->`Push Button`,双击该按钮修改名字为:”**关闭窗口**”

并在右下角**属性**面板中的`objectName`中将值改为对应的实际函数名:`closeBtn`。

```c++
// 单击按钮发出的信号
[signal] void QAbstractButton::clicked(bool checked = false)
// 关闭窗口的槽函数
[slot] bool QWidget::close();
```

对于上边的需求只需要一句代码, 只需要写一句代码就能实现了

//发送者对象:`ui->closeBtn`,发送的信号`&QPushButton::clicked`

//接受者对象:`this`,接收的信号:`&MainWindow::close`

```c++
connect(ui->closeBtn, &QPushButton::clicked, this, &MainWindow::close);
//当用户在界面上点击名为“关闭窗口”的按钮时，按钮会发射clicked信号。Qt的信号槽机制会自动捕捉到这个信号，并调用MainWindow对象的close()槽函数，从而关闭当前窗口。
//简单来说，这行代码实现了“点击关闭按钮，程序窗口就被关闭”的功能。
```

| 参数                    | 含义           | 说明                                                         |
| :---------------------- | :------------- | :----------------------------------------------------------- |
| `ui->closeBtn`          | **发送者对象** | 这是一个指向`QPushButton`的指针，通常来自Qt设计师生成的`ui`对象。`closeBtn`是您在`.ui`文件中为按钮设置的`objectName`，通过`ui->closeBtn`可以访问到该按钮实例。 |
| `&QPushButton::clicked` | **信号**       | 这是`QPushButton`类的一个信号，当按钮被点击（鼠标按下并释放）时，该信号会被发射（emit）。`&QPushButton::clicked`表示取该信号的地址，用于明确指定要连接的信号。 |
| `this`                  | **接收者对象** | 指当前`MainWindow`对象（即窗口本身）。通常`connect`语句写在`MainWindow`的构造函数或某个成员函数中，`this`就代表了窗口实例。 |
| `&MainWindow::close`    | **槽函数**     | 这是`MainWindow`类（或其基类）的一个成员函数。`close()`实际上是`QWidget`类提供的公共槽，用于关闭窗口。由于`MainWindow`继承自`QWidget`，因此它拥有`close()`方法。`&MainWindow::close`指向该槽函数。 |

# 3. 自定义信号槽使用

Qt框架提供的信号槽在某些特定场景下是无法满足我们的项目需求的，因此我们还设计自己需要的的信号和槽，同样还是使用connect()对自定义的信号槽进行连接。

如果想要在QT类中自定义信号槽, 需要满足一些条件, 并且有些事项也需要注意:

- 要编写新的类并且让其继承Qt的某些标准类
- 这个新的子类必须从`QObject`类或者是`QObject`子类进行派生
- 在定义类的头文件中加入 `Q_OBJECT` 宏

```c++
// 在头文件派生类的时候，首先像下面那样引入Q_OBJECT宏：
class MyMainWindow : public QWidget
{
    Q_OBJECT
    ......
}
```

## 3.1 自定义信号

在Qt中信号的本质是事件, 但是在框架中也是以函数的形式存在的, 只不过信号对应的函数只有声明, 没有定义。如果Qt中的标准信号不能满足我们的需求，可以在程序中进行信号的自定义，当自定义信号对应的事件产生之后，认为的将这个信号发射出去即可（其实就是调用一下这个信号函数）。

```c++
下边给大家阐述一下, 自定义信号的要求和注意事项:
/*要求:
1. 信号是类的成员函数
2. 返回值必须是 void 类型
3. 信号的名字可以根据实际情况进行指定
4. 参数可以随意指定, 信号也支持重载
5. 信号需要使用 signals 关键字进行声明, 使用方法类似于public等关键字
6. 信号函数只需要声明, 不需要定义(没有函数体实现)
7. 在程序中发射自定义信号: 发送信号的本质就是调用信号函数
	- 习惯性在信号函数前加关键字: emit, 但是可以省略不写
	- emit只是显示的声明一下信号要被发射了, 没有特殊含义
	- 底层 emit == #define emit
*/
```

```c++
// 举例: 信号重载
// Qt中的类想要使用信号槽机制必须要从QObject类派生(直接或间接派生都可以)
class Test : public QObject
{
    Q_OBJECT
signals:
    void testsignal();
	// 参数的作用是数据传递, 谁调用信号函数谁就指定实参
	// 实参最终会被传递给槽函数
    void testsignal(int a);
};
```

## 3.2 自定义槽

槽函数就是信号的处理动作，在Qt中槽函数可以作为普通的成员函数来使用。如果标准槽函数提供的功能满足不了需求，可以自己定义槽函数进行某些特殊功能的实现。自定义槽函数和自定义的普通函数写法是一样的。

```c++
下边给大家阐述一下, 自定义槽的要求和注意事项:
/* 要求:
1. 返回值必须是 void 类型
2. 槽也是函数, 因此也支持重载
	- 槽函数需要指定多少个参数, 需要看连接的信号的参数个数
	- 槽函数的参数是用来接收信号传递的数据的, 信号传递的数据就是信号的参数
	- 举例:
    	-信号函数: void testsig(int a, double b);
    	-槽函数: void testslot(int a, double b);
	- 总结:
		-槽函数的参数应该和对应的信号的参数个数, 从左到右类型依次对应
		-信号的参数可以大于等于槽函数的参数个数 == 信号传递的数据被忽略了
			-信号函数: void testsig(int a, double b);
			-槽函数: void testslot(int a);
3. Qt中槽函数的类型是多样的,可以是:
	- 类的成员函数
	- 全局函数
	- 静态函数
	- Lambda表达式（匿名函数）

4. 槽函数可以使用关键字进行声明: slots (Qt5中slots可以省略不写)
	- public slots:
	- private slots: –> 这样的槽函数不能在类外部被调用
	- protected slots: –> 这样的槽函数不能在类外部被调用
```

```c++
// 槽函数书写格式举例
// 类中的这三个函数都可以作为槽函数来使用
class Test : public QObject
{
    public:
    void testSlot();
    static void testFunc();

    public slots:
    void testSlot(int id);
};
```

根据特定场景自定义信号槽:

```
还是上边的场景: 	  
	  女朋友说：“我肚子饿了！”，于是我带她去吃饭。
```

### 示例:带女朋友去吃麻辣烫

- 首先选择 `新建文件`->选择 新建一个类 `C++ Class`->Class name:`girlfriend` ->Base class :`QOjbect`
- 然后选择 `新建文件`->选择 新建一个类 `C++ Class`->Class name:`me` ->Base class :`QOjbect`

- 1. 在头文件 `girlfriend.h`中自定义信号

```c++
#ifndef GIRLFRIEND_H
#define GIRLFRIEND_H

#include <QObject>

class GirlFriend : public QObject
{
    Q_OBJECT
public:
    explicit GirlFriend(QObject *parent = nullptr);

signals:
    void hungry();
};

#endif // GIRLFRIEND_H
```

- 2. 在头文件 `me.h`中**声明**自定义槽函数

```c++
#ifndef ME_H
#define ME_H

#include <QObject>

class Me : public QObject
{
    Q_OBJECT
public:
    explicit Me(QObject *parent = nullptr);

public slots:
    //槽函数
    void eat();
};

#endif // ME_H
```

- 3. 在源文件 `me.cpp`中**定义**槽函数的处理动作

```c++
#include "me.h"
#include <QDebug>

Me::Me(QObject *parent)
    : QObject{parent}
{}

void Me::eat()
{
    qDebug() <<"我带你去麻辣烫...";
}
```

- 4. 在头文件`mainwindow.h`中

```c++
//添加头文件
    #include "me.h"
    #include "girlfriend.h"
//添加私有成员变量
    Me *m_me;
    GirlFriend *m_girl;
```

- 5. 在源文件`mainwindow.cpp`中	进行关联`connect`

````c++
//把两个指针对应的变量进行初始化创建
 	m_me = new Me;
    m_girl = new GirlFriend;
//将这两个对象进行关联connect
    //当信号发出者发出自定义信号,信号接受者接收信号进行eat处理
    connect(m_girl,&GirlFriend::hungry,m_me,&Me::eat);
````

因为自定义信号在Qt中不能自主触发,所以进行以下操作：

​	让一个按钮的点击信号发送后的 信号处理函数(槽函数)发送该自定义信号 



- 6. 在界面文件`mainwindow.ui`中新建一个 `PushButton`按钮,标题和`objectName`都修改为:`hungry` `ui->hungry` 得到的就是那个 **名为`hungry` 的按钮对象** 的指针。

```c++
//一旦点击该按钮,,然后在mainwindow中进行槽函数处理
```

- 7. 在头文件`mainwindow.h`中**声明**添加该按钮点击信号的槽函数`hungryClick`

```c++
	//添加hungry槽函数
    void hungryClick();
```

- 8. 在源文件`mainwindow.cpp`中**定义**该槽函数`hungryClick`

```c++
//该槽函数发送m_girl的信号
void MainWindow::hungryClick()
{
    // 发射自定义信号
    emit m_girl->hungry();
}
```

- 9. 在源文件`mainwindow.cpp`中	进行关联`connect`

```c++
// 一旦这个hungry按钮被点击(信号触发),则进行hungryClick函数处理(槽函数)
connect(ui->hungry,&QPushButton::clicked,this,&MainWindow::hungryClick);
```

-  一旦这个hungry按钮被点击(**信号触发**),则进行`hungryClick`函数处理(**槽函数**)，       [**第8步**]
- 而这个槽函数又会让`GirlFriend`类的类对象`m_girl`的发送`hungry`(**信号触发**)	[**第5步**] 
- 而随之，一旦发送该信号,则`Me`类的类对象`m_me`将会进行处理(**槽函数**)。                  [**第3步**]

# 4. 信号槽拓展

## 4.1 信号槽使用拓展

### 一个信号可以连接多个槽函数, 发送一个信号有多个处理动作

- 需要写多个`connect()`连接

- 槽函数的调用顺序和信号发射的顺序相同

- 信号的接收者可以是一个对象, 也可以是多个对象

当在`MainWindow.h`中**声明**了`void eatSlot();`并在`MainWindow.cpp`中**定义**了

```c++
void MainWindow::hungryClick()
{
    // 发射自定义信号
    emit m_girl->hungry();
}
```

后,此时一个自定义信号`hungry`同时连接了两个槽函数,即多个处理动作:`eatSlot`和`eat`

```c++
女朋友饿了
//自定义信号的处理,当点击hungry对应的控件而触发信号后,由MainWindow的槽函数hungryClick调用发射hungry信号
connect(ui->hungry,&QPushButton::clicked,this,&MainWindow::hungryClick);
//1. 当信号发出者发出信号,信号接受者MainWindow接收信号进行eatSlot处理
connect(m_girl,&GirlFriend::hungry,this,&MainWindow::eatSlot);
//2. 当信号发出者发出信号,信号接受者m_me接收信号进行eat处理
connect(m_girl,&GirlFriend::hungry,m_me,&Me::eat);

		->1. MainWindow:接收到信号,调用MainWindow::eatSlot
        ->2. 我:接收到信号,调用Me::eat
            
我带你去吃海鲜...
我带你去麻辣烫...
```



### 一个槽函数可以连接多个信号, 多个不同的信号, 处理动作是相同的

- 需要写多个connect（）连接

不管女朋友说什么（**信号触发**），处理动作(**槽函数**)都是:多喝点热水



### 信号可以连接信号

- 信号接收者可以不处理接收的信号, 而是继续发射新的信号，这相当于传递了数据, 并没有对数据进行处理

```c++
connect(const QObject *sender, &QObject::signal, 
        const QObject *receiver, &QObject::siganl-new);
```

如:取消由控件`hungry`的触发 的结果来发送 `GirlFriend`自定义信号`hungry`，而是由控件触发时，进行信号传递。（当点击hungry对应的控件而触发信号后的**槽函数改为新的信号**）即,

将：

```c++
//自定义信号的处理,当点击hungry对应的控件而触发信号后,由MainWindow的槽函数hungryClick调用发射hungry信号
connect(ui->hungry,&QPushButton::clicked,this,&MainWindow::hungryClick);
```

改为：

```c++
//信号连接信号,当点击hungry对应的控件而触发信号后,传递给GirlFriend的hungry信号
connect(ui->hungry,&QPushButton::clicked,m_girl,&GirlFriend::hungry);
```



### 信号槽是可以断开的

```c++
disconnect(const QObject *sender, &QObject::signal, 
        const QObject *receiver, &QObject::method);
```

## 4.2 信号槽的连接方式

### `Qt5`的连接方式

```c++
// 语法:
QMetaObject::Connection QObject::connect(
    	const QObject *sender, PointerToMemberFunction signal, 
        const QObject *receiver, PointerToMemberFunction method, 
		Qt::ConnectionType type = Qt::AutoConnection);

// 信号和槽函数也就是第2,4个参数传递的是地址, 编译器在编译过程中会对数据的正确性进行检测
connect(const QObject *sender, &QObject::signal, 
        const QObject *receiver, &QObject::method);
```

### `Qt4`的连接方式

> 这种旧的信号槽连接方式在`Qt5`中是支持的, 但是不推荐使用, 因为这种方式在进行信号槽连接的时候, 信号槽函数通过宏SIGNAL和SLOT转换为字符串类型。
>
> 因为信号槽函数的转换是通过宏来进行转换的，因此传递到宏函数内部的数据不会被进行检测， 如果使用者传错了数据，编译器也不会报错，但实际上信号槽的连接已经不对了，只有在程序运行起来之后才能发现问题，而且问题不容易被定位。
>

```c++
// Qt4的信号槽连接方式
[static] QMetaObject::Connection QObject::connect(
    const QObject *sender, const char *signal, 
    const QObject *receiver, const char *method, 
    Qt::ConnectionType type = Qt::AutoConnection);

connect(const QObject *sender,SIGNAL(信号函数名(参数1, 参数2, ...)),
        const QObject *receiver,SLOT(槽函数名(参数1, 参数2, ...)));
```

`Qt4`中声明槽函数必须要使用 `slots` 关键字, 不能省略。

````c++
//使用Qt4的方式进行连接
    connect(m_girl,SIGNAL(hungry()),m_me,SLOT(eat()));
//输出:
我带你去麻辣烫...
````

- `Qt4`的信号槽连接方式因为使用了宏函数, 宏函数对用户传递的信号槽不会做错误检测, 容易出bug
- `Qt5`的信号槽连接方式, 传递的是信号槽函数的地址, 编译器会做错误检测, 减少了bug的产生
- 当信号槽函数被重载之后, `Qt4`的信号槽连接方式不受影响
- 当信号槽函数被重载之后, `Qt5`中需要给被重载的信号或者槽定义函数指针

### 示例:

- 在头文件`girlfriend.h`和头文件`me.h`中:分别**声明**重载的 信号和槽函数

```c++
//在girlfriend.h中:
signals:
	void hungry(QString msg);
//在me.h中:
public slots:
	void eat(QString msg);
```

- 在源文件`me.cpp`中:实现槽函数的**定义**

```c++
void Me::eat(QString msg)
{
    qDebug() <<"我带你去吃:" <<msg;
}
```

- 在`mainwindow.cpp`中:将点击按钮信号 的 槽函数`hungryClick`方法中添加一个重载的带参自定义信号

```c++
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_me = new Me;
    m_girl = new GirlFriend;

    void(GirlFriend::*girlNone)() = &GirlFriend::hungry;
    void(GirlFriend::*girlQstr)(QString) = &GirlFriend::hungry;
    void(Me::*eatNone)() = &Me::eat;
    void(Me::*eatQStr)(QString) = &Me::eat;

    //1. 当信号发出者发出信号,信号接受者MainWindow接收信号进行eatSlot处理
    connect(m_girl,girlNone,this,&MainWindow::eatSlot);//我带你去吃海鲜...
    //2. 当信号发出者发出重载的不同信号,信号接受者m_me接收信号进行eat重载的不同处理
    connect(m_girl,girlNone,m_me,eatNone);			  //我带你去麻辣烫... 
    connect(m_girl, girlQstr, m_me, eatQStr);		  //我带你去吃: "意大利面"
    //自定义信号的处理,当点击hungry对应的控件而触发信号后,
    //由MainWindow的槽函数hungrySlot调用两个重载的发射hungry信号
    connect(ui->hungry,&QPushButton::clicked,this,&MainWindow::hungryClick);

    connect(ui->closeBtn,&QPushButton::clicked,this,&MainWindow::close);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::hungryClick()
{
    // 发射自定义信号
    emit m_girl->hungry();
    emit m_girl->hungry("意大利面");
}

void MainWindow::eatSlot()
{
    //MainWindow进行处理girlfriend发送的信号
    qDebug() <<"我带你去吃海鲜...";
}
```

- 运行结果:

```c++
我带你去吃海鲜... //eatSlot
我带你去麻辣烫... //Me::eat()
我带你去吃: "意大利面" //Me::eat(QString msg)
```

# 5. Lambda表达式

Lambda表达式是 C++ 11 最重要也是最常用的特性之一，是现代编程语言的一个特点，简洁，提高了代码的效率并且可以使程序更加灵活，Qt是完全支持c++语法的， 因此在Qt中也可以使用Lambda表达式。

## 5.1 语法格式

Lambda表达式就是一个匿名函数， 语法格式如下：

```c++
[capture](params) opt -> ret {body;};
    - capture: 捕获列表
    - params: 参数列表
    - opt: 函数选项
    - ret: 返回值类型
    - body: 函数体
```

关于Lambda表达式的细节介绍:

1. 捕获列表: 捕获一定范围内的变量
   - [] - 不捕捉任何变量
   - [&] - 捕获外部作用域中所有变量, 并作为引用在函数体内使用 (**按引用捕获**)
   - [=] - 捕获外部作用域中所有变量, 并作为副本在函数体内使用 (**按值捕获**)
     - **拷贝的副本在匿名函数体内部是只读的**
   - `[=, &foo]` - 按值捕获外部作用域中所有变量, 并按照引用捕获外部变量 `foo`(除了`foo`以引用方式捕获,其他变量都按值捕获)
   - [bar] - 按值捕获 bar 变量, 同时不捕获其他变量
   - [&bar] - 按引用捕获 bar 变量, 同时不捕获其他变量
   - [this] - 捕获当前类中的this指针
     - 让lambda表达式拥有和当前类成员函数同样的访问权限
     - 如果已经使用了 & 或者 =, 默认添加此选项
2. 参数列表: 和普通函数的参数列表一样
3. `opt` 选项  函数选项 –> 可以省略
   - `mutable`: 可以修改按值传递进来的拷贝（注意是能修改拷贝，而不是值本身）
   - `exception`: 指定函数抛出的异常，如抛出整数类型的异常，可以使用throw();
4. `ret` 选项 返回值类型:
   - 标识函数返回值的类型，**当返回值为`void`，或者函数体中只有一处`return`的地方**（此时编译器可以自动推断出返回值类型）时，这部分可以省略
5. `{}` 函数体:
   - 函数的实现，这部分不能省略，但函数体可以为空。

## 5.2 定义和调用

因为Lambda表达式是一个匿名函数, 因此是没有函数声明的, 直接在程序中进行代码的定义即可, 但是如果只定义匿名函数在程序执行过程中是不会被调用的。

```c++
    //匿名函数的定义,代码执行这个匿名函数是不会被调用的
    [](){
        //qDebug() <<"hello,我是一个lambda表达式...";
    };

	// 匿名函数的定义+调用1:
    int ret = [](int a) -> int
    {
        return a+1;
    }(100);  // 100是传递给匿名函数的参数
```

- `(100)`：**函数调用运算符**，将实参 `100` 传递给 lambda，相当于执行该匿名函数。

因此，整个表达式的效果是：定义了一个 lambda，然后以 `100` 作为实参调用它，得到结果 `101`，并赋值给 `ret`。最终 `ret` 的值为 `101`。

这种写法常用于需要临时定义一个简单函数并立即执行的场景，类似于 JavaScript 中的 **`IIFE`（立即执行函数表达式）**。



- 在Lambda表达式的捕获列表中也就是 []内部添加不同的关键字, 就可以在函数体中使用外部变量了。


````c++
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	// 在匿名函数外部定义变量
	int a=100,b=200,c=300;
	// 调用匿名函数
    [](){
        // 打印外部变量的值
        //qDebug() << "a:" << a << ", b: " << b << ", c:" << c;  // error, 不能使用任何外部变量
    }();

	//按引用的方式使用外部变量
    [&](){
        qDebug() <<"hello,我是一个lambda表达式...";
        qDebug() <<"使用引用的方式传递数据:";
        qDebug() <<"在函数内部:a+1=" <<++a<<",b+c=" <<b+c;
    }();   
	qDebug();	

	//值拷贝的方式使用外部数据
   [=](int m,int n)mutable{
        qDebug() <<"hello,我是一个lambda表达式...";
        qDebug() <<"使用拷贝的方式传递数据:";
        //拷贝的外部数据的副本在匿名函数体内部是只读的
        qDebug() <<"在函数内部:a+1=" <<++a<<",b+c=" <<b+c;
        qDebug() <<"m+1=" <<++m<<",n="<<n;
    }(1,2);
	qDebug();

	//在函数外部 a的结果仍然是原先值。
    qDebug() <<"在函数外部:a=" <<a;
}
````

```c++
hello,我是一个lambda表达式...
使用引用的方式传递数据:
在函数内部:a+1= 101 ,b+c= 500

hello,我是一个lambda表达式...
使用拷贝的方式传递数据:
在函数内部:a+1= 102 ,b+c= 500
m+1= 2 ,n= 2

在函数外部:a= 101
```

## 示例:使用匿名函数实现自定义信号的捕获

- `lambda`函数作为信号的槽函数进行使用

```c++
    void(GirlFriend::*girlNone)() = &GirlFriend::hungry;
    void(GirlFriend::*girlQstr)(QString) = &GirlFriend::hungry;
    void(Me::*eatNone)() = &Me::eat;
    void(Me::*eatQStr)(QString) = &Me::eat;	

	connect(m_girl,girlNone,this,&MainWindow::eatSlot);//我带你去吃海鲜...   
    connect(m_girl,girlNone,m_me,eatNone);			  //我带你去麻辣烫...
    connect(m_girl, girlQstr, m_me, eatQStr);		  //我带你去吃: "意大利面"

	connect(ui->hungry,&QPushButton::clicked,this,[=](){
        //发射自定义信号
        emit m_girl->hungry();
        emit m_girl->hungry("意大利面");
    });

signals:
    void hungry();
    void hungry(QString msg);
public slots:
    //槽函数
    void eat();
    void eat(QString msg);

void MainWindow::eatSlot()
{
    //MainWindow进行处理girlfriend发送的信号
    qDebug() <<"我带你去吃海鲜...";
}

void Me::eat()
{
    qDebug() <<"我带你去麻辣烫...";
}
void Me::eat(QString msg)
{
    qDebug() <<"我带你去吃:" <<msg;
}


我带你去吃海鲜...
我带你去麻辣烫...
我带你去吃: "意大利面"
```

