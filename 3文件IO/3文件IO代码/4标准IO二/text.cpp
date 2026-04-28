#include<iostream>
int main()
{
    char name[1024] = {0};
    int age = 0;
    char major[1024] = {0};
    char buffer[1024] = {0};

    FILE * file = fopen("1.txt","a+");

    std::cout<<"请输入姓名、年龄和专业"<<std::endl;
    std::cin >> name >> age >> major;
    
    fprintf(file,"姓名：%s 年龄：%d 专业：%s\n",name,age,major);

    sprintf(buffer, "姓名：%s 年龄：%d 专业：%s\n",name,age,major);

    return 0;
}