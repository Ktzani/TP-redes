/****************** SERVER CODE ****************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h> 

int main(){
  int welcomeSocket, newSocket;
  char recvBuffer[1024], sendBuffer[1024];
  struct sockaddr_in serverAddr;
  //struct sockaddr_storage serverStorage;
  socklen_t addr_size;

  /*---- Create the socket. The three arguments are: ----*/
  /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */

  //Welcome socket diz relação ao recebimento de conexões, o que inicia o socket
  //SOCKET_STREAM = instancia do TCP;
  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
  
  /*---- Configure settings of the server address struct ----*/
  /* Address family = Internet */
  serverAddr.sin_family = AF_INET;
  /* Set port number, using htons function to use proper byte order */
  serverAddr.sin_port = htons(7891);
  /* Set IP address to localhost */
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Porta que estarei esperando as conexões
  /* Set all bits of the padding field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  /*---- Bind the address struct to the socket ----*/
  bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  time_t ticks; 

    //Depois de eu abrir o socket, eu começo a ouvir , esperando receber ate no maximo 5 conexões
  /*---- Listen on the socket, with 5 max connection requests queued ----*/
  if(listen(welcomeSocket,5)==0)
    printf("Listening\n");
  else
    printf("Error\n");

    //Servidor para esperando uma conexão do cliente. 
    //Faz-se aquela conexão de tres vias que tinhamos visto
  /*---- Accept call creates a new socket for the incoming connection ----*/
  newSocket = accept(welcomeSocket, (struct sockaddr *) NULL, NULL);
  printf ("cliente conectou.\n");

  do {


    /*---- Read the message from the client into the buffer ----*/
    printf("Esperando Mensagem do cliente...\n");
    recv(newSocket, recvBuffer, sizeof(recvBuffer), 0); //Quando uma msg enviada do cliente é recebida pelo servidor

    printf("Mensagem recebida do cliente = %s\n", recvBuffer);

    if (strcmp(recvBuffer, "1") == 0) {
      ticks = time(NULL);
      snprintf(sendBuffer, sizeof(sendBuffer), "%.24s\r\n", ctime(&ticks));
    }

    else if(strcmp(recvBuffer, "4") == 0){
        recv(newSocket, recvBuffer, sizeof(recvBuffer), 0);
        strcpy(sendBuffer,recvBuffer);

    } 
    else if (strcmp(recvBuffer, "3") == 0) {
      strcpy(sendBuffer,"3");
  
    } 
    //Copia o que recebeu do cliente pro sendBuffer
    else {
      strcpy(sendBuffer, recvBuffer);
      strcat(sendBuffer, " -- mensagem do servidor.");
    }

    
    printf("enviando mensagem de volta....\n");
    /*---- Send message to the socket of the incoming connection ----*/
    send(newSocket,sendBuffer,sizeof(sendBuffer),0); //Envio a msg recebida com o objeto do servidor, ou, seja, a resposta para o cliente novamente com um "send"

  }while (strcmp(recvBuffer, "3") != 0);

  printf("fechando conexao...\n");
  close(newSocket);

  return 0;
}
