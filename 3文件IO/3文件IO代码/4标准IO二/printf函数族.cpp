/*fprintf*/


/*sprintf
// #include <stdio.h>

// int main() {
//     char buffer[100];
//     int year = 2024;
//     int month = 3;
//     int day = 20;
//     char event[] = "编程学习";
    
//     // 将格式化数据写入字符串
//     sprintf(buffer, "日期: %d年%d月%d日, 事件: %s", year, month, day, event);
    
//     printf("生成的字符串: %s\n", buffer);
//     return 0;
// }

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
    
    //将姓名、年龄和专业写入file文件中
    fprintf(file,"姓名：%s 年龄：%d 专业：%s\n",name,age,major);
    
    //姓名、年龄和专业写入字符串buffer中
    sprintf(buffer, "姓名：%s 年龄：%d 专业：%s\n",name,age,major);

    return 0;
}
*/
