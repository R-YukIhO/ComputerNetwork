#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include<pthread.h>
const int N = 1000;
char a[N] = "H";
char b[N] = "H";
SOCKET clientSock;
void *mySend(void*);
void *myRecv(void*);
int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);   //初始化socket库
    clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //创建套接字
    sockaddr_in sockAddr;  //创建地址变量结构体，注意，此处是客户端，该地址变量要存放目标地址
    memset(&sockAddr, 0, sizeof(sockAddr));//客户端无需知道自己的IP，只要知道服务器IP，即可连接
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("111.111.111.111");//目标服务器的IP
    sockAddr.sin_port = htons(9929);//该IP下的具体应用的端口
    connect(clientSock, (SOCKADDR *)&sockAddr, sizeof(SOCKADDR)); //将地址结构体与套接字连接
    pthread_t MySend;  //开始准备传输数据，利用线程并行收发
    pthread_t MyRecv;
    pthread_create(&MySend, NULL, mySend,NULL);
    pthread_create(&MyRecv, NULL, myRecv,NULL);
    pthread_join(MySend,NULL);
    pthread_join(MyRecv,NULL);
    printf("DXY: Over\n");
    closesocket(clientSock);  //收发线程均结束，关闭套接字
    WSACleanup();
        system("pause");
    return 0;
}
void *mySend(void *p)
{
    while (a[0] != '0')
    {
        memset(a, 0, sizeof(a));
        scanf("%s",a);
        send(clientSock, a, N, NULL);
    }
    pthread_exit(NULL);
}
void *myRecv(void *p)
{
    while (b[0] != 0)
    {
        memset(b, 0, sizeof(b));
        recv(clientSock, b, N, NULL);
        printf("DXY: %s\n", b);
    }
    pthread_exit(NULL);
}