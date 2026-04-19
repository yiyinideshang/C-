# 4. QMainWindow

`QMainWindow`是标准基础窗口中结构最复杂的窗口, 其组成如下:

- 提供了菜单栏, 工具栏, 状态栏, 停靠窗口
- 1个菜单栏`menubar`: 只能有一个, 位于窗口的最上方
- N个工具栏: 可以有多个, 默认提供了一个, 窗口的上下左右都可以停靠
- 1个状态栏`statusbar`: 只能有一个, 位于窗口最下方
- N个停靠窗口: 可以有多个, 默认没有提供, 窗口的上下左右都可以停靠
- 1个核心窗口

# 4.1 菜单栏

- 添加菜单项

关于顶级菜单可以直接在`UI`窗口中双击, 直接输入文本信息即可, 对应子菜单项也可以通过先双击在输入的方式完成添加, 但是这种方式不支持中文的输入。

1. > 一般情况下, 我们都是先在外面创建出`QAction`对象, 然后再将其拖拽到某个菜单下边, 这样子菜单项的添加就完成了。

2. > 通过代码的方式添加菜单或者菜单项

```c++
// 给菜单栏添加菜单
QAction *QMenuBar::addMenu(QMenu *menu);
QMenu *QMenuBar::addMenu(const QString &title);
QMenu *QMenuBar::addMenu(const QIcon &icon, const QString &title);

// 给菜单对象添加菜单项(QAction)
QAction *QMenu::addAction(const QString &text);
QAction *QMenu::addAction(const QIcon &icon, const QString &text);

// 添加分割线
QAction *QMenu::addSeparator();
```

- 菜单项 QAction 事件的处理

> 单击菜单项, 该对象会发出一个信号

```c++
// 点击QAction对象发出该信号
[signal] void QAction::triggered(bool checked = false);
```

示例代码:

```c++
// save_action 是某个菜单项对象名, 点击这个菜单项会弹出一个对话框
connect(ui->save_action, &QAction::triggered, this, [=]()
{
      QMessageBox::information(this, "Triggered", "我是菜单项, 你不要调戏我...");
});
```

# 4.2 工具栏

## 4.2.1 添加工具按钮

窗口中的工具栏我们经常见到, 并不会为此感到陌生, 那么如何往工具栏中添加工具按钮呢? 一共有两种方式, 这里依次为大家进行介绍。

方式1：先创建`QAction`对象, 然后拖拽到工具栏中, 和添加菜单项的方式相同

方式2：如果不通过`UI`界面直接操作，那么就需要调用相关的`API`函数了

````c++
// 在QMainWindow窗口中添加工具栏
void QMainWindow::addToolBar(Qt::ToolBarArea area, QToolBar *toolbar);
void QMainWindow::addToolBar(QToolBar *toolbar);
QToolBar *QMainWindow::addToolBar(const QString &title);

// 将Qt控件放到工具栏中
// 工具栏类: QToolBar
// 添加的对象只要是QWidget或者启子类都可以被添加
QAction *QToolBar::addWidget(QWidget *widget);

// 添加QAction对象
QAction *QToolBar::addAction(const QString &text);
QAction *QToolBar::addAction(const QIcon &icon, const QString &text);

// 添加分隔线
QAction *QToolBar::addSeparator()
````

通过代码的方式对工具栏进行操作

## 4.2.2 工具栏的属性设置

> 在`UI`窗口的树状列表中, 找到工具栏节点, 就可以到的工具栏的属性设置面板了, 这样就可以根据个人需求对工具栏的属性进行设置和修改了。

在Qt控件的属性窗口中对应了一些属性, 这些属性大部分都应了一个设置函数

- 在对应的类中函数名叫什么?
  - 规律: `set+属性名 == 函数名`
- 某些属性没有对应的函数, 只能在属性窗口中设置

# 4.3 状态栏

> 一般情况下, 需要在状态栏中添加某些控件, 显示某些属性, 使用最多的就是添加标签 QLabel

```c++
// 类型: QStatusBar
void QStatusBar::addWidget(QWidget *widget, int stretch = 0);

[slot] void QStatusBar::clearMessage();
[slot] void QStatusBar::showMessage(const QString &message, int timeout = 0);
```

- 相关操作

```c++
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
        
	//状态栏添加子控件
    ui->statusBar->showMessage("我是状态栏...",3000);//显示信息,3秒后消失
    //按钮
    QPushButton* button = new QPushButton("按钮");
    ui->statusBar->addWidget(button);
    //标签
    QLabel* label = new QLabel("hello world");
    ui->statusBar->addWidget(label);


    QTimer::singleShot(5000,this,[=](){//5秒后发射信号,显式按钮和标签
        button->show();
        label->show();
    });
}
```

# 4.4 停靠窗口

- QDockWidget

> 停靠窗口可以通过鼠标拖动停靠到窗口的上、下、左、右，或者浮动在窗口上方。如果需要这种类型的窗口必须手动添加，`如果在非QMainWindow类型的窗口中添加了停靠窗口, 那么这个窗口是不能移动和浮动的。`
>
> 浮动窗口在工具栏中， 直接将其拖拽到`UI`界面上即可。

停靠窗口也有一个属性面板, 我们可以在其对应属性面板中直接进行设置和修改相关属性。

# 5. 资源文件 .`qrc`

> 资源文件顾名思义就是一个存储资源的文件，在Qt中引入资源文件好处在于他能提高应用程序的部署效率并且减少一些错误的发生。
> `在程序编译过程中， 添加到资源文件中的文件也会以二进制的形式被打包到可执行程序中，这样这些资源就永远和可执行程序捆绑到一起了，不会出现加载资源却找不到的问题。`
> 虽然资源文件优势很明显，但是它也不是万能的，资源文件中一般添加的都是比较小的资源，比如： 图片，配置文件，`MP3` 等，如果是类似视频这类比较大的文件就不适合放到资源文件中了。

比如我们需要给某个窗口设置图标, 代码如下:

```c++
// 创建图标对象
QIcon::QIcon(const QString &fileName);
// QWidget类的 公共成员函数
void setWindowIcon(const QIcon &icon);

// 给窗口设置图标
// 弊端: 发布的 exe 必须要加载 d:\\pic\\1.ico 如果当前主机对应的目录中没有图片, 图标就无法被加载
// 发布 exe 需要额外发布图片, 将其部署到某个目录中
setWindowIcon(QIcon("d:\\pic\\1.ico"));
```

> 我们可以使用资源文件解决上述的弊端, 这样发布应用程序的时候直接发布exe就可以, 不需要再额外提供图片了。

下面介绍一下关于资源文件的创建步骤:

- 给资源添加前缀

> 一个资源文件中可以添加多个前缀, 前缀就是添加的资源在资源文件中的路径, 前缀根据实际需求制定即可, 路径以 `/` 开头
