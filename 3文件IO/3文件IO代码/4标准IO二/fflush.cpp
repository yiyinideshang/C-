#include<iostream>

int main()
{
    FILE *file_ptr = fopen("fflush.cpp","a+");
    fwrite("//123456",1,8,file_ptr);
    fclose(file_ptr);
    return 0;
}//123456