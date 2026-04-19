# C/C++对于`MySQL`的支持

`API` 下载网址：https://dev.mysql.com/downloads/ 

- 选择“ Include MySQL C Connector ”选项，下载 MySQL的C 连接器。

**编译程序连接库：**

- `g++ test.cpp -I mysql的头文件目录 -L mysql的库目录 -lmysql的库名`

把压缩包中的 `include` 和 `lib` 目录复制到工程目录下，

再将`lib` 目录下的`libmysql.dl`复制到程序的相同目录下，

最后编译

`g++ .\MySQL_Demo.cpp -I .\include\ -L .\lib\ -lmysql` 

执行

`.\a.exe`  

**libmysql.dll**是MySQL数据库的动态链接库(`Dynamic Link Library`)文件，位于Windows系统目录中，通常在操作系统安装时自动创建，用于支持`PHP`与`MySQL`数据库之间的通信

````tex
PS D:\VSCodeProject\MysqlAPI\Project> g++ .\MySQL_Demo.cpp -I .\include\ -L .\lib\ -lmysql
PS D:\VSCodeProject\MysqlAPI\Project> .\a.exe
Mysql connect success!
information_schema
mysql
performance_schema
sys
vip_1
````

# MySQL的接口流程

`mysql C库`

- 初始化 `MySQL` 库：
  -  初始化 `MySQL` 库会完成一些必要的**内部设置**和**资源分配**，为后续与数据库的交互操作做好准 备。同时可以确保库处于一个已知的、正确的初始状态，避免一些潜在的未定义行为和错误。 
- 连接 `Mysql` 数据库 
- 执行 `SQL` 语句 
- 获取结果 
- 处理结果 
- 释放资源 
  - 释放结果的资源 
- 关闭数据库连接 
- 解除 `mysql` 库初始化

# API 基本接口概述

## 初始化 `MySQL` 库`mysql_library_init`

通过调用 `mysql_library_init` 初始化 `MySQL` 库。

```c++
int mysql_library_init(int argc,char **argv,char **groups)
/*
    @描述:
    	用于初始化MySQL客户端库。
    @argc:
    	命令行参数个数
    @argv:
    	命令行参数数组
    @groups:
    	选项组，用于控制MySQL客户端库的行为
    @返回值：
    	如果初始化成功就返回0，如果失败就返回非0
    
    注意：在非多线程环境中，mysql_init() 根据需要自动调用 mysql_library_init()。如果是在多线程环境中是要手动去调用这个函数的，比如建立聊天室等，就必须要调用到这个函数，建议写上去比较好。在mysql 8.0版本，argc、argv 和 groups 参数未使用，所以写成
    mysql_library_init(0,NULL,NULL)
*/
```

通过调用 `mysql_init` 初始化连接处理程序

```c++
MYSQL *mysql_init(MYSQL *mysql)
/*
    @描述：
    	用于初始化一个MYSQL结构体，返回一个初始化的MYSQL*句柄(类似于Linux的文件描述符	)
    @mysql：
        mysql 一个指向 MYSQL 连接句柄结构的指针。
        如果mysql是NULL，mysql_init() 将分配空间并初始化并返回新的连接句柄。如果mysql不为空，mysql_init对mysql进行初始化，并且同样的指针会被返回。
    @返回值：
    	一个被始化的MYSQL*句柄，在内存不足的情况下，返回NULL
    
    注意：如果 mysql_init() 分配了一个新对象，则在调用 mysql_close() 关闭连接时将其释放。
*/
```

## 连接到服务器  `mysql_real_connect`

通过调用 `mysql_real_connect` 连接到服务器

```c++
MYSQL *mysql_real_connect(
    MYSQL *mysql, 
    const char *host,
    const char *user,
    const char *passwd,
    const char *db,
    unsigned int port,
    const char *unix_socket,
    unsigned long clientflag);
/*
    @mysql：
    	一个预先分配和初始化的MYSQL对象指针。该指针用于保存与MySQL服务器的连接相关信
息。
    @host：
    	MySQL服务器的主机名或IP地址。
    @user：
    	连接MySQL服务器所使用的用户名。
    @passwd：
    	连接MySQL服务器所使用的密码。
    @db：
    	连接成功后要使用的默认数据库名。
    @port：
    	MySQL服务器的端口号。默认是3306
    @unix_socket：
    	Unix域套接字路径，用于本地连接Unix系统上的MySQL服务器，一般情况都属设置
为nullptr。
    @client_flag：
    	客户端标志位，用于指定连接选项。
    @return：
        成功，返回一个新的MYSQL对象指针，该对象与第一个参数的值相同，用于后续的MySQL操作。
        失败，返回NULL，并通过调用mysql_errno()和mysql_error()函数获取错误代码和错误信息。
*/
```

## 发出 `SQL` 语句  `mysql_query`

````c++
int mysql_query(MYSQL *mysql, const char *query)
/*
    @mysql:数据库指针
    @query:数据库操作指令字符串指针
    @return:成功，返回0；失败，返回非零值，可以通过调用mysql_error()函数获取错误信息。
*/
````

## 获取结果  `mysql_store_result`

通过 `mysql_store_result` 获取结果

````c++
MYSQL_RES *mysql_store_result(MYSQL *mysql)
/*
    @描述：
    	用来获取查询结果的
    @mysql：
    	执行了查询结果的数据库句柄
    @return:
    	返回结果集
*/
````

## 处理获取到的结果 `mysql_fetch_row`

通过 `mysql_fetch_row` 处理获取到的结果

```c++
MYSQL_ROW mysql_fetch_row(MYSQL_RES *result);
/*
    @描述：
    	拆分获取的结果
    @result：
    	通过mysql_store_result获取到的结果集合
    @return：
    	一行一行的数据(一条数据)
*/
```

## 释放资源 `mysql_free_result`

通过 `mysql_free_result` 释放结果的资源

```c++
mysql_free_result(result); // 释放存放结果的空间
```

## 关闭`MySQL` 连接  `mysql_close`

通过调用 `mysql_close` 关闭与 MySQL 服务器的连接。

````c++
void mysql_close(MYSQL *connection);
/*
    @connection：指向一个已经打开的MYSQL对象的指针。该对象代表与MySQL服务器的连接。
    
    注意：
    在调用mysql_close之前，确保已经完成了与数据库的所有操作，包括查询和事务等。
    关闭连接后，将无法再使用该连接对象执行任何操作，除非重新使用mysql_real_connect建立新的连接。
    对于多个线程同时共享一个连接对象的情况，应该确保在多个线程之间正确地协调和同步连接的打
开和关闭操作，以避免出现竞态条件和意外错误。
*/
````

## 解除初始化 `mysql_library_end`

```c++
mysql_library_end(); // 终止库
```

## 错误信息  `mysql_error`

````c++
const char *mysql_error();
/*
    @描述：
    	发送错误了之后，可以调用该函数，然后返回错误信息
    @return：
    	错误信息的字符串形式
*/
````

# API 基本数据结构参考	

| 名字        | 描述                                                         |
| ----------- | ------------------------------------------------------------ |
| `MYSQL`     | 这个结构代表一个数据连接的句柄，它几乎用于所有 `MySQL` 函数。 |
| `MYSQL_RES` | 这个结构体代表一个询问语句的返回值,（ `SELECT`，`SHOW`, `DESCRIBE`, `EXPLAIN` ）,查询语句返回的信息叫做结果集 |
| `MYSQL_ROW` | 这是一行数据的”类型安全“表示。，可以把它看成一个字符串集（但是如果他包含 二进制数据的话，就不能这样做，因为很多数据内部都包含Null字节），行是通 过 `mysql_fetch_row` 获得的。 |

# API 基本函数参考

## 表1

| 名字                           | 描述                                                         | 是 否 弃 用 |
| ------------------------------ | ------------------------------------------------------------ | ----------- |
| `mysql_affected_rows()`        | 上次 `更新 、删除或插入`语句更改/ `删除 / 插 入` 的行数      |             |
| `mysql_autocommit()`           | 设置自动提交模式                                             |             |
| `mysql_bind_param()`           | 为执行的下一条语句定义查询属性                               |             |
| `mysql_change_user()`          | 在打开的连接上更改用户和数据库                               |             |
| `mysql_character_set_name()`   | 当前连接的默认字符集名称                                     |             |
| `mysql_close()`                | 关闭与服务器的连接                                           |             |
| `mysql_commit()`               | 提交事务                                                     |             |
| `mysql_connect()`              | 连接到 MySQL 服务器，现已改用 `mysql_real_connect()`         | 是          |
| `mysql_create_db()`            | 创建数据库，改用 `mysql_real_query` 或 `mysql_query` 发出 `SQL` `CREATE` `DATABASE` 语句 | 是          |
| mysql_data_seek()              | 查找查询结果集中的任意行号                                   |             |
| mysql_debug()                  | 使用给定字符串执行 `DBUG_PUSH`                               |             |
| mysql_drop_db()                | 删除数据库，改用 `mysql_real_query`或 `mysql_query` 发出 `SQL` `DROP` `DATABASE` 语 句 | 是          |
| mysql_dump_debug_info()        | 导致服务器将调试信息写入错误日志                             |             |
| mysql_eof()                    | 确定是否已读取结果集的最后一行                               | 是          |
| mysql_errno()                  | 最近调用的 MySQL 函数的错误号                                |             |
| mysql_error()                  | 最近调用的 MySQL 函数的错误消息                              |             |
| mysql_escape_string()          | 转义字符串中用于 SQL 语句的特殊字符                          |             |
| mysql_fetch_field()            | 下一个表字段的类型                                           |             |
| mysql_fetch_field_direct()     | 给定字段编号的表字段类型                                     |             |
| mysql_fetch_fields()           | 返回所有字段结构的数组                                       |             |
| mysql_fetch_lengths()          | 返回当前行中所有列的长度                                     |             |
| mysql_fetch_row()              | 提取下一个结果集行                                           |             |
| mysql_field_count()            | 最新语句的结果列数                                           |             |
| mysql_field_seek()             | 查找结果集行中的列                                           |             |
| mysql_field_tell()             | 上次 `mysql_fetch_field` 调用的字段位置                      |             |
| mysql_free_result()            | 释放结果集内存                                               |             |
| mysql_free_ssl_session_data()  | 释放上次 mysql_get_ssl_session_data 调用的会话数据句柄       |             |
| mysql_get_character_set_info() | 有关默认字符集的信息                                         |             |
| mysql_get_client_info()        | 客户端版本（字符串）                                         |             |
| mysql_get_client_version()     | 客户端版本（整数）                                           |             |
| mysql_get_host_info()          | 有关连接的信息                                               |             |
| mysql_get_option()             | `mysql_options` 选项的值                                     |             |
| mysql_get_proto_info()         | 连接使用的协议版本                                           |             |
| mysql_get_server_info()        | 服务器版本号（字符串）                                       |             |
| mysql_get_server_version()     | 服务器版本号（整数）                                         |             |
| mysql_get_ssl_cipher()         | 当前 SSL 密码                                                |             |
| mysql_get_ssl_session_data()   | 返回启用 SSL 的连接的会话数据                                |             |
| mysql_get_ssl_session_reused() | 会话是否重复使用                                             |             |
| mysql_hex_string()             | 以十六进制格式对字符串进行编码                               |             |
| mysql_info()                   | 有关最近执行的语句的信息                                     |             |
| mysql_init()                   | 获取或初始化结构 MYSQL                                       |             |
| mysql_insert_id()              | 为列生成的 ID 以前的声明 `AUTO_INCREMENT`                    |             |
| mysql_kill()                   | 终止线程                                                     | 是          |
| ==mysql_library_end()==        | 完成 MySQL C API 库                                          |             |
| mysql_library_init()           | 初始化 MySQL C API 库                                        |             |
| mysql_list_dbs()               | 返回与正则表达式匹配的数据库名称                             |             |
| mysql_list_fields()            | 返回与正则表达式匹配的字段名称                               |             |
| mysql_list_processes()         | 当前服务器线程列表                                           |             |
| mysql_list_tables()            | 返回与正则表达式匹配的表名                                   |             |
| mysql_more_results()           | 检查是否存在更多结果                                         |             |
| mysql_next_result()            | 在多结果执行中返回/启动下一个结果                            |             |

## 表2

| 名称                             | 描述                                             | 是否弃用 |
| -------------------------------- | ------------------------------------------------ | -------- |
| mysql_num_fields()               | 结果集中的列数                                   |          |
| mysql_num_rows()                 | 结果集中的行数                                   |          |
| mysql_options()                  | 连接前设置选项                                   |          |
| mysql_options4()                 | 连接前设置选项                                   |          |
| mysql_ping()                     | Ping 服务器                                      |          |
| ==mysql_query()==                | 执行语句                                         |          |
| ==mysql_real_connect()==         | 连接到 MySQL 服务器                              |          |
| mysql_real_connect_dns_srv()     | 使用 `DNS` `SRV` 记录连接到 MySQL 服务器         |          |
| mysql_real_escape_string()       | 对语句字符串中的特殊字符进行编码                 |          |
| mysql_real_escape_string_quote() | 对语句字符串中的特殊字符进行编码，以 引 用上下文 |          |
| mysql_real_query()               | 执行语句                                         |          |
| mysql_refresh()                  | 刷新或重置表和缓存                               |          |
| mysql_reload()                   | 重新加载授权表                                   | 是       |
| mysql_reset_connection()         | 重置连接以清除会话状态                           |          |
| mysql_reset_server_public_key()  | 从客户端库中清除缓存的 RSA 公钥                  |          |
| mysql_result_metadata()          | 结果集是否具有元数据                             |          |
| mysql_rollback()                 | 回滚事务                                         |          |
| mysql_row_seek()                 | 查找结果集中的行偏移量                           |          |
| mysql_row_tell()                 | 结果集行中的当前位置                             |          |
| mysql_select_db()                | 选择数据库                                       |          |
| mysql_server_end()               | 完成 MySQL C API 库                              |          |
| mysql_server_init()              | 初始化 MySQL C API 库                            |          |
| mysql_session_track_get_first()  | 会话状态更改信息的第一部分                       |          |
| mysql_session_track_get_next()   | 会话状态更改信息的下一部分                       |          |
| mysql_set_character_set()        | 设置当前连接默认字符集                           |          |
| mysql_set_local_infile_default() | 处 理程序回调到默认值                            |          |

## 表3

| 名称                             | 描述                                | 是否弃用 |
| -------------------------------- | ----------------------------------- | -------- |
| mysql_set_local_infile_handler() | 处 理程序回调                       |          |
| mysql_set_server_option()        | 设置当前连接的选项                  |          |
| mysql_shutdown()                 | 关闭 MySQL 服务器                   |          |
| mysql_sqlstate()                 | 最近调用的 MySQL 函数的 SQLSTATE 值 |          |
| mysql_ssl_set()                  | 准备与服务器建立 SSL 连接           |          |
| mysql_stat()                     | 服务器状态                          |          |
| mysql_store_result()             | 检索和存储整个结果集                |          |
| mysql_thread_id()                | 当前线程 ID                         |          |
| mysql_use_result()               | 启动逐行结果集检索                  |          |
| mysql_warning_count()            | 上一个语句的警告计数                |          |

## 示例代码

```c++
#include <iostream>
#include <mysql.h>

int main()
{
    //初始化数据库链接库
    mysql_library_init(0,NULL,NULL);

    //初始化数据库
    MYSQL * mysql = mysql_init(NULL);

    //连接数据库
    if(mysql_real_connect(mysql,"localhost","root","123456",NULL,0,NULL,0)==NULL)
        std::cout<<"Mysql connect failed:"<<mysql_error(mysql)<<std::endl;
    else
        std::cout<<"Mysql connect success!"<<std::endl;

    //操作数据库，执行数据库语句
    std::string sql = "show databases";
    if(mysql_real_query(mysql,sql.c_str(),sql.length()) !=0)
        std::cout<<"Sql query failed:"<<mysql_error(mysql)<<std::endl;
    else
    {
        //获取结果集
        MYSQL_RES *result = mysql_store_result(mysql);

        //处理结果集
        MYSQL_ROW row = NULL;
        while(1)
        {   
            row = mysql_fetch_row(result);
            if(row == NULL)break;

            //打印结果集
            std::cout<<row[0]<<std::endl;
        }
        //释放结果集
        mysql_free_result(result);
    }

    //关闭数据库
    mysql_close(mysql);

    //释放数据库连接库
    mysql_library_end();
    return 0;
}
```

# 作业： 

练习一下 Mysql C 的 API 使用。