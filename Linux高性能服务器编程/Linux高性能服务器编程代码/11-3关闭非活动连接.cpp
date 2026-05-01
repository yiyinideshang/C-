#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <libgen.h>
#include "11-2升序定时器链表.h"

#define FD_LIMIT 65535
#define MAX_EVENT_NUMBER 1024
#define TIMESLOT 5

static int pipefd[2];
//利用代码清单11-2中的升序定时器链表来管理定时器
static sort_timer_list timer_list;//定时器链表对象
static int epollfd = 0;

int setnonblocking(int fd)
{
    int old_option = fcntl(fd,F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
    return old_option;
}

void addfd(int epollfd,int fd)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
    setnonblocking(fd);
}

void sig_handler(int sig)//信号的处理
{
    int save_errno = errno;
    int msg = sig;
    send(pipefd[1],(char*)&msg,1,0);//向管道中写信号
}

void addsig(int sig)
{
    struct sigaction sa;
    memset(&sa,'\0',sizeof(sa));
    sa.sa_handler = sig_handler;
    sa.sa_flags |= SA_RESETHAND;
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig,&sa,NULL) != -1);
}

void timer_handler()
{
    //定时处理任务，实际上就是调用tick()函数
    timer_list.tick();
    //因为一次alarm调用只会引起一次SIGALRM信号，所以需要重新定时，以不断地触发SIGALRM信号
    alarm(TIMESLOT);
}

//定时器回调函数，它删除非活动连接的socket上的注册事件，并关闭
void cb_func(client_data* user_data)
{
    epoll_ctl(epollfd,EPOLL_CTL_DEL,user_data->sockfd,0);
    assert(user_data);
    close(user_data->sockfd);
    printf("close fd %d\n",user_data->sockfd);
}

int main(int argc,char* argv[])
{
    if(argc<=2)
    {
        printf("Usage: %s ip_address port_number\n", basename(argv[0]));
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);

    int ret = 0;
    struct sockaddr_in address;
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&address.sin_addr);
    address.sin_port = htons(port);

    int listenfd = socket(PF_INET,SOCK_STREAM,0);
    assert(listenfd >= 0);

    ret = bind(listenfd,(struct sockaddr*)&address,sizeof(address));
    assert(ret != -1);

    ret = listen(listenfd,5);
    assert(ret != -1);

    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);
    assert(epollfd != -1);
    addfd(epollfd,listenfd);

    ret = socketpair(PF_UNIX,SOCK_STREAM,0,pipefd);
    assert(ret != -1);
    setnonblocking(pipefd[1]);
    addfd(epollfd,pipefd[0]);

    //设置信号处理函数
    addsig(SIGALRM);
    addsig(SIGTERM);
    bool stop_server = false;

    client_data* users = new client_data[FD_LIMIT];
    bool timeout = false;
    alarm(TIMESLOT);//定时

    while(!stop_server)
    {
        int number = epoll_wait(epollfd,events,MAX_EVENT_NUMBER,-1);
        if((number<0)&&(errno != EINTR))
        {
            printf("epoll failure\n");
            break;
        }
        for(int i = 0;i<number;i++)
        {
            int sockfd = events[i].data.fd;
            //处理新到的客户连接
            if(sockfd == listenfd)
            {
                struct sockaddr_in client_address;
                socklen_t client_addresslength = sizeof(client_address);
                int connfd = accept(listenfd,(struct sockaddr*)&client_address,&client_addresslength);
                addfd(epollfd,connfd);
                users[connfd].address = client_address;
                users[connfd].sockfd = connfd;
                //创建定时器，设置其回调函数与超时时间，然后绑定定时器与用户数据，最后将定时器容器添加到链表timer_list中
                util_timer* timer = new util_timer;
                timer->user_data = &users[connfd];
                timer->cb_func = cb_func;
                time_t cur = time(NULL);
                timer->expire = cur + 3*TIMESLOT;
                users[connfd].timer = timer;
                timer_list.add_timer(timer);
            }
            //处理信号
            else if((sockfd == pipefd[0]) && events[i].events & EPOLLIN)
            {
                int sig;
                char signal[1024];
                ret = recv(pipefd[0],signal,sizeof(signal),0);
                if(ret == -1)
                {
                    //handle ther error
                    continue;
                }
                else if(ret == 0)
                {
                    continue;
                }
                else 
                {
                    for(int i = 0;i<ret;i++)
                    {
                        switch(signal[i])
                        {
                            case SIGALRM:
                            {
                                // 使用timeout变量标记有定时任务需要处理，但是不立即处理定时任务，
                                // 因为定时任务的优先级不是很高，优先处理其他更重要的任务
                                timeout = true;
                                break;
                            }
                            case SIGTERM:
                            {
                                stop_server = true;
                            }
                        }
                    }
                }
            }
            //处理客户连接上收到的数据
            else if(events[i].events & EPOLLIN)
            {
                memset(users[sockfd].buf,'\0',BUFFER_SIZE);
                ret = recv(sockfd,users[sockfd].buf,BUFFER_SIZE-1,0);
                printf("get %d bytes of client data %s from %d \n",ret,users[sockfd].buf,sockfd);

                util_timer* timer = users[sockfd].timer;
                if(ret<0)
                {
                    //如果发生读错误，则关闭连接，并移除其对应的定时器
                    if(errno != EAGAIN)//EAGAIN表示：资源暂时不可用，操作会阻塞
                    //非阻塞模式下，读无数据、写缓冲区满、`accept` 无新连接、`connect` 未完成等。
                    {
                        cb_func(&users[sockfd]);
                        if(timer)
                        {
                            timer_list.del_timer(timer);
                        }
                    }
                    else if (ret == 0)
                    {
                        //如果对方关闭了连接，则我们也关闭连接，并移除其对应的定时器
                        cb_func(&users[sockfd]);
                        if(timer)
                        {
                            timer_list.del_timer(timer);
                        }
                    }
                    else 
                    {
                        //如果某个客户端上有数据可读，则需要调整该连接对应的定时器，来延迟该连接被关闭的时间
                        if(timer)
                        {
                            time_t cur = time(NULL);
                            timer->expire = cur + 3* TIMESLOT;
                            printf("adjust timer once\n");
                            timer_list.adjust_timer(timer);
                        }
                    }
                }
            }
            else 
            {
                //others
            }
        }
        //最后处理定时事件，因为I/O事件具有更高的优先级，这样会导致定时任务不能精准地按照预期的时间执行
        if(timeout)
        {
            timer_handler();
            timeout = false;
        }
    }
    close(listenfd);
    close(pipefd[1]);//先关闭写端
    close(pipefd[0]);//再关闭读端
    delete []users;
    return 0;
}