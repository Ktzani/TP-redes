/****************** CLIENT CODE ****************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <string.h>
#include <stdlib.h>

typedef struct ArquivoAtributos
{
    char nomeArquivo[50]; 
    char extensao[10];
    char nome_extensao[60];
    FILE *arq;
    double tamanho;
    
    
} Arquivo;


int main(){
    Arquivo *arquivoEnviado;
    char serverIP[17];
    char nomeArquivo[100];
    int serverPorta;

    char comandoCliente[10];

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

    printf("\nDigite o IP do servidor que deseja conectar: ");
    scanf("%[^\n]%*c", &serverIP);

    printf("\n\nDigite a porta do servidor que deseja conectar: "):
    scanf("%d", &serverPorta);

    /* Set port number, using htons function to use proper byte order */
    serverAddr.sin_port = htons(serverPorta);
    /* Set IP address to localhost */
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);
    /* Set all bits of the padding field to 0 */
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

    printf("\n\nInforme qual arquivo deseja enviar e sua extensão (Exemplo: catiza.txt): ");
    scanf("%[^\n]%*c", arquivoEnviado->nome_extensao);

    if((arquivoEnviado->arq = fopen(arquivoEnviado->nome_extensao, "rb")) == NULL){
        printf("Erro na abertura do arquivo\n")
        return 0;
    }

    arquivoEnviado->tamanho = sizeof(arquivoEnviado->arq);

    /*---- Connect the socket to the server using the address struct ----*/
    addr_size = sizeof serverAddr;
    connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)); //Vai liberar o accept do servidor 

    do {
        //Cliente envia a msg pro servidor e o servidor sai daquele estado de espera de resposta 
        /*---- Send message to the server socket ----*/
        
        send(clientSocket,arquivoEnviado,sizeof(arquivoEnviado),0);
        
        /*---- Read the message from the server into the buffer ----*/
        recv(clientSocket, buffer, sizeof(buffer), 0);

        printf("\nData received: %s\n",buffer);   

        printf("Deseja sair ou continuar enviando mais algum arquivo?\n");
        printf("1 para continuar enviando e 2 para SAIR: ");
        scanf("%[^\n]%*c", &comandoCliente)


    } while (strcmp(comandoCliente, "2") != 0);
 
    printf("Fechando Conexao e encerrando o programa...\n"); 
    close(clientSocket);

    return 0;
}