/*************************************************************************
  > File Name: server_client.c
  > Author: stmatengss
  > Mail: stmatengss@163.com 
  > Created Time: Mon Jan  9 10:23:47 2017
 ************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>

// Compare, print, and exit
#define CPE(val, msg, err_code) \
if(val) { fprintf(stderr, msg); fprintf(stderr, " Error %d \n", err_code); \
exit(err_code);}

#define MEM_MAX 2048

int main(int argc, char **argv) {

	int sockfd, connfd, n;
	int result;
	int is_server;
	char send_mem[MEM_MAX], recv_mem[MEM_MAX];
	uint32_t port;
	struct in_addr addr;
	struct sockaddr_in server_addr;

	while ((result = getopt(argc, argv, "scp:i:")) != -1) {
		switch (result) {
			case 's':
				is_server = 1;
				break;
			case 'c':
				is_server = 0;
				break;
			case 'p':
				port = atoi(optarg);
				break;
			case 'i':
				if (is_server == 1) {
					CPE(1, "./server_client [-s|-c] [-i] addr [-p] port", 0);
				}
				inet_pton(AF_INET, optarg, &addr);
				break;
			default:
				CPE(1, "./server_client [-s|-c] [-i] addr [-p] port", 0);
		}
	}

	if (is_server == 0) {
		memset(&server_addr, 0, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(port);
		server_addr.sin_addr = addr;
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		CPE((connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr))), "error", 0);
		int count = 0;
		while(1) {
			printf("msg %d input\n:", count++);
			fgets(send_mem, MEM_MAX, stdin);
			CPE(send(sockfd, send_mem, strlen(send_mem), 0), "error", 0);
		}
		close(sockfd);
	} else {
		memset(&server_addr, 0, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(port);
		server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		CPE(bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)), "error bind", 0);
		CPE(listen(sockfd, 10), "error listen", 0);
		int count = 0;
		while (1) {
			connfd = accept(sockfd, (struct sockaddr*)NULL, NULL);
			n = recv(connfd, recv_mem, MEM_MAX, 0);
			recv_mem[n] = '\n';
			printf("msg %d output:\n", count);
			printf("%s\n", recv_mem);
			close(connfd);
		}
		close(sockfd);
	}
	return 0;
}

