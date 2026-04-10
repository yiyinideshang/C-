## makefile

```makefile
CXX = g++                          # 定义编译器为 g++
CXXFLAGS = -std=c++11 -Wall -O2    # 编译选项：C++11标准，显示所有警告，优化级别2
TARGET = ticket_window             # 目标可执行文件名

# 默认目标
all: $(TARGET)                     # 输入 'make' 或 'make all' 会编译程序

# 编译规则
$(TARGET): ticket_window.cpp ticket_common.h
	$(CXX) $(CXXFLAGS) -o $(TARGET) ticket_window.cpp

# 清理目标
clean:
	rm -f $(TARGET) 代售车票.txt 已售车票.txt

# 启动目标
start: $(TARGET)                   # 依赖于 $(TARGET)，确保程序已编译
	chmod +x start_windows.sh      # 给启动脚本执行权限
	./start_windows.sh             # 运行启动脚本

# 声明伪目标
.PHONY: all clean start
```

### 1. 编译程序

```makefile
make              # 编译生成 ticket_window 可执行文件
# 或者
make all

make -B           # 强制重新编译，忽略时间戳
```

### 2. 清理文件

```makefile
make clean        # 删除可执行文件和数据文件
```

### 3. 一键编译并启动

```makefile
make start        # 编译程序（如果需要）并启动5个售票窗口
```

```makefile
# 第一次使用
make clean        # 清理
make start        # 编译并启动系统

# 修改代码后
make clean        # 清理旧版本
make start        # 重新编译并启动

# 或者简单方式
make -B start     # 强制重新编译并启动
```

