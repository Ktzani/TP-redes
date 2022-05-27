/****************** SERVER CODE ****************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h> 

typedef struct ArquivoAtributos
{
    char nomeArquivo[50]; 
    char extensao[10];
    char nome_extensao[60];
    FILE *arq;
    double tamanho;
    
} Arquivo;


int main(){
    Arquivo *arquivoRecebido;

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

      //Depois de eu abrir o socket, eu começo a ouvir , esperando receber ate no maximo 5 conexões
    /*---- Listen on the socket, with 5 max connection requests queued ----*/
    if(listen(welcomeSocket,5)==0)
        printf("Listening - Esperando CONEXÃO do cliente\n");
    else
        printf("Error - Erro ao conectar com o cliente\n");

    do {
        /*---- Read the message from the client into the buffer ----*/
        printf("Esperando ARQUIVO do cliente...\n");

        recv(newSocket, arquivoRecebido, sizeof(arquivoRecebido), 0); //Quando uma msg enviada do cliente é recebida pelo servidor

        printf("ARQUIVO recebida do cliente = %s\n", arquivoRecebido->nome_extensao);

        //FALTA PEGAR O ARQUIVO E BAIXA-LO NO SERVIDOR

        printf("Enviando MENSAGEM de volta...\n");
        
        sendBuffer = "ARQUIVO recebido com sucesso 100\%";
        /*---- Send message to the socket of the incoming connection ----*/
        send(newSocket,sendBuffer,sizeof(sendBuffer),0); //Envio a msg recebida com o objeto do servidor, ou, seja, a resposta para o cliente novamente com um "send"

    }while (1);

    printf("fechando conexao...\n");
    close(newSocket);


}