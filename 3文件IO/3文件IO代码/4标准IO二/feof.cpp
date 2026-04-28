#include<iostream>
#include<cstring>
int main()
{
    FILE *file_ptr = fopen("fflush.cpp","r");
    if(!file_ptr) 
    {
        std::cerr << "Failed to open file" << std::endl;
        return 1;
    }

    char buff[1024] = {0};

    //将实际读取个数给到bytesRead
    size_t bytesRead;
    bytesRead = fread(buff, 1, sizeof(buff), file_ptr);
    std::cout.write(buff, bytesRead)<<std::endl; // 精确输出读取的字节数
    std::cout<<bytesRead<<std::endl;

    
    fclose(file_ptr); 
    std::cout<<std::endl;
    return 0;
}
// #include<iostream>
// int main()
// {   
//     FILE * file_ptr = fopen("fflush.cpp","r");
//     char buff[1024] = {0};
//     while(!feof(file_ptr))
//     {
//         fread(buff,1024,1,file_ptr);
//         std::cout<<buff;
//     }
// 
//     fclose(file_ptr);
//     std::cout<<std::endl;
//     return 0;
// }