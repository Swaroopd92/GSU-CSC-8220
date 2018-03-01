#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int rtr, Y, y=0, cc;
int reading, actvt, writing = 0, count = 1;
char buffer[2048];
struct sockaddr_in raddr;
struct hostent* haddr;
struct in_addr* addr;
struct timeval timeout;
fd_set read_fd;

void printing() {
    for(y=1; y<Y+1; y++)
    {
        bzero(buffer, 2048);
        strncpy(buffer, "Sending Packet", strlen("Sending Packet"));
        printf("Sending Packet %d \n",count);
        cc = 0;
        
    resending:
        
        writing = write(rtr, buffer, strlen(buffer));
        if (writing < 0)
        {
            printf("ERROR : Cannot Write");
            continue;
        }
        //If the packet is not delivered within the given timeout, the packet will be resent. Timeout here is 15 secs.
        timeout.tv_sec  = 15;
        timeout.tv_usec = 0;
        FD_ZERO(&read_fd);
        FD_SET(rtr, &read_fd);
        actvt = select(rtr+1, &read_fd, NULL, NULL, &timeout);
        if (actvt>0||actvt<0)
        {
            if (FD_ISSET(rtr, &read_fd))
            {
                bzero(buffer, 2048);
                reading = read(rtr, buffer, 2048);
                if (reading < 0)
                {
                    printf("ERROR : Cannot Read");
                    continue;
                }
                printf("Recieving Acknowledgement %d\n\n",count);
            }
        }
        else if(actvt == 0)
        {
            cc++;
            if(cc>1)
                break;
            printf("Resending packet %d..\n", count);
            goto resending;
        }
        count++;
    }
}

int main() {
    
    
    rtr = socket(AF_INET, SOCK_STREAM, 0);
    if ( rtr < 0 )
    {
        printf(" Unable to Create a socket for Router\n");
        return 0;
    }
    
    printf("Enter the number of Packets to be sent:  ");
    scanf("%d", &Y);
    bzero((char *) &raddr, sizeof(raddr));
    haddr = gethostbyname("localhost");
    addr = (struct in_addr*)haddr->h_addr_list[0];
    raddr.sin_family = AF_INET;
    raddr.sin_port   = htons(2244);
    raddr.sin_addr.s_addr = addr->s_addr;
    if (connect(rtr, (struct sockaddr *)&raddr,sizeof(raddr)) < 0)
    {
        printf("ERROR : Cannot Connect to Router");
        return 0;
    }
    FD_ZERO(&read_fd);
    FD_SET(rtr, &read_fd);
    printf("Connected to Router \n\n");
    printing();
    return 0;
}
