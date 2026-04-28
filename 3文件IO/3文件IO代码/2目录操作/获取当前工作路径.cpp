#include<unistd.h>
#include<iostream>

int main()
{
    char *currrent_directory = get_current_dir_name();
    std::cout<<"当前的工作路径是："<<currrent_directory<<std::endl;
    free(currrent_directory);

    return 0;
}