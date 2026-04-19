#  2. QDialog

对话框`QDialog`类是`QWidget`类的子类, 处理继承自父类的属性之外, 还有一些自己所特有的属性, 常用的一些API函数如下:

![QDialog](D:\Typora\typora_work\C++研修阶段\2QT图形库\QDialog.png)

```c++
// 构造函数
QDialog::QDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

// 模态显示窗口
[virtual slot] int QDialog::exec();
// 隐藏模态窗口, 并且解除模态窗口的阻塞, 将 exec() 的返回值设置为 QDialog::Accepted
[virtual slot] void QDialog::accept();
// 隐藏模态窗口, 并且解除模态窗口的阻塞, 将 exec() 的返回值设置为 QDialog::Rejected
[virtual slot] void QDialog::reject();
// 关闭对话框并将其结果代码设置为r。finished()信号将发出r;
// 如果r是QDialog::Accepted 或 QDialog::Rejected，则还将分别发出accept()或Rejected()信号。
[virtual slot] void QDialog::done(int r);

[signal] void QDialog::accepted();
[signal] void QDialog::rejected();
[signal] void QDialog::finished(int result);
```

## 槽函数：

### `accpet()`

- **作用**：隐藏对话框，结束 `exec()` 的事件循环，并将`exec()`返回值设为 `QDialog::Accepted`（值为 1）。
- **内部行为**：相当于调用 `done(QDialog::Accepted)`。
- **效果**：发射 `accepted()` 信号，然后发射 `finished(1)` 信号。

### `reject()`

- **作用**：隐藏对话框，结束 `exec()` 的事件循环，并将`exec()`返回值设为 `QDialog::Rejected`（值为 0）。
- **内部行为**：相当于调用 `done(QDialog::Rejected)`。
- **效果**：发射 `rejected()` 信号，然后发射 `finished(0)` 信号。

### `done(int r)`

1. **设置结果码**
   将对话框的内部结果码设置为参数 `r`，然后关闭对话框并结束事件循环。

   这个值将决定 `exec()` 的返回值，并影响后续信号的发射。

2. **发射特定信号（根据结果码）**

   - 如果 `r == QDialog::Accepted`（通常值为1），则立即发射 `accepted()` 信号。
   - 如果 `r == QDialog::Rejected`（通常值为0），则立即发射 `rejected()` 信号。
   - 如果 `r` 是其他整数值，则**不会**发射这两个信号。

3. **发射 `finished(int)` 信号**
   无论 `r` 为何值，都会发射 `finished(int)` 信号，并将 `r` 作为参数传递。**注意**：如果步骤2中发射了 `accepted()` 或 `rejected()`，那么这两个信号会**先于** `finished(int)` 发射（顺序为：先特定信号，后 `finished`）。

4. **结束事件循环（如果通过 `exec()` 显示）**
   如果对话框是通过 `exec()` 模态显示的，`done()` 会终止本地事件循环，并使 `exec()` 函数返回 `r` 值。此时，`exec()` 的调用点将继续执行，并可根据返回值判断用户操作。

5. **处理窗口关闭与销毁**

   - 如果对话框设置了 `Qt::WA_DeleteOnClose` 属性（通过 `setAttribute(Qt::WA_DeleteOnClose)`），则 `done()` 会**删除**对话框对象（调用 `deleteLater` 或直接析构）。
   - 如果该对话框是应用程序的主窗口，则调用 `done()` 会导致整个应用程序退出。
   - 如果这是最后一个关闭的窗口，则会触发 `QGuiApplication::lastWindowClosed()` 信号。

### 同步模态 `exec()`

- ## 作用

  - 将对话框**显示为模态对话框**，阻塞当前代码的执行，直到用户关闭对话框。

  - 启动一个本地事件循环，处理用户交互，直至对话框被隐藏或销毁。

  - 返回一个整数结果码，通常用于判断用户的操作（接受、拒绝或其他自定义值）。

- ## 返回值

  - 当对话框通过 `accept()`、`reject()` 或 `done(int r)` 关闭时，`exec()` 返回设置的结果码 `r`。
    - **`accept()`**：将结果码设置为 `QDialog::Accepted`（值为 1）。
    - **`reject()`**：将结果码设置为 `QDialog::Rejected`（值为 0）。
    - **`done(int r)`**：将结果码直接设置为传入的参数 `r`。

  - 可通过 `exec()` 的返回值在调用处执行不同的逻辑：

### 异步非模态 `open()`

以窗口模态对话框的形式显示该对话框，并立即返回。



## 信号:

### `accepted()`

- **发射时机**：当对话框被“接受”时触发。接受的方式包括：
  - 用户点击了与 `accept()` 槽关联的按钮（如“OK”）。
  - 代码中直接调用 `accept()` 或 `done(QDialog::Accepted)`。
- **注意**：仅当结果码为 `1` 时才会发射，且**先于** `finished(int)` 发射。

### `rejected()`

- **发射时机**：当对话框被“拒绝”时触发。拒绝的方式包括：
  - 用户点击了与 `reject()` 槽关联的按钮（如“Cancel”）。
  - 代码中直接调用 `reject()` 或 `done(QDialog::Rejected)`。
  - 用户点击标题栏关闭按钮（默认行为是调用 `reject()`）。
- **注意**：仅当结果码为 `0` 时才会发射，且**先于** `finished(int)` 发射。

### `finished()`

- **发射时机**：只要对话框的**结果码被设置（即对话框即将关闭并返回结果）时就会发射**。无论结果码是 `Accepted`、`Rejected` 还是自定义值，都会触发。
- **参数**：`result` 即为传递给 `done()` 或由 `accept()`/`reject()` 设置的**结果码**。
- **顺序**：总是在 `accepted()` 或 `rejected()` **之后发射**（如果它们被发射的话）。

## 示例

- 新增一个类名为`mydialog`
  - 选择**项目**文件夹,**单击右键**,选择 **添加新文件...**,选择一个模版:选择 **Qt**,右边选择**Qt Widgets Designer Form class**,选择一个模版形式:选择 **Dialog without Butttons**,选择类名:类名修改为:`Mydialog`

- 在`ui`文件中新增三个`Push Button`按钮, 将**按钮的标题（Title）**或**显示文本（Text）**为:`Accpet`、`Reject`、`Done`,将它们的`objectName`的值(**对象名**)分别修改为:`accpetBtn`、`rejectBtn`、`doneBtn`
  - 在UI设计器中设置后，可以通过 `findChild<QPushButton*>("donBtn")` 查找该按钮，也常用于自动连接信号与槽（例如 `on_donBtn_clicked` 槽函数会自动连接到 `donBtn` 的 `clicked()` 信号
- 之后分别右击这三个按钮,选择 **转到槽**,选择信号 **`clicked()`**
- 分别转到槽后,在`mydialog.cpp`文件中对**槽函数**进行以下处理

```c++
void MyDialog::on_accpetBtn_clicked()
{
    this->accept();//返回值1
}


void MyDialog::on_rejectBtn_clicked()
{
    this->reject(); //返回值0
}


void MyDialog::on_donBtn_clicked()
{
    this->done(10);
    // this->done(1); 参数为1时表示accepted信号
    // this->done(0); 参数为0时表示rejected
}
```

- 之后在`mainwindow.ui`文件中添加一个**新的`Push Button`按钮**,标题为:**显示模态对话框**,并将它的`objectName`的值修改为:**`modalDlg`**,再次右击该按钮,**选择转到槽**,选择**信号`clicked()`**
  - 一旦点击这个按钮,就会生成一个`MyDialog`类的对象`dlg`,它有三个按钮,分别为上述的`Accpet`、`Reject`、`Done`
- 在`mianwindow.cpp`文件中,对**槽函数**进行以下处理

```c++
void MainWindow::on_modalDlg_clicked()
{
    MyDialog dlg;
    connect(&dlg,&MyDialog::finished,this,[=](int res){
        qDebug() <<"result:"<<res;//三种槽函数都会在最后发送finished信号
        //当done槽函数参数为1时，会发射出accept和finished两种信号；
        //当done槽函数参数为0时，会发射出reject和finished两种信号；
    });
    connect(&dlg,&MyDialog::accepted,this,[=](){
        qDebug() <<"accepted 信号被发射出去了...";
    });
    connect(&dlg,&MyDialog::rejected,this,[=](){
        qDebug() <<"rejected 信号被发射出去了...";
    });

    int ret = dlg.exec();
    if(ret == QDialog::Accepted)//值为1,表示对话框被接受
    //例如点击了“OK”按钮或调用了 accept() 槽函数
    {
        qDebug() <<"accpet button clicked...";
    }
    else if(ret == QDialog::Rejected)//值为2,表示对话框被拒绝
    //例如点击了“Cancel”按钮、关闭窗口或调用了 reject() 槽函数
    {
        qDebug() <<"reject button clicked...";
    }
    else
    {
        qDebug() <<"done button clicked...";
    }
}
```

### 运行结果

点击运行

单击 **显示模态对话框**

窗口标题为Dialog,无图标,有三个按钮,分别为:`Accpet`、`Reject`、`Done`

### 单击`Accpet`

当单击`Accpet`按钮后,	会调用`accept()`槽函数,	对应: `void MyDialog::on_accpetBtn_clicked()`

1. 这个槽函数会 发送信号:	发射 `accepted()` 信号，然后发射 `finished(1)` 信号。**这是Qt规定的。**

2. 这个槽函数会 隐藏对话框，结束 `exec()` 的事件循环，并将`exec()`返回值设为 `QDialog::Accepted`（值为 1）。

-  `accepted()` 信号处理、`finished(1)` 信号处理

```c++
    connect(&dlg,&MyDialog::accepted,this,[=](){
        qDebug() <<"accepted 信号被发射出去了...";
    });
```

```c++
connect(&dlg,&MyDialog::finished,this,[=](int res){
        qDebug() <<"result:"<<res;
    });
```

- `exec()`返回值的判定:

```c++
	int ret = dlg.exec();
    if(ret == QDialog::Accepted)//值为1,表示对话框被接受
    //例如点击了“OK”按钮或调用了 accept() 槽函数
    {
        qDebug() <<"accpet button clicked...";
    }
```

- 3 应用程序输出

```
accepted 信号被发射出去了...
result: 1
accpet button clicked...
```

### 单击`Reject`

当单击`Reject`按钮后,	会调用`reject()`槽函数,	对应: `void MyDialog::on_rejectBtn_clicked()`

1. 这个槽函数会 发送信号:	发射 `rejected()` 信号，然后发射 `finished(0)` 信号。**这是Qt规定的。**

2. 这个槽函数会 隐藏对话框，结束 `exec()` 的事件循环，并将`exec()`返回值设为 `QDialog::Rejected`（值为 0）。

-  `rejected()` 信号处理、`finished(0)` 信号处理

```c++
    connect(&dlg,&MyDialog::rejected,this,[=](){
        qDebug() <<"rejected 信号被发射出去了...";
    });
```

```c++
connect(&dlg,&MyDialog::finished,this,[=](int res){
        qDebug() <<"result:"<<res;
    });
```

- `exec()`返回值的判定:

```c++
	int ret = dlg.exec();
    if(ret == QDialog::Accepted)//值为1,表示对话框被接受
    //例如点击了“OK”按钮或调用了 accept() 槽函数
    {
        qDebug() <<"accpet button clicked...";
    }
    else if(ret == QDialog::Rejected)//值为2,表示对话框被拒绝
    //例如点击了“Cancel”按钮、关闭窗口或调用了 reject() 槽函数
    {
        qDebug() <<"reject button clicked...";
    }
```

- 3 应用程序输出

```c++
rejected 信号被发射出去了...
result: 0
reject button clicked...
```

### 单击`Done`

当单击`Done`按钮后,	会调用`done(int r)`槽函数,	对应: `void MyDialog::on_donBtn_clicked()`

#### 当`done`槽函数的参数为1或0 时

这个槽函数会 发送信号:	发射 `accepted()` / `rejected()` 信号，然后发射 `finished(1)` / `finished(0)` 信号。**这是Qt规定的。**

```c++
    //发送1或0,然后发送finished信号,参数为1或0
//1
	connect(&dlg,&MyDialog::accepted,this,[=](){
        qDebug() <<"accepted 信号被发射出去了...";
    });
//0
    connect(&dlg,&MyDialog::rejected,this,[=](){
        qDebug() <<"rejected 信号被发射出去了...";
    });
//
    connect(&dlg,&MyDialog::finished,this,[=](int res){
            qDebug() <<"result:"<<res;
        });
```

- `exec()`返回值的判定:

```c++
	int ret = dlg.exec();
    if(ret == QDialog::Accepted)//值为1,表示对话框被接受
    //例如点击了“OK”按钮或调用了 accept() 槽函数
    {
        qDebug() <<"accpet button clicked...";
    }
    else if(ret == QDialog::Rejected)//值为2,表示对话框被拒绝
    //例如点击了“Cancel”按钮、关闭窗口或调用了 reject() 槽函数
    {
        qDebug() <<"reject button clicked...";
    }
```

- 3 应用程序输出

```c++
//当参数为1时
accepted 信号被发射出去了...
result: 1
accpet button clicked...
//当参数为0时
rejected 信号被发射出去了...
result: 0
reject button clicked...
```

#### 当`done`槽函数的参数不为1或0 时

这个槽函数只会 发送信号:	发射 `finished(r)` 信号。**这是Qt规定的。**

```c++
    connect(&dlg,&MyDialog::finished,this,[=](int res){
            qDebug() <<"result:"<<res;
        });
```

- `exec()`返回值的判定:

```c++
   	else
    {
        qDebug() <<"done button clicked...";
    }
```

- 3 应用程序输出

```c++
result: 10
done button clicked...
```

# 3. QDialog的子类

![QDialog2](D:\Typora\typora_work\C++研修阶段\2QT图形库\QDialog2.png)

## 3.1 QMessageBox

QMessageBox 对话框类是 QDialog 类的子类, 通过这个类可以显示一些简单的提示框, 用于展示警告、错误、问题等信息。关于这个类我们只需要掌握一些静态方法的使用就可以了。

## 3.2 QFileDialog

## 3.3 QFontDialog

关于字体的属性信息, 在QT框架中被封装到了一个叫QFont的类中, 下边为大家介绍一下这个类的API, 了解一下关于这个类的使用。

### 3.3.1 QFont 字体类

```c++
// 构造函数
  QFont::QFont();
  /*
  参数:
    - family: 本地字库中的字体名, 通过 office 等文件软件可以查看
    - pointSize: 字体的字号
    - weight: 字体的粗细, 有效范围为 0 ~ 99
    - italic: 字体是否倾斜显示, 默认不倾斜
  */
  QFont::QFont(const QString &family, int pointSize = -1, int weight = -1, bool italic = false);
  
  // 设置字体
  void QFont::setFamily(const QString &family);
  // 根据字号设置字体大小
  void QFont::setPointSize(int pointSize);
  // 根据像素设置字体大小
  void QFont::setPixelSize(int pixelSize);
  // 设置字体的粗细程度, 有效范围: 0 ~ 99
  void QFont::setWeight(int weight);
  // 设置字体是否加粗显示
  void QFont::setBold(bool enable);
  // 设置字体是否要倾斜显示
  void QFont::setItalic(bool enable);
  
  // 获取字体相关属性(一般规律: 去掉设置函数的 set 就是获取相关属性对应的函数名)
  QString QFont::family() const;
  bool QFont::italic() const;
  int QFont::pixelSize() const;
  int QFont::pointSize() const;
  bool QFont::bold() const;
  int QFont::weight() const;
```

- 窗口字体的设置

如果一个QFont对象被创建, 并且进行了初始化, 我们可以将这个属性设置给某个窗口, 或者设置给当前应用程序对象。

```c++
// QWidget 类
// 得到当前窗口使用的字体
const QWidget::QFont& font() const;
// 给当前窗口设置字体, 只对当前窗口类生效
void QWidget::setFont(const QFont &);

// QApplication 类
// 得到当前应用程序对象使用的字体
[static] QFont QApplication::font();
// 给当前应用程序对象设置字体, 作用于当前应用程序的所有窗口
[static] void QApplication::setFont(const QFont &font, const char *className = nullptr);
```

### 3.3.2 QFontDialog类的静态API

- `QFont`类,见上文

```c++
/*
参数:
  - ok: 传出参数, 用于判断是否获得了有效字体信息, 指定一个布尔类型变量地址
  - initial: 字体对话框中默认选中并显示该	字体信息, 用于对话框的初始化
  - parent: 字体对话框窗口的	父对象
  - title: 字体对话框的	窗口标题
  - options: 字体对话框选项, 使用默认属性即可, 一般不设置
*/
[static] QFont QFontDialog::getFont(
		bool *ok, const QFont &initial, 
		QWidget *parent = nullptr, const QString &title = QString(), 
		QFontDialog::FontDialogOptions options = FontDialogOptions());
  
[static] QFont QFontDialog::getFont(bool *ok, QWidget *parent = nullptr);
```

## 3.4 QColorDialog

QColorDialog类是QDialog的子类, 通过这个类我们可以得到一个选择颜色的对话框窗口, 和前边介绍的对话框类一样, 我们只需要调用这个类的静态成员函数就可以得到想要的窗口了。

### 3.4.1 QColor 颜色类

关于颜色的属性信息, 在QT框架中被封装到了一个叫QColor的类中, 下边为大家介绍一下这个类的API, 了解一下关于这个类的使用。
各种颜色都是基于红, 绿, 蓝这三种颜色调配而成的, 并且颜色还可以进行透明度设置, 默认是不透明的。

```c++
// 构造函数
QColor::QColor(Qt::GlobalColor color);//GlobalColor是一个枚举值
QColor::QColor(int r, int g, int b, int a = ...);
QColor::QColor();

// 参数设置 red, green, blue, alpha, 取值范围都是 0-255	
void QColor::setRed(int red);		// 红色
void QColor::setGreen(int green);	// 绿色
void QColor::setBlue(int blue);	// 蓝色
void QColor::setAlpha(int alpha);	// 透明度, 默认	完全不透明(255)
void QColor::setRgb(int r, int g, int b, int a = 255);

int QColor::red() const;
int QColor::green() const;
int QColor::blue() const;
int QColor::alpha() const;
void QColor::getRgb(int *r, int *g, int *b, int *a = nullptr) const;//传出参数
```

### 3.4.2 QColorDialog类的静态API函数

- QColor类,见上文

```c++
  // 弹出颜色选择对话框, 并返回选中的颜色信息
/*
参数:
    - initial: 对话框中默认选中的颜色, 用于窗口初始化
    - parent: 给对话框窗口指定父对象
    - title: 对话框窗口的标题
    - options: 颜色对话框窗口选项, 使用默认属性即可, 一般不需要设置
*/
  [static] QColor QColorDialog::getColor(
		const QColor &initial = Qt::white, 
		QWidget *parent = nullptr, const QString &title = QString(), 
		QColorDialog::ColorDialogOptions options = ColorDialogOptions());
```

## 3.5 QInputDialog

QInputDialog类是QDialog的子类, 通过这个类我们可以得到一个输入对话框窗口, 根据实际需求我们可以在这个输入窗口中输入整形, 浮点型, 字符串类型的数据, 并且还可以显示下拉菜单供使用者选择。

和前边介绍的对话框类一样, 我们只需要调用这个类的静态成员函数就可以得到想要的窗口了。

### 3.5.1QInputDialog类的静态函数

- **API - 静态函数**

- ## 1. getDouble

```c++
// 得到一个可以输入浮点数的对话框窗口, 返回对话框窗口中输入的浮点数
/*
参数:
  - parent: 对话框窗口的父窗口
  - title: 对话框窗口显示的标题信息
  - label: 对话框窗口中显示的文本信息(用于描述对话框的功能)
  - value: 对话框窗口中显示的浮点值, 默认为 0
  - min: 对话框窗口支持显示的最小数值
  - max: 对话框窗口支持显示的最大数值
  - decimals: 浮点数的精度, 默认保留小数点以后1位
  - ok: 传出参数, 用于判断是否得到了有效数据, 一般不会使用该参数
  - flags: 对话框窗口的窗口属性, 使用默认值即可
  - step: 步长,即当用户通过对话框中的上下箭头按钮（或键盘上下键）调整数值时，每次增加或减少的量。
*/
[static] double QInputDialog::getDouble(
    		QWidget *parent, const QString &title, 
    		const QString &label, double value = 0, 
    		double min = -2147483647, double max = 2147483647, 
    		int decimals = 1, bool *ok = nullptr, 
    		Qt::WindowFlags flags = Qt::WindowFlags(), double step = 1);
```

`getDobule`的最大值参数`max`后面一个为精度参数`decimals`,用来指定保留小数点后几位,最后一个参数为步长`step`

`geeInt`的最大值参数`max`后面一个为步长参数`step`

- ## 2. getInt

```c++
// 得到一个可以输入整形数的对话框窗口, 返回对话框窗口中输入的整形数
/*
参数:
  - parent: 对话框窗口的父窗口
  - title: 对话框窗口显示的标题信息
  - label: 对话框窗口中显示的文本信息(用于描述对话框的功能)
  - value: 对话框窗口中显示的整形值, 默认为 0
  - min: 对话框窗口支持显示的最小数值
  - max: 对话框窗口支持显示的最大数值
  - step: 步长, 通过对话框提供的按钮调节数值每次增长/递减的量
  - ok: 传出参数, 用于判断是否得到了有效数据, 一般不会使用该参数
  - flags: 对话框窗口的窗口属性, 使用默认值即可
*/
[static] int QInputDialog::getInt(
    		QWidget *parent, const QString &title, 
    		const QString &label, int value = 0, 
    		int min = -2147483647, int max = 2147483647, 
    		int step = 1, bool *ok = nullptr, 
    		Qt::WindowFlags flags = Qt::WindowFlags());
```

- ## 3. getItem

```c++
// 得到一个带下来菜单的对话框窗口, 返回选择的菜单项上边的文本信息
/*
参数:
  - parent: 对话框窗口的父窗口
  - title: 对话框窗口显示的标题信息
  - label: 对话框窗口中显示的文本信息(用于描述对话框的功能)
  - items: 字符串列表, 用于初始化窗口中的下拉菜单, 每个字符串对应一个菜单项
  - current: 通过菜单项的索引指定显示下拉菜单中的哪个菜单项, 默认显示第一个(编号为0)
  - editable: 设置菜单项上的文本信息是否可以进行编辑, 默认为true, 即可以编辑
  - ok: 传出参数, 用于判断是否得到了有效数据, 一般不会使用该参数
  - flags: 对话框窗口的窗口属性, 使用默认值即可
  - inputMethodHints: 设置显示模式, 默认没有指定任何特殊显示格式, 显示普通文本字符串
    - 如果有特殊需求, 可以参数帮助文档进行相关设置
*/
[static] QString QInputDialog::getItem(
    		QWidget *parent, const QString &title, 
    		const QString &label, const QStringList &items, 
    		int current = 0, bool editable = true, bool *ok = nullptr, 
    		Qt::WindowFlags flags = Qt::WindowFlags(), 
    		Qt::InputMethodHints inputMethodHints = Qt::ImhNone);
```

- ## 4. getMultiLineText	/	5. getText

```c++
// 得到一个可以输入多行数据的对话框窗口, 返回用户在窗口中输入的文本信息
/*
参数:
  - parent: 对话框窗口的父窗口
  - title: 对话框窗口显示的标题信息
  - label: 对话框窗口中显示的文本信息(用于描述对话框的功能)
  - text: 指定显示到多行输入框中的文本信息, 默认是空字符串
  - ok: 传出参数, 用于判断是否得到了有效数据, 一般不会使用该参数
  - flags: 对话框窗口的窗口属性, 使用默认值即可
  - inputMethodHints: 设置显示模式, 默认没有指定任何特殊显示格式, 显示普通文本字符串
    - 如果有特殊需求, 可以参数帮助文档进行相关设置
*/
[static] QString QInputDialog::getMultiLineText(
    		QWidget *parent, const QString &title, const QString &label, 
    		const QString &text = QString(), bool *ok = nullptr, 
    		Qt::WindowFlags flags = Qt::WindowFlags(), 
    		Qt::InputMethodHints inputMethodHints = Qt::ImhNone);


// 得到一个可以输入单行信息的对话框窗口, 返回用户在窗口中输入的文本信息
/*
参数:
  - parent: 对话框窗口的父窗口 
  - title: 对话框窗口显示的标题信息
  - label: 对话框窗口中显示的文本信息(用于描述对话框的功能)
  - mode: 指定单行编辑框中数据的反馈模式, 是一个 QLineEdit::EchoMode 类型的枚举值
    - QLineEdit::Normal: 显示输入的字符。这是默认值
    - QLineEdit::NoEcho: 不要展示任何东西。这可能适用于连密码长度都应该保密的密码。
    - QLineEdit::Password: 显示与平台相关的密码掩码字符，而不是实际输入的字符。
    - QLineEdit::PasswordEchoOnEdit: 在编辑时按输入显示字符，否则按密码显示字符。
  - text: 指定显示到单行输入框中的文本信息, 默认是空字符串
  - ok: 传出参数, 用于判断是否得到了有效数据, 一般不会使用该参数
  - flags: 对话框窗口的窗口属性, 使用默认值即可
  - inputMethodHints: 设置显示模式, 默认没有指定任何特殊显示格式, 显示普通文本字符串
     - 如果有特殊需求, 可以参数帮助文档进行相关设置
*/
[static] QString QInputDialog::getText(
    		QWidget *parent, const QString &title, const QString &label,
    		QLineEdit::EchoMode mode = QLineEdit::Normal, 
    		const QString &text = QString(), bool *ok = nullptr, 
    		Qt::WindowFlags flags = Qt::WindowFlags(), 
    		Qt::InputMethodHints inputMethodHints = Qt::ImhNone);
```

### 使用示例

## 3.6 QProgressDialog

QProgressDialog类是QDialog的子类, 通过这个类我们可以得到一个带进度条的对话框窗口, 这种类型的对话框窗口一般常用于文件拷贝、数据传输等实时交互的场景中。

### 3.6.1 QProgressDialog常用API

```c++
// 构造函数
/*
参数:
  - labelText: 对话框中显示的提示信息
  - cancelButtonText: 取消按钮上显示的文本信息
  - minimum: 进度条最小值 0%
  - maximum: 进度条最大值 100%
  - parent: 当前窗口的父对象
  - f: 当前进度窗口的flag属性, 使用默认属性即可, 无需设置
*/
QProgressDialog::QProgressDialog(
	QWidget *parent = nullptr, 
	Qt::WindowFlags f = Qt::WindowFlags());

QProgressDialog::QProgressDialog(
	const QString &labelText, const QString &cancelButtonText, 
	int minimum, int maximum, QWidget *parent = nullptr,
	Qt::WindowFlags f = Qt::WindowFlags());


// 设置取消按钮显示的文本信息
[slot] void QProgressDialog::setCancelButtonText(const QString &cancelButtonText);

// 公共成员函数和槽函数
QString QProgressDialog::labelText() const;
void QProgressDialog::setLabelText(const QString &text);

// 得到进度条最小值
int QProgressDialog::minimum() const;
// 设置进度条最小值
void QProgressDialog::setMinimum(int minimum);
// 得到进度条最大值
int QProgressDialog::maximum() const;
// 设置进度条最大值
void QProgressDialog::setMaximum(int maximum);

// 设置进度条范围(最大和最小值)
[slot] void QProgressDialog::setRange(int minimum, int maximum);

// 得到进度条当前的值
int QProgressDialog::value() const;
// 设置进度条当前的值
void QProgressDialog::setValue(int progress);

//查看是否重置,true为重置
bool QProgressDialog::autoReset() const;
// 当value() = maximum()时，进程对话框是否调用reset()，此属性默认为true。(重置)
void QProgressDialog::setAutoReset(bool reset);

//查看进度条走完后,是否关闭,true为关闭
bool QProgressDialog::autoClose() const;
// 当value() = maximum()时，进程对话框是否调用reset()并且隐藏，此属性默认为true。
void QProgressDialog::setAutoClose(bool close);

// 判断用户是否按下了取消键, 按下了返回true, 否则返回false
bool wasCanceled() const;


// 重置进度条
// 重置进度对话框。wascancelled()变为true，直到进程对话框被重置。进度对话框被隐藏。
[slot] void QProgressDialog::cancel();
// 重置进度对话框。如果autoClose()为真，进程对话框将隐藏。
[slot] void QProgressDialog::reset();   

// 信号
// 当单击cancel按钮时，将发出此信号。默认情况下，它连接到cancel()槽。
[signal] void QProgressDialog::canceled();

// 设置窗口的显示状态(模态, 非模态)
/*
参数:
	Qt::NonModal  -> 非模态
	Qt::WindowModal	-> 模态, 阻塞父窗口
	Qt::ApplicationModal -> 模态, 阻塞应用程序中的所有窗口
*/
void QWidget::setWindowModality(Qt::WindowModality windowModality);
```

