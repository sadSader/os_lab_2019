#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include "mixed_server.h"
//#define SERV_PORT 10050
//#define BUFSIZE 100
#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)



   //###################################UDP##############################
void UDP_RECIEVE(int BUFSIZE, int SERV_PORT, int tcp_fd){
     char buf[BUFSIZE], buf2[BUFSIZE]; 
  char mesg[BUFSIZE], ipadr[16];
  struct sockaddr_in udp_servaddr;
  struct sockaddr_in udp_cliaddr;

  int nread;
  int udp_sockfd;
  if ((udp_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket problem");
    exit(1);
  }

  memset(&udp_servaddr, 0, SLEN);
  udp_servaddr.sin_family = AF_INET;
  udp_servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  udp_servaddr.sin_port = htons(SERV_PORT);

  if (bind(udp_sockfd, (SADDR *)&udp_servaddr, SLEN) < 0) {
    perror("bind problem");
    exit(1);
  }
  printf("UDP SERVER starts...\n");

  while (1) {
    unsigned int len = SLEN;
    int n;/*
    if ((n = recvfrom(udp_sockfd, mesg, BUFSIZE, 0, (SADDR *)&udp_cliaddr, &len)) < 0) {
      perror("recvfrom");
      exit(1);
    }    */
    
    nread = read(tcp_fd, buf2, BUFSIZE);
        printf("tcp read %s \n", buf2);
        fflush(0);
    
    while(1){
        nread = read(tcp_fd, buf2, BUFSIZE);
        if(nread>=0)
            break;
         (nread = read(udp_sockfd, buf, BUFSIZE));
             buf[nread]=0;
             printf("SRV::%s \n", buf);
             fflush(0);
    }
   /* if (nread == -1) {
      perror("read");
      exit(1);
    }*/
    printf("SRV:: udp closing");
    fflush(0);
    return;
    close(udp_sockfd);
}
}

    //#################################UDP#####################################



int main(int argc, char *argv[]) {
  const size_t kSize = sizeof(struct sockaddr_in);

  int lfd, tcp_cfd;

  if (argc < 3) {
    printf("Too few arguments \n");
    printf("usage: %s buf_size port\n",argv[0]);
    exit(1);
  }
  int BUFSIZE=atoi(argv[1]);
  int SERV_PORT=atoi(argv[2]);

  char buf[BUFSIZE];
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;

  if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }

  memset(&servaddr, 0, kSize);
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);

  if (bind(lfd, (SADDR *)&servaddr, kSize) < 0) {
    perror("bind");
    exit(1);
  }

  if (listen(lfd, 5) < 0) {
    perror("listen");
    exit(1);
  }

  while (1) {
    unsigned int clilen = kSize;

    if ((tcp_cfd = accept(lfd, (SADDR *)&cliaddr, &clilen)) < 0) {
      perror("accept");
      exit(1);
   }
   printf("SRV:ready to receive");
    const int flags = fcntl(tcp_cfd, F_GETFL, 0);
    fcntl(tcp_cfd, F_SETFL, flags | O_NONBLOCK);
   UDP_RECIEVE(BUFSIZE, atoi(argv[2]), tcp_cfd);
   }
    printf("connection established\n");

    close(tcp_cfd);
  }
