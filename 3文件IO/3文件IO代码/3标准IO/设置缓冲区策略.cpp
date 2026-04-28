#include <iostream>

int main()
{
    
    char in_buffer[10] = {0};
    setvbuf(stdout,in_buffer,_IOLBF,10);//设置一个标准输出流，以行缓冲进行输出
   
    printf("1234567890");
    printf("\n");
    printf("1234567890");
    printf("\n");  
    fflush(stdout);//手动刷新缓冲区


    return 0;
}