#include<iostream>

// int fseek(FILE *stream,int offset,int whence)
// {

//     return lssek(stream->_fileno,offset,whence);
// }

int main()
{
    FILE* file_ptr = fopen("fflush.cpp","r");

    char buffer[1024]={0};
    // 每次读写之前，先确定光标（定位流），【第一次读，不指定光标位置，默认从文件开头开始读取】
    fread(buffer,1,10,file_ptr);//读取：#include<i
    //读取文件中前10个字节的内容
    std::cout << buffer << std::endl;

    //光标定位到：文件开头的位置偏移1个字节
    fseek(file_ptr,1,SEEK_SET);
    //从光标定位的位置开始，【每个元素大小为1，读取十个元素】
    fread(buffer,1,10,file_ptr);//读取：include<io
    std::cout << buffer << std::endl;

    //从光标后来定位的位置开始，【每个元素大小为1，读取十个元素】
    fread(buffer,1,10,file_ptr);        
    std::cout << buffer << std::endl;   
    // 读取：
    //stream>   
    //
    //i
    std::cout<<ftell(file_ptr)<<std::endl;//记录当前光标的偏移量

    fseek(file_ptr,0,SEEK_END);// 将光标移动到文件末尾
    std::cout<<ftell(file_ptr)<<std::endl;// 输出文件总大小
    return 0;
}