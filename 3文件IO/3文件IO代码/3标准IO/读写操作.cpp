#include<iostream>

int main()
{
    char buffer[1024] = {0};
    std::cout<<"请输入一行数据"<<std::endl;
    fgets(buffer,1024,stdin);
    std::cout<<buffer<<std::endl;
    
    
    return 0;
}
