#include<iostream>

int main()
{
    FILE *file_ptr = fopen("设置缓冲区策略.cpp","r");
    if(file_ptr == nullptr)
        return -1;
    std::cout<<"打开成功"<<std::endl;
    fclose(file_ptr);
    
    return 0;
}