#include <iostream>
#include <stdio.h>
#include <string>
#include <winsock2.h>
#include<pthread.h>
const int N = 1000;
using namespace std;
char a[N] = "H";
char b[N] = "H";
SOCKET clientSock;
void *mySend(void*);
void *myRecv(void*);
int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);//初始化socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//创建套接字
    //AF_INEF=IPV4;SOCK_STREAM=连接传输;最后是TCP协议

    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = INADDR_ANY; //接受该局域网下任何IP为服务器IP
    sockAddr.sin_port = htons(9929);//该程序的端口号

    bind(serverSocket, (SOCKADDR *)&sockAddr, sizeof(SOCKADDR));//将套接字与服务器地址结构体绑定，是服务器端的操作
    listen(serverSocket, 20); //从该套接字入口开始监听

    sockaddr_in clientAddr;//创建客户地址结构体，存储监听到的访问请求方的地址
    int nSize = sizeof(SOCKADDR);
    clientSock = accept(serverSocket, (SOCKADDR *)&clientAddr, &nSize);//通过该函数将客户地址同另一个套接字绑定
    cout << inet_ntoa(clientAddr.sin_addr) << endl; //获取并输出客户端ip
    pthread_t MySend;//开始准备数据传输，通过两个线程并行收发
    pthread_t MyRecv;
    pthread_create(&MySend, NULL, mySend,NULL);
    pthread_create(&MyRecv, NULL, myRecv,NULL);
    pthread_join(MySend,NULL);
    pthread_join(MyRecv,NULL);
    printf("Client: Over\n");
    closesocket(clientSock);  //结束后关闭与客户连接的套接字
    closesocket(serverSocket);//关闭监听套接字
    WSACleanup(); //卸载socket库
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
        printf("Client: %s\n", b);
    }
    pthread_exit(NULL);
}