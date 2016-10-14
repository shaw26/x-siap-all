#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAXLINE 4096

int main(int argc, char** argv) {
  int sockfd, n, rec_len;
  char recvline[4096], sendline[4096];
  char buf[MAXLINE];
  struct sockaddr_in servaddr;
  if(argc != 2) {
    printf("Usage: ./client <IPaddress>\n");
    exit(0);
  }

  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Create socket error: s(errno: %d)\n", strerror(errno), errno);
    exit(0);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(8000);
  if(inet_pton(AF_INET, argv[1], (void*)&servaddr.sin_addr) <= 0) {
    printf("inet_ptons error for %s\n", argv[1]);
    exit(0);
  }

  if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
    printf("Connect error: %s(errno: %d)\n", strerror(errno), errno);
    exit(0);
  }

  printf("send message to server: \n");
  fgets(sendline, MAXLINE, stdin);
  if(send(sockfd, sendline, strlen(sendline), 0) < 0) {
    printf("Send message error: %s(errno: %d)\n", strerror(errno), errno);
    exit(0);
  }

  if((rec_len = recv(sockfd, buf, MAXLINE,0)) == -1) {
    printf("recv error: %s(errno: %d)\n");
    exit(1);
  }

  buf[rec_len]  = '\0';
  printf("Received : %s ",buf);
  close(sockfd);
  exit(0);
}

