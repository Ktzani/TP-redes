#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SIZE 1024
#define N 100

void send_file(FILE *fp, int sockfd)
{
  int nb;
  char data[SIZE] = {0};

  nb = fread(data, 1, sizeof(data), fp);

  while (!feof(fp) || nb > 0)
  { 
    if (write(sockfd, data, nb) == -1)
    {
      perror("[-]Error in sending file.");
      exit(1);
    }

    nb = fread(data, 1, sizeof(data), fp);
  }
}

int main()
{
  char *ip = "127.0.0.1";
  int port = 7891;
  int e;

  int sockfd;
  struct sockaddr_in server_addr;
  FILE *fp;
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

  connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

  printf("[+]Connected to Server.\n");

  printf("\nInforme qual arquivo deseja enviar e sua extensão (Exemplo: catiza.txt): ");
  scanf("%[^\n]%*c", filename);

  fp = fopen(filename, "r");
  if (fp == NULL)
  {
    perror("[-]Error in reading file.");
    exit(1);
  }

  //Filename
  send(sockfd, filename, sizeof(filename), 0);
  bzero(filename, N);

  send_file(fp, sockfd);
  printf("[+]File data sent successfully.\n");

  printf("[+]Closing the connection.\n");
  close(sockfd);

  return 0;
}