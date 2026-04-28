#include<iostream>
#include<regex.h>

// 正则表达式 ，原始字符串：正则表达式
#define IPREGEX "[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.([0-9]{1,3})"
#define MAXSIZE 512

int main()
{
    // 创建一个变量：用于存储错误码
    int errcode = 0;
    // 创建一个字符串数组，用于存储错误信息
    char errbuf[MAXSIZE]={0};

    // 需要匹配的内容的母串
    const char * src_ptr =
    "192.168.31.1adsadasdadasdsadasd456as35d4as53d12313as192.163.32.2";

    // 场景一个regex来存储编译后的正则表达式
    regex_t regex;

    // 编译正则表达式
    errcode = regcomp(&regex,IPREGEX,REG_EXTENDED);
    if(errcode!=0)
    {
        regerror(errcode,&regex,errbuf,MAXSIZE);
        std::cout << "正则表达式编译错误:" << errbuf << std::endl;
        return -1;
    }

    // 定义一个数组，用于存储匹配到的结果数据
    regmatch_t pmatch[2];

    // 定义一个变量，表示偏移量：跳过已经匹配过结果
    int offset=0;

    do
    {
        errcode = regexec(&regex,offset+src_ptr,2,pmatch,0);
        if(errcode!=REG_NOMATCH) // 匹配到了结果
        {
            // 打印一下匹配到的这个结果的下标
            std::cout << "结果下标:" << pmatch[0].rm_so << ":" << pmatch[0].rm_eo
            << std::endl;

            // 打印匹配结果
            for(int pos = offset+pmatch[0].rm_so;pos <offset+pmatch[0].rm_eo;pos++)
            std::cout << src_ptr[pos];
            std::cout << std::endl;

            // 更新偏移量，向后跳当前结果的字节数
            offset+=pmatch[0].rm_eo;
        }
    } while (errcode!=REG_NOMATCH);

    // 清理正则表达式对象
    regfree(&regex);

    return 0;    
    
}