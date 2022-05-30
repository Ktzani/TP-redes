#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SIZE 1024

void write_file(int sockfd, const char * filename)
{
  int n;
  FILE *fp;
  char buffer[SIZE];

  fp = fopen(filename, "wb");
  while (1)
  {
    n = recv(sockfd, buffer, SIZE, 0);
    if (n <= 0)
    {
      break;
      return;
    }
    fprintf(fp, "%s", buffer);
    bzero(buffer, SIZE);
  }
  return;
}

int main()
{
  char *ip = "127.0.0.1";
  int port = 7891;
  int e, n;

  int sockfd, new_sock;
  struct sockaddr_in server_addr, new_addr;
  socklen_t addr_size;
  char buffer[SIZE];
  char filename[SIZE];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
  {
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Server socket created successfully.\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  e = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (e < 0)
  {
    perror("[-]Error in bind");
    exit(1);
  }
  printf("[+]Binding successfull.\n");

  if (listen(sockfd, 5) == 0)
  {
    printf("[+]Listening....\n");
  }
  else
  {
    perror("[-]Error in listening");
    exit(1);
  }

  addr_size = sizeof(new_addr);
  new_sock = accept(sockfd, (struct sockaddr *)&new_addr, &addr_size);

  // Filename
  n = recv(sockfd, buffer, SIZE, 0);
  strcpy(filename, buffer);
  printf("[+]%d:File recived: %s\n", n, filename);
  bzero(buffer, SIZE);

  write_file(new_sock, filename);

  printf("[+]Data written in the file successfully.\n");

  return 0;
}