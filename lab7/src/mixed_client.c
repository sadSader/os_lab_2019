#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

//#define BUFSIZE 100
#define SADDR struct sockaddr
#define SIZE sizeof(struct sockaddr_in)
#define SLEN sizeof(struct sockaddr_in)

int main(int argc, char *argv[]) {
  if (argc !=4) {
    printf("Too few arguments \n");
    printf("usage:%s ip port buff_size",argv[0]);
    exit(1);
  }

  int fd;
  int nread;
  int BUFSIZE=atoi(argv[2]);
  char buf[BUFSIZE];
  struct sockaddr_in tcp_servaddr;

  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket creating");
    exit(1);
  }

  memset(&tcp_servaddr, 0, SIZE);
  tcp_servaddr.sin_family = AF_INET;

  if (inet_pton(AF_INET, argv[1], &tcp_servaddr.sin_addr) <= 0) {
    perror("bad address");
    exit(1);
  }

  tcp_servaddr.sin_port = htons(atoi(argv[2]));

  if (connect(fd, (SADDR *)&tcp_servaddr, SIZE) < 0) {
    perror("connect");
    exit(1);
  }

///###########UDP#######################
 char sendline[BUFSIZE], recvline[BUFSIZE + 1];
  struct sockaddr_in udp_servaddr;
  struct sockaddr_in udp_cliaddr;
  int udp_sockfd;

  memset(&udp_servaddr, 0, sizeof(udp_servaddr));
  udp_servaddr.sin_family = AF_INET;
  udp_servaddr.sin_port = htons(atoi(argv[2]));

  if (inet_pton(AF_INET, argv[1], &udp_servaddr.sin_addr) < 0) {
    perror("inet_pton problem");
    exit(1);
  }
  if ((udp_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket problem");
    exit(1);
  }

usleep(100)  ;
for(int i=0; i<20;++i){
  char*sendline=malloc(20*sizeof(char));
    
    sprintf(sendline, "%i",i);    
  int res;
  if(res=sendto(udp_sockfd, sendline, strlen(sendline), 0, (SADDR *)&udp_servaddr, SLEN) == -1) {
      perror("sendto problem");
      exit(1);
    }


  }
  sleep(1);
  char endbuf[4]="END";
  write(fd,endbuf,sizeof(buf));

  close(fd);
  printf("CLNT::finished");
  fflush(0);
  exit(0);
}