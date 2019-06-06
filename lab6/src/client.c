#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "utils.h"
struct Server {
  char ip[255];
  int port;
};

uint64_t MultModulo(uint64_t a, uint64_t b, uint64_t mod) {
  uint64_t result = 0;
  a = a % mod;
  while (b > 0) {
    if (b % 2 == 1)
      result = (result + a) % mod;
    a = (a * 2) % mod;
    b /= 2;
  }

  return result % mod;
}

bool ConvertStringToUI64(const char *str, uint64_t *val) {
  char *end = NULL;
  unsigned long long i = strtoull(str, &end, 10);
  if (errno == ERANGE) {
    fprintf(stderr, "Out of uint64_t range: %s\n", str);
    return false;
  }

  if (errno != 0)
    return false;

  *val = i;
  return true;
}

int main(int argc, char **argv) {
  uint64_t k = -1;
  uint64_t mod = -1;
  char servers[255] = {'\0'}; // TODO: explain why 255
  
  unsigned int servers_num = 0;
  struct Server* to;
  
  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {"servers", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0: {
      switch (option_index) {
      case 0:
        ConvertStringToUI64(optarg, &k);
        // TODO: your code here?
        break;
      case 1:
        ConvertStringToUI64(optarg, &mod);
       if(mod<=0)
        printf("mod must be>0");
        break;
      case 2:
        memcpy(servers, optarg, strlen(optarg));
        

        FILE* servers_file=fopen(servers,"r");
        if(servers_file==NULL){
          printf("file openning error\n");
          fflush(0);
          exit(-1);
        };


        int port;
        char ipstr[20];
        fscanf(servers_file, "%u",&servers_num);
        fflush(0);
        to = malloc(sizeof(struct Server) * servers_num);

        for (size_t i = 0; i < servers_num; i++)
        {        
          fscanf(servers_file, "%i %s \n", &port, ipstr );
          to[i].port = port;
          memcpy(to[i].ip, ipstr, sizeof(ipstr));  
        }
        break;
      default:
        printf("Index %d is out of options\n", option_index);
      }
    } break;

    case '?':
      printf("Arguments error\n");
      break;
    default:
      fprintf(stderr, "getopt returned character code 0%o?\n", c);
    }
  }

  if (k == -1 || mod == -1 || !strlen(servers)) {
    fprintf(stderr, "Using: %s --k 1000 --mod 5 --servers /path/to/file\n",
            argv[0]);
    return 1;
  }
    // TODO: delete this and parallel work between servers
  // TODO: work continiously, rewrite to make parallel
  int* server_sockets =malloc(sizeof(char)*servers_num);
  for (int i = 0; i < servers_num; i++) {
    struct hostent *hostname = gethostbyname(to[i].ip);
    if (hostname == NULL) {
      fprintf(stderr, "gethostbyname failed with %s\n", to[i].ip);
      exit(1);
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(to[i].port);
    server.sin_addr.s_addr = *((unsigned long *)hostname->h_addr);

    uint64_t begin = 1;
    uint64_t end = k;

    //char task[sizeof(uint64_t) * 3];
    if(k<servers_num)
      servers_num=k;
    struct FactorialArgs* task=prepare_factorial_args(1, k, mod, servers_num);
    
   // for (size_t i = 0; i < servers_num; i++)
    {

      server_sockets[i] = socket(AF_INET, SOCK_STREAM, 0);
      if (server_sockets[i] < 0) {
        fprintf(stderr, "Socket creation failed!\n");
        exit(1);
      }
    
      if (connect(server_sockets[i], (struct sockaddr *)&server, sizeof(server)) < 0) {
        fprintf(stderr, "Connection failed, port=%u, i=%i\n",to[i].port,i);
        exit(1);
      }

        if (send(server_sockets[i], &task[i], sizeof(struct FactorialArgs), 0) < 0) {
          printf("%i", errno);
          fprintf(stderr, "CLNT::Send failed #%i\n ",i);
          exit(1);
        }
    }
    
    

    
  }

uint64_t answer = 0;
uint64_t result=1;
int ret;
char response[sizeof(uint64_t)];
for (size_t i = 0; i < servers_num; i++) {  
      if (ret=recv(server_sockets[i], response, sizeof(uint64_t), 0) < 0) {
        fprintf(stderr, "CLNT::Recieve failed\n");
        exit(1);
      }
      memcpy(&answer, response, sizeof(uint64_t));
      result=MultModulo(result, answer,mod);

    
      close(server_sockets[i]);
    }
    printf("answer: %llu\n", result);

  free(to);

  return 0;
}