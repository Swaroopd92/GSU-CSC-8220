#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>  
#include <arpa/inet.h>  
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> 

int actvt, a, MaximumSocdes, Client, reading, socdes, loop_no,c,no;
int len_of_addr, NewSoc, no_of_packets, MastSoc;
int opt = 1;
int ClientSoc[5];
char buffer[2048];
int MaximumNoClients = 3;
struct sockaddr_in addr;
fd_set readfds;
no_of_packets = 0;
MastSoc = 0;
len_of_addr = 0;
NewSoc = 0;
a = 0;
c = 1;

void errors() {
    if((MastSoc = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket Connection Failure");
        exit(EXIT_FAILURE);
    }
    else if( setsockopt(MastSoc, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("Set Socket OPT");
        exit(EXIT_FAILURE);
    }
    if (bind(MastSoc, (struct sockaddr *)&addr, sizeof(addr))<0)
    {
        perror("Socket Binding Failure");
        exit(EXIT_FAILURE);
    }
    else if (listen(MastSoc, 3) < 0)
    {
        perror("Listen");
        exit(EXIT_FAILURE);
    }
    
}

void connecting() {
    for( ; ; )
    {
        FD_ZERO(&readfds);
        FD_SET(MastSoc, &readfds);
        MaximumSocdes = MastSoc;
        a=0;
        while(a!= MaximumNoClients)
        {
            socdes = ClientSoc[a];
            if(socdes > 0)
                FD_SET( socdes , &readfds);
            if(socdes > MaximumSocdes)
                MaximumSocdes = socdes;
            a++;
        }
        actvt = select( MaximumSocdes + 1 , &readfds , NULL , NULL , NULL);
        if ((actvt < 0))
        {
            printf("Error in Selection");
        }
        if (FD_ISSET(MastSoc, &readfds))
        {
            if ((NewSoc = accept(MastSoc, (struct sockaddr *)&addr, (socklen_t*)&len_of_addr))<0)
            {
                perror("Accept");
                exit(EXIT_FAILURE);
            }
            printf("Connection Established\n");
            printf("IP Address: %s\n",inet_ntoa(addr.sin_addr));
            printf("Port: %d \n",ntohs(addr.sin_port));
            printf("Socket fd: %d\n", NewSoc);
            a=0;
            while(a!= MaximumNoClients)
            {
                if(ClientSoc[a] == 0)
                {
                    ClientSoc[a] = NewSoc;
                    if(c==1)
                    {
                        printf("Connection Established with Sender!!\n\n");
                    }
                    if(c==0)
                    {
                        printf("Connection Established with Receiver!!\n\n");
                    }
                    c=0;
                    break;
                }
                a++;
            }
            
        }
        a=0;
        while(a!= MaximumNoClients)
        {
            socdes = ClientSoc[a];
            if (FD_ISSET( socdes , &readfds))
            {
                if ((reading = read( socdes , buffer, 1024)) == 0)
                {
                    getpeername(socdes , (struct sockaddr*)&addr , (socklen_t*)&len_of_addr);
                    close( socdes );
                    ClientSoc[a] = 0;
                }
                else
                {
                    no_of_packets++;
                    no = 13;
                    //Here My team Probability is 10+6 , so for every 100 packets, 16 packets are lost, for every 6 packets, 1 packet is lost. This is the logic for the same. 6 depicts the number of packets after which a packet will be dropped by the router.
                    if(no_of_packets%no != 0)
                    {
                        continue;
                    }
                    else
                    {
                        buffer[reading] = '\0';
                        break;
                    }
                    Client = 0;
                    loop_no = 0;
                    buffer[reading] = '\0';
                    while(loop_no < MaximumNoClients)
                    {
                        Client = ClientSoc[loop_no++];
                        if (Client != socdes && Client != 0)
                            send(Client , buffer , strlen(buffer) , 0 );
                    }
                }
            }
            a++;
        }
    }
}

int main( ) {
    while(a!= MaximumNoClients)
    {
        ClientSoc[a] = 0;
        a++;
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    //This is the port Number
    addr.sin_port = htons( 2244 );
    
    errors();
    
    len_of_addr = sizeof(addr);
    
    connecting();
    
    return 0;
    
}