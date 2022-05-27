/****************** CLIENT CODE ****************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(){
  int clientSocket;
  char buffer[1024];
  struct sockaddr_in serverAddr;
  socklen_t addr_size;

  /*---- Create the socket. The three arguments are: ----*/
  /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  
  /*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;
  /* Set port number, using htons function to use proper byte order */
  serverAddr.sin_port = htons(7891);
  /* Set IP address to localhost */
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  /* Set all bits of the padding field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  /*---- Connect the socket to the server using the address struct ----*/
  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)); //Vai liberar o accept do servidor 

  do {

    // 1 para horas, 2 para mandar a mensagem de volta e 3 para sair
    printf("\n1: saber as horas do servidor\n");
    printf("qualuqer coisa: manda a mensagem de volta\n");
    printf("3: sair\n");
    printf("Digite sua opcao: ");

    scanf("%[^\n]%*c", buffer);

    //Cliente envia a msg pro servidor e o servidor sai daquele estado de espera de resposta 
    /*---- Send message to the server socket ----*/
    if(strcmp(buffer, "4") == 0){
        send(clientSocket,buffer,sizeof(buffer),0);
        scanf("%[^\n]%*c", buffer);
        send(clientSocket,buffer,sizeof(buffer),0);

    }
    else{
        send(clientSocket,buffer,sizeof(buffer),0);
    }
    
    /*---- Read the message from the server into the buffer ----*/
    recv(clientSocket, buffer, sizeof(buffer), 0);

    printf("\nData received: %s\n",buffer);   


  } while (strcmp(buffer, "3") != 0);
 
  printf("Fechando Conexao e encerrando o programa...\n"); 
  close(clientSocket);

  return 0;
}
