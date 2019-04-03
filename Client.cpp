#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <thread>
#include <atomic>    
using namespace std;

class Test{
public:
    int ReadCin(std::atomic<bool>& run,int sock)
    {
        char buffer[512];
        char sendMessage[512];
        int result;
        while (run.load())
        {
            
            fgets(buffer,512,stdin);
            send(sock,buffer,strlen(buffer), 0);
            printf("Send The message:%s ",buffer);
            if (buffer == "Quit")
            {
                run.store(false);
            }
        }
    }
};

int main()
{
    Test a;
        char sendMessage[512],receiveMessage[512]; //length of messages to send and recieve
        int sock, result;                                                                                                                                   
        struct hostent *host;   //Returns a pointer to a host name,null pointer indicates the network address was not found in the network file. 
        struct sockaddr_in serverAdd; 
        host = gethostbyname("192.168.126.197");     
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) // socket prototype socket(domain, type, protocol) 
        {
            perror("Socket creation failed");
            exit(1);
        }
        serverAdd.sin_family = AF_INET; //AF_INET refers to addresses from the internet, IP addresses specifically    
        serverAdd.sin_port = htons(5000); // (16 bit port number)htons:function makes sure that numbers are stored in memory in network byte order,which is usually big-endian
        serverAdd.sin_addr = *((struct in_addr *)host->h_addr); // 32 bit ip address
        bzero(&(serverAdd.sin_zero),8);
        //Connect function is used by a TCP client to establish a connection with a TCP server
        if (connect(sock, (struct sockaddr *)&serverAdd, sizeof(struct sockaddr)) == -1)
        {
            perror("Connection failed");
            exit(1);
        }
        std::atomic<bool> run(true);
        thread first(&Test::ReadCin,&a,std::ref(run),sock);
        while (run.load())
        {
            result = recv(sock,receiveMessage,1024,0);   //recv(socket descriptor,the pointer to buffer that recieves data,)
            receiveMessage[result] = '\0'; 
            printf("Recieved Message: %s " , receiveMessage);
        }        

        run.store(false);
        first.join();
        return 0;
}