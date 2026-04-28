/*fscanf
// #include<iostream>

// int main()
// {
//     // 姓名：张三，年龄：18,专业：上海major
//     FILE * file_ptr = fopen("1.txt","r");

//     char name[1024] = {0};
//     char major[1024] = {0};
//     int age = 0;
//     fscanf(file_ptr,"姓名：%s 年龄：%d 专业：%s",name,&age,major);
//     std::cout<< "姓名："<<name<<" 年龄："<<age<<" 专业："<<major<<std::endl;
//     fclose(file_ptr);

//     return 0;
// }
*/

/*sscanf

// #include <iostream>
// #include <cstdio>

// int main() {
//     const char *str = "123456asdsa4564asd";
//     int r = 0, a = 0;
//     char text1[100] = {0}, text2[100] = {0}; // 用来存储字符串
    
//     // 修改格式字符串，使用扫描集 %[a-zA-Z] 来匹配字母
//     int result = sscanf(str, "%d%[a-zA-Z]%d%[a-zA-Z]", &r, text1, &a, text2);
    
//     std::cout << "成功匹配了 " << result << " 个项目" << std::endl;
//     std::cout << "第一个数字: " << r << std::endl;
//     std::cout << "中间文本: " << text1 << std::endl;
//     std::cout << "第二个数字: " << a << std::endl;
//     std::cout << "最后文本: " << text2 << std::endl;
    
//     // 组合输出完整内容
//     std::cout << "完整内容: " << r << text1 << a << text2 << std::endl;
    
//     return 0;
// }
*/

#include<iostream>
int main()
{
    FILE * file = fopen("1.txt","r");
    char name[1024] = {0};
    char major[1024] = {0};
    int age = 0;
    char buffer[1024] = {0};
    while(fgets(buffer,1024,file))
    {
        sscanf(buffer,"姓名：%s 年龄：%d 专业：%s\n",name,&age,major);
        std::cout<<name<<" ："<<age<<" ："<<major<<std::endl;

    }
    std::cout<<"--------"<<std::endl;

    return 0;
}