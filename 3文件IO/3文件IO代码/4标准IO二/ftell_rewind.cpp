#include<iostream>

int main()
{
    FILE * file_ptr = fopen("fflush.cpp","r");

    fseek(file_ptr,0,SEEK_END); // 将光标移动到文件末尾
    std::cout << ftell(file_ptr) << std::endl;// 输出文件总大小

    rewind(file_ptr);// 将文件指针重置到文件开头
    std::cout << ftell(file_ptr) << std::endl;// 输出 0

    fclose(file_ptr);
    return 0;
}