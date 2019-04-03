#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <thread>
using namespace std;
void recieve(int connected)
{
    char receiveMessage[1024];
    int result;
    while(1)
    {
        //connected here is socket descriptor 
        result = recv(connected,receiveMessage,512,0);
        receiveMessage[result] = '\0';
        printf(" Received message : %s" , receiveMessage);
                        
    }
}
class Test{
public:
    int send1(int connected)
    {
        
        char sendMessage[1024];
        while(1)
        {
                    
                                   
            fgets(sendMessage,100,stdin);
            send(connected, sendMessage,strlen(sendMessage), 0);  
            printf(" SEND the message :%s",sendMessage );
    
        }
    }
};
int main()
{   
        Test a;
        int b=1;
        char sendMessage[1024] ,receiveMessage[1024];
        int sock, connected, result;        
        struct sockaddr_in serverAdd, clientAdd;   // defined them as we don't want to hide the identity of client from server 
        socklen_t length;
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) // socket(domain, type, protocol) 
            // specifying the type of communication protocol (TCP IPv4 or IPv6 or UDP)
        {
            perror("Socket creation is failed");
            exit(1);
        }
 
        if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&b,sizeof(int)) == -1) 
        {
            perror("Set socket option");
            exit(1);
        }
       
        serverAdd.sin_family = AF_INET;  //domain      
        serverAdd.sin_port = htons(5000); // (16 bit port number)htons:function makes sure that numbers are stored in memory in network byte order,which is usually big-endian
        serverAdd.sin_addr.s_addr = INADDR_ANY; // 32 bit ip address 
        bzero(&(serverAdd.sin_zero),8);
    
        /*
        Bind : assigns local address to socket 
         sock : is socket descriptor
         (struct sockaddr *)&serverAdd : is a pointer to a protocol-specific address
         sizeof(struct sockaddr) : is the size of the address structure

        */

        if (bind(sock, (struct sockaddr *)&serverAdd, sizeof(struct sockaddr))== -1) 
        {
            perror("Unable to bind");
            exit(1);
        }

        /* Listen : converts an unconnected socket into a passive socket
        sock :  is the socket descriptor
        here 3 is the is the maximum number of connections the kernel should queue for this socket
        */

        if (listen(sock, 3) == -1)
        {
            perror("Listen");
            exit(1);
        }
        printf("TCPServer Waiting for client connection\n");
        fflush(stdout);
        
        /*
        accept:  is used to retrieve a connect request and convert that into a request
        sock : is the socket descriptor
        (struct sockaddr *)&clientAdd : returns the address of the client
        &length :  pointer to length of client add 
        */

        while(1)
        { 
            length = sizeof(struct sockaddr_in); // setting the pointer large enough to hold the address 
            connected = accept(sock,(struct sockaddr *)&clientAdd,&length);
            //inet_ntoa : converts network address to dots-number format string 
            printf("Server is connected with IP address %s and port %d    \n",inet_ntoa(clientAdd.sin_addr),ntohs(clientAdd.sin_port));
            thread first(recieve,connected);
            thread second(&Test::send1,&a,connected);
            first.join();
            second.join();      
        }   
        return 0;
}

