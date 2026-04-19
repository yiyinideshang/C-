# 1. QWidget

![窗口类](D:\Typora\typora_work\C++研修阶段\2QT图形库\窗口类.png)

`QWidget`类是所有窗口类的父类(控件类是也属于窗口类), 并且`QWidget`类的父类的QObject, 也就意味着所有的窗口类对象只要指定了父对象, 都可以实现内存资源的自动回收。
在 [QT1入门.md](QT1入门.md) 章节中已经为大家介绍了`QWidget`的一些特点, 为了让大家能够对这个类有更深入的了解, 下面来说一说这个类常用的一些`API`函数。
关于这个窗口类的属性介绍, 请参考`容器控件之QWidget`。

## 1.1 设置父对象

```c++
// 构造函数
QWidget::QWidget(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

// 公共成员函数
// 给当前窗口设置父对象
void QWidget::setParent(QWidget *parent);
void QWidget::setParent(QWidget *parent, Qt::WindowFlags f);
// 获取当前窗口的父对象, 没有父对象返回 nullptr
QWidget *QWidget::parentWidget() const;
```

## 1.2 窗口位置

````c++
//------------- 窗口位置 -------------
// 得到相对于当前窗口父窗口的几何信息, 边框也被计算在内
QRect QWidget::frameGeometry() const;
// 得到相对于当前窗口父窗口的几何信息, 不包括边框
const QRect &geometry() const;
// 设置当前窗口的几何信息(位置和尺寸信息), 不包括边框
void setGeometry(int x, int y, int w, int h);
void setGeometry(const QRect &);
    
// 移动窗口, 重新设置窗口的位置
void move(int x, int y);
void move(const QPoint &);
````

`QRect`信息参看: [QT2基础数据类型.md](QT2基础数据类型.md) 

```c++
// 通过 左上角坐标(x, y), 和 矩形尺寸(width, height) 构造一个矩形对象
QRect::QRect(int x, int y, int width, int height);
```

## 关于封装信息:

````c++
int x,int y ->QPoint

int x1,int y1,int x2,int y2 ->QLine

int width,int height ->QSize

int x,int y,int width,int height ->QRect
````

## 1.3 窗口尺寸

```c++
//------------- 窗口尺寸 -------------
// 获取当前窗口的尺寸信息
QSize size() const
// 重新设置窗口的尺寸信息
void resize(int w, int h);
void resize(const QSize &);
// 获取当前窗口的最大尺寸信息
QSize maximumSize() const;
// 获取当前窗口的最小尺寸信息
QSize minimumSize() const;
// 设置当前窗口固定的尺寸信息
void QWidget::setFixedSize(const QSize &s);
void QWidget::setFixedSize(int w, int h);
// 设置当前窗口的最大尺寸信息
void setMaximumSize(const QSize &);
void setMaximumSize(int maxw, int maxh);
// 设置当前窗口的最小尺寸信息
void setMinimumSize(const QSize &);
void setMinimumSize(int minw, int minh);


// 获取当前窗口的高度    
int height() const;
// 获取当前窗口的最小高度
int minimumHeight() const;
// 获取当前窗口的最大高度
int maximumHeight() const;
// 给窗口设置固定的高度
void QWidget::setFixedHeight(int h);
// 给窗口设置最大高度
void setMaximumHeight(int maxh);
// 给窗口设置最小高度
void setMinimumHeight(int minh);

// 获取当前窗口的宽度
int width() const;
// 获取当前窗口的最小宽度
int minimumWidth() const;
// 获取当前窗口的最大宽度
int maximumWidth() const;
// 给窗口设置固定宽度
void QWidget::setFixedWidth(int w);
// 给窗口设置最大宽度
void setMaximumWidth(int maxw);
// 给窗口设置最小宽度
void setMinimumWidth(int minw);
```

## 1.4 窗口标题和图标

````c++
//------------- 窗口图标 -------------
// 得到当前窗口的图标
QIcon windowIcon() const;
// 构造图标对象, 参数为图片的路径
QIcon::QIcon(const QString &fileName);
// 设置当前窗口的图标
void setWindowIcon(const QIcon &icon);

//------------- 窗口标题 -------------
// 得到当前窗口的标题
QString windowTitle() const;
// 设置当前窗口的标题
void setWindowTitle(const QString &);
````

## 1.5 信号

```c++
QWidget::setContextMenuPolicy(Qt::ContextMenuPolicy policy);
// 窗口的右键菜单策略 contextMenuPolicy()是一个枚举值,当参数设置为 Qt::CustomContextMenu 时,按下鼠标右键发射customContextMenuRequested信号
[signal] void QWidget::customContextMenuRequested(const QPoint &pos);
```

```c++
enum Qt::ContextMenuPolicy
```

![contexmenupolicy](D:\Typora\typora_work\C++研修阶段\2QT图形库\contexmenupolicy.png)

```c++
// 窗口图标发生变化, 发射此信号
[signal] void QWidget::windowIconChanged(const QIcon &icon);
// 窗口标题发生变化, 发射此信号
[signal] void QWidget::windowTitleChanged(const QString &title);
```

基于窗口策略实现右键菜单具体操作请参考 `Qt右键菜单的添加和使用` 和`使用示例2`

## 1.6 槽函数

###  窗口状态

```c++
// 判断窗口是否可用
bool QWidget::isEnabled() const; // 非槽函数
// 设置窗口是否可用, 不可用窗口无法接收和处理窗口事件
// 参数true->可用, false->不可用
[slot] void QWidget::setEnabled(bool);
// 设置窗口是否可用, 不可用窗口无法接收和处理窗口事件
// 参数true->不可用, false->可用

[slot] void QWidget::setDisabled(bool disable);
// 设置窗口是否可见, 参数为true->可见, false->不可见
[slot] virtual void QWidget::setVisible(bool visible);
```

### 窗口显示

```c++
// 关闭当前窗口
[slot] bool QWidget::close();
// 隐藏当前窗口
[slot] void QWidget::hide();
// 显示当前创建以及其子窗口
[slot] void QWidget::show();
// 全屏显示当前窗口, 只对windows有效
[slot] void QWidget::showFullScreen();
// 窗口最大化显示, 只对windows有效
[slot] void QWidget::showMaximized();
// 窗口最小化显示, 只对windows有效
[slot] void QWidget::showMinimized();
// 将窗口回复为最大化/最小化之前的状态, 只对windows有效
[slot] void QWidget::showNormal();
```

**注意**:

`QWidgt`可以被它的子类所继承,即在QDialog、控件类、QMainWindow等中都可以使用以上的API

## 使用示例

```c++
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置窗口的最大尺寸
    //setMaximumSize(600,600);
    //设置窗口的最小尺寸
    //setMinimumSize(300,300);
    //设置窗口的固定尺寸
    //setFixedSize(500,500);
    //设置窗口标题
    setWindowTitle("hello,Qt");
    //设置窗口图标
    setWindowIcon(QIcon("C:\\Users\\Lenovo\\Desktop\\图库\\双城之战\\1.jpg"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_moveBtn_clicked()
{
    QRect rect = this->frameGeometry();
    move(rect.topLeft() + QPoint(10,20));

}


void MainWindow::on_PositionBtn_clicked()
{
    QRect rect = this->frameGeometry();
    qDebug()<<"左上角:" <<rect.topLeft()
            <<"右上角:" <<rect.topRight()
            <<"左下角:" <<rect.bottomLeft()
            <<"右下角:" <<rect.bottomRight()
            <<"宽度:" <<rect.width()
            <<"高度:" <<rect.height();

}

void MainWindow::on_geometryBtn_clicked()
{
    int x = 100 + rand() % 500;
    int y = 100 + rand() % 500;
    int width = this->width() + 10;
    int height = this->height() + 10;
    setGeometry(x,y,width,height);
}
```

## 使用示例2

- QMenu：菜单类

```c++
//点击右键
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,&MainWindow::customContextMenuRequested,this,[=](const QPoint &pos){
        QMenu menu;
        menu.addAction("西红柿");
        menu.addAction("黄瓜");
        menu.addAction("土豆");
        menu.exec(QCursor::pos());
    });
```

