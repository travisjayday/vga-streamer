#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define MAX 6096
#define PORT 8080
#define SA struct sockaddr
  
// Function designed for chat between client and server.
void func(int sockfd)
{
    char buf[MAX];

    // infinite loop for chat
    uint64_t i = 1; 
    char* sig = "FRSTART";
    for (;;) {
        uint8_t r = i;
        uint16_t size = MAX; 
        memcpy(buf, sig, sizeof(sig)); 
        memcpy(buf + sizeof(sig), &size, sizeof(uint16_t)); 
        memset(buf + sizeof(sig) + sizeof(uint16_t), r, MAX - sizeof(sig) + sizeof(uint16_t));
        write(sockfd, buf, sizeof(buf));
        i++;
        usleep(32 * 1000); 
    }
}
 // Driver function
int main()
{

    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;
  
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
  
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("192.168.1.152");
    servaddr.sin_port = htons(PORT);
  
    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
  
    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);
  
    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server acccept failed...\n");
        exit(0);
    }
    else
        printf("server acccept the client...\n");
  
    // Function for chatting between client and server
    func(connfd);
  
    // After chatting close the socket
    close(sockfd);
} 
