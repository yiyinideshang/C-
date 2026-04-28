#include<sys/types.h>
#include<iostream>
#include<unistd.h>

int main()
{
    truncate("1.txt",10);

    return 0;
}