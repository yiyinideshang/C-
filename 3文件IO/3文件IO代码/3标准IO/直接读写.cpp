// #include<iostream>
// #include <sys/stat.h>
// #include <filesystem>

// // size_t fread(void *ptr, size_t size, size_t count, FILE *stream);
//     // ptr：目标缓冲区
//     // size：每个元素的大小（字节）
//     // count：要读取的元素数量，个数，次数
//     // stream：文件指针
//     // 返回值：成功读取的元素数量
// int main()
// {
//     char buffer[1024] = {0};
//     //使用标准IO以读的方式打开文件设置缓冲区策略.cpp
//     FILE *file_ptr = fopen("设置缓冲区策略.cpp","r");  //这是标准IO函数，带缓冲，跨平台，高级接口

//     // 每个元素1字节，读取1024个元素，返回值为元素数量
//     // 【每个元素1个字节，一共324个字节，也就是用324个元素，返回值为：324】
//     std::cout << fread(buffer,1,1024,file_ptr) << std::endl;

//     //每个元素1024字节，读取1个元素，返回值为元素数量，如果实际文件内容不足1024个字节，返回值为0
//     // 【每个元素1024个字节，一共324个字节，不足1个元素，返回值为：0】
//     // std::cout << fread(buffer,1024,1,file_ptr) << std::endl;

//     fclose(file_ptr);
//     std::cout << buffer << std::endl;//将文件的内容输入到缓冲区内，打印结果为文件内容

//     //这是基于Linux系统的IO函数，无缓冲，性能好.不可跨平台
//     struct stat stat_buf;
//     if (stat("设置缓冲区策略.cpp", &stat_buf) == 0) {//获取一个指定文件的属性信息,成功返回0, 失败返回-1，同时errno被设置
//         std::cout << "文件大小: " << stat_buf.st_size << " 字节" << std::endl;
//     }

//     //这是C++标准库函数，跨平台，面向对象，可能带缓冲
//     long file_size = std::filesystem::file_size("设置缓冲区策略.cpp");
//     std::cout << "文件大小: " << file_size << " 字节" << std::endl;

//     return 0;
// }

// #include<iostream>

// int main()
// {   
//     FILE *file_ptr =  fopen("设置缓冲区策略.cpp","r");

//     char buffer[1024] = {0};
//     std::cout<< "元素数量：" << fread(buffer,1,1024,file_ptr) << std::endl;
//     fclose(file_ptr);
//     std::cout<< buffer <<std::endl;

//     return 0;
// }

// #include <cstdio>
#include <iostream>
int main() {
    FILE* file = fopen("test.txt", "w");  
    if (file) {
        fwrite("Hello1", 1, 6, file);
        fclose(file);
        std::cout << "文件写入完成" << std::endl;
    }
    return 0;
}