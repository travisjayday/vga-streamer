#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sys/time.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void func(int sockfd)
{
	char buff[MAX];
	int n;
    int recvd = 0;
    int recvd_total = 0;
    struct timeval st, et;
    gettimeofday(&st, NULL);
	for (;;) {
		bzero(buff, sizeof(buff));
		recvd = read(sockfd, buff, sizeof(buff));
        recvd_total += recvd;
		printf("Received: %dB\r", recvd_total);
        if (recvd == 0) break;
	}
    gettimeofday(&et, NULL);

    int elapsed = ((et.tv_sec - st.tv_sec) * 1000000) + (et.tv_usec - st.tv_usec);
    double elapsed_seconds = (double) elapsed / 1.0e6; 
    printf("\nElapsed Seconds: %f seconds\n", elapsed_seconds);
    printf("KBytes/Seconds: %f seconds\n", (double) recvd_total / elapsed_seconds / 1024);
}

int main()
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("192.168.1.152");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	// function for chat
	func(sockfd);

	// close the socket
	close(sockfd);
}

