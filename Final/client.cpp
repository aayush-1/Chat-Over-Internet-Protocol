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
using namespace std;
#include <thread>
#include <atomic>    
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

/*void recieve(int sock)
{
    char receiveMessage[512];
    int result;
    while(1)
    {
                        result = recv(sock,receiveMessage,1024,0);   //recv(socket descriptor,the pointer to buffer that recieves data,)
                        receiveMessage[result] = '\0';
                        printf("\nRecieved Message: %s " , receiveMessage);
                        
    }
}*/
/*class Test{
public:
    int send1(int sock)
    {
        
        char sendMessage[512];
        while(1)
        {
            
                            printf("\nSEND The message: ");
                            fgets(sendMessage,512,stdin);
                            send(sock,sendMessage,strlen(sendMessage), 0);   
        }
    }
};*/
int main()
{
    Test a;
        char sendMessage[512],receiveMessage[512];
        int sock, result;                                                                                                           
        struct hostent *host;
        struct sockaddr_in serverAdd; 
        host = gethostbyname("192.168.110.175");     
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
            {
            perror("Socket creation failed");
            exit(1);
            }
        serverAdd.sin_family = AF_INET;    
        serverAdd.sin_port = htons(5000);  
        serverAdd.sin_addr = *((struct in_addr *)host->h_addr);
        bzero(&(serverAdd.sin_zero),8);
        if (connect(sock, (struct sockaddr *)&serverAdd, sizeof(struct sockaddr)) == -1)
        {
            perror("Connection failed");
            exit(1);
        }
        std::atomic<bool> run(true);
        //thread first(recieve,sock);
        thread first(&Test::ReadCin,&a,std::ref(run),sock);
        while (run.load())
        {
                        result = recv(sock,receiveMessage,1024,0);   //recv(socket descriptor,the pointer to buffer that recieves data,)
                        receiveMessage[result] = '\0';
                        printf("Recieved Message: %s " , receiveMessage);

        }
        
        run.store(false);
        first.join();
        //second.join();

        /*while(1)
        {
                        result = recv(sock,receiveMessage,1024,0);   //recv(socket descriptor,the pointer to buffer that recieves data,)
                        receiveMessage[result] = '\0';
                        printf("\nRecieved Message: %s " , receiveMessage);
                        printf("\nSEND The message: ");
                        fgets(sendMessage,512,stdin);
                        send(sock,sendMessage,strlen(sendMessage), 0);
        }  */
        return 0;
}