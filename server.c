#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAXLINE 4096
#define DEFAULT_PORT 8000

int main(int argc, char** argv) {
  int listenfd, connfd;
  struct sockaddr_in servaddr;
  char buff[4096];
  int n;
  if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    printf ("Create socket error: %s(errno: %d)\n", strerror(errno), errno);
    exit(0);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(DEFAULT_PORT);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
    printf("Listen socket error: %s(errno: %d)\n", strerror(errno), errno);
    exit(0);
  }

  if(listen(listenfd, 10) == -1) {
    printf("Listen error: %s(errno: %d)\n", strerror(errno), errno);
    exit(0);
  }

  printf("=======waiting for client's request======\n");
  while(1) {
   if((connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1) {
     printf("Accept error: %s(errno: %d)\n", strerror(errno), errno);
     continue;
   }

    n = recv(connfd, buff, MAXLINE, 0);
    if(!fork()) {
      if (send(connfd, "Hello, you are connected!\n", 26, 0) == -1) {
        printf("Send error: %s(errno: %d)\n", strerror(errno), errno);
      }
      close(connfd);
      exit(0);
    }
    buff[n] = '\0';
    printf("receive message from client: %s\n", buff);
    close(connfd);
  }

  close(listenfd);
}
