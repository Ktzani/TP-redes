#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define SIZE 1024
#define IP_SIZE 16

void write_file(int sockfd, const char * filename)
{
  int nb;
  FILE *fp;
  char buffer[SIZE];

  fp = fopen(filename, "w");

  nb = read(sockfd, buffer, SIZE);

  while (nb > 0)
  {
    fwrite(buffer, 1, nb, fp); 
    nb = read(sockfd, buffer, SIZE);
  }

  return;
}

int main()
{
  char ip[IP_SIZE];
  char port_char[IP_SIZE];
  char ch;
  int port;
  int e, n;

  int sockfd, new_sock;
  struct sockaddr_in server_addr, new_addr;
  socklen_t addr_size;
  char buffer[SIZE];
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

  e = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (e < 0)
  {
    perror("[-]Erro no bind");
    exit(1);
  }
  printf("[+]Bind feito com sucesso.\n");

  if (listen(sockfd, 5) == 0)
  {
    printf("[+]Ouvindo....\n");
  }
  else
  {
    perror("[-]Erro ao ouvir");
    exit(1);
  }

  addr_size = sizeof(new_addr);
  new_sock = accept(sockfd, (struct sockaddr *)&new_addr, &addr_size);

  // Filename
  recv(new_sock, buffer, SIZE, 0);
  strcpy(filename, buffer);
  printf("[+]Arquivo recebido: %s\n", filename);
  bzero(buffer, SIZE);

  write_file(new_sock, filename);

  printf("[+]Arquivo escrito com sucesso.\n");

  return 0;
}