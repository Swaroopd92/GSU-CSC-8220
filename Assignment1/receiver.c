#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <math.h>

#define IP "127.0.0.1"

int rtr;
int reading = 0, writing = 0, count = 1;
char buffer[2048];
struct sockaddr_in raddr;
struct hostent* haddr;
struct in_addr* addr;

int main() {
    
    rtr = socket(AF_INET, SOCK_STREAM, 0);
    if(rtr < 0)
    {
        printf(" Unable to Create a socket for Router\n");
        return 0;
    }
    bzero((char *) &raddr, sizeof(raddr));
    haddr = gethostbyname("localhost");
    addr = (struct in_addr*)haddr->h_addr_list[0];
    raddr.sin_family = AF_INET;
    raddr.sin_port   = htons(2244);
    raddr.sin_addr.s_addr = (char *) malloc(sizeof(IP));
    raddr.sin_addr.s_addr = inet_addr((char *)IP);
    if (connect(rtr, (struct sockaddr *)&raddr,sizeof(raddr)) < 0)
    {
        printf("ERROR : Cannot Connect to Router");
        return 0;
    }
    for( ; ; )
    {
        bzero(buffer, 2048);
        reading = read(rtr, buffer, 2048);
        if ( reading < 0 )
        {
            printf("ERROR : Cannot Read");
            continue;
        }
        
        printf("Recieving packet %d \n", count);
        printf("Sending Acknowldge %d \n\n", count);
        count++;
        writing = write(rtr, buffer, strlen(buffer));
        if (writing < 0)
        {
            printf("ERROR : Cannot Write");
            continue;
        }
    }
    return 0;
}