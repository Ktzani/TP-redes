#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SIZE 1024
#define IP_SIZE 16
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
      perror("[-]Erro ao enviar arquivo.");
      exit(1);
    }

    nb = fread(data, 1, sizeof(data), fp);
  }
}

int main()
{
  char ip[IP_SIZE];
  char port_char[N];
  char ch;
  int port;
  int e;

  int sockfd;
  struct sockaddr_in server_addr;
  FILE *fp;
  char filename[SIZE];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
  {
    perror("[-]Erro no socket");
    exit(1);
  }
  printf("[+]Server socket criado com sucesso.\n\n");

  printf("[+]Aperte enter para selecionar o ip padrao (127.0.0.1)\n");
  printf("[+]Digite o ip do server para conectar: ");
  scanf("%[^\n]%*c", ip);
  ch = getchar(); 

  if(ch == '\n')
    strcpy(ip, "127.0.0.1");

  printf("[+]Ip selecionado: %s\n\n", ip);  

  printf("[+]Aperte enter para selecionar a porta padrao (7891)\n");
  printf("[+]Digite a porta para conectar: ");
  scanf("%[^\n]%*c", port_char);
  ch = getchar();

  if(ch == '\n')
    strcpy(port_char, "7891");
  
  port = atoi(port_char);

  printf("[+]Endereco selecionado: %s:%d\n\n", ip, port);

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

  printf("[+]Conectado ao Server.\n");

  printf("[+]Digite o nome do arquivo para ser enviado (file.txt): ");
  scanf("%[^\n]%*c", filename);

  printf("\n");

  fp = fopen(filename, "r");
  if (fp == NULL)
  {
    perror("[-]Error ao abrir arquivo.");
    exit(1);
  }

  //Filename
  send(sockfd, filename, sizeof(filename), 0);

  send_file(fp, sockfd);
  printf("[+]Arquivo enviado com sucesso.\n");

  printf("[+]Fechando conexao.\n");
  close(sockfd);

  return 0;
}