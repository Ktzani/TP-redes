/****************** CLIENT CODE ****************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 1024

typedef struct ArquivoAtributos
{
    char nome[30];
    char extensao[30];
    FILE *arq;
    size_t tamanho;

} Arquivo;

const char *separaNomeDaExtensao(const char *filename)
{
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename)
        return "";
    return dot + 1;
}

const char *separaNomeDoArquivo(const char *filename)
{
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename)
        return "";
    return dot - 1;
}

Arquivo converteParaBinario(Arquivo arq)
{
    char c;
    Arquivo aux;

    printf("%s\n", arq.nome);

    char nomeArquivo[20];

    char *token = strtok(arq.nome, ".");

    strcpy(nomeArquivo, token);

    strcpy(aux.extensao, "bin");

    strcat(nomeArquivo, ".");

    strcat(nomeArquivo, aux.extensao);

    strcpy(aux.nome, nomeArquivo);

    printf("%s - %s\n", arq.nome, aux.nome);

    // NAO FUNCIONA DAQUI PARA BAIXO

    arq.arq = fopen(arq.nome, "r");
    aux.arq = fopen(aux.nome, "wb");

    do
    {
        c = getc(arq.arq);
        printf("%c", c);
        fputc(c, aux.arq);
    } while (!feof(arq.arq));

    fclose(arq.arq);
    fclose(aux.arq);

    return aux;
}

void send_file(FILE *fp, int sockfd)
{
    int n;
    char data[SIZE] = {0};

    while (fgets(data, SIZE, fp) != NULL)
    {
        if (send(sockfd, data, sizeof(data), 0) == -1)
        {
            perror("[-]Error in sending file.");
            exit(1);
        }
        bzero(data, SIZE);
    }
}

int main()
{
    Arquivo arquivoEnviado;
    char serverIP[17];
    int serverPorta;

    char comandoCliente[10];
    char tipoEntrada[3];
    size_t size;

    int clientSocket;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

    printf("\nInforme qual arquivo deseja enviar e sua extensão (Exemplo: catiza.txt): ");
    scanf("%[^\n]%*c", arquivoEnviado.nome);

    strcpy(arquivoEnviado.extensao, separaNomeDaExtensao(arquivoEnviado.nome));

    // Se for txt, transformo em binario
    if (strcmp(arquivoEnviado.extensao, "txt") == 0)
        arquivoEnviado = converteParaBinario(arquivoEnviado);

    printf("\nDigite o IP do servidor que deseja conectar: ");
    scanf("%[^\n]%*c", serverIP);

    printf("\nDigite a porta do servidor que deseja conectar: ");
    scanf("%d", &serverPorta);
    getchar();

    /*---- Create the socket. The three arguments are: ----*/
    /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);

    /*---- Configure settings of the server address struct ----*/
    /* Address family = Internet */
    serverAddr.sin_family = AF_INET;
    /* Set port number, using htons function to use proper byte order */
    serverAddr.sin_port = htons(serverPorta);
    /* Set IP address to localhost */
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);
    /* Set all bits of the padding field to 0 */
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    /*---- Connect the socket to the server using the address struct ----*/
    addr_size = sizeof serverAddr;
    connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)); // Vai liberar o accept do servidor

    do
    {
       

        // // Abrindo arquivo
        // if ((arquivoEnviado.arq = fopen(arquivoEnviado.nome, "rb")) == NULL)
        // {
        //     printf("Erro na abertura do arquivo\n");
        //     return 0;
        // }

        // // Pegando o tamanho do arquivo
        // fseek(arquivoEnviado.arq, 0, SEEK_END);
        // size = ftell(arquivoEnviado.arq);
        // arquivoEnviado.tamanho = size;

        // strcpy(buffer, arquivoEnviado.nome);
        // send(clientSocket, buffer, sizeof(buffer), 0);

        // // Envio o tamanho do arquivo para o servidor saber quantos pacotes estarão chegando
        // // e quantos receives serão necessários
        // send(clientSocket, size, sizeof(size), 0);

        // // Voltando para o inicio do arquivo para pegar os "pacotes"
        // fseek(arquivoEnviado.arq, 0, SEEK_SET);

        // // Vou enviando pacotes a pacotes para o servidor
        // while (!feof(arquivoEnviado.arq))
        // {
        //     fread(buffer, sizeof(char), 1024, arquivoEnviado.arq);

        //     // Cliente envia a msg pro servidor e o servidor sai daquele estado de espera de resposta
        //     /*---- Send message to the server socket ----*/
        //     send(clientSocket, buffer, sizeof(buffer), 0);

        //     bzero(buffer, 1024);
        // }

        send_file(arquivoEnviado.arq, clientSocket);

        /*---- Read the message from the server into the buffer ----*/
        recv(clientSocket, buffer, sizeof(buffer), 0);

        printf("\nData received: %s\n", buffer);

        printf("Deseja sair ou continuar enviando mais algum arquivo?\n");
        printf("1 para continuar enviando e 2 para SAIR: ");
        scanf("%[^\n]%*c", comandoCliente);

        fclose(arquivoEnviado.arq);

    } while (strcmp(comandoCliente, "2") != 0);

    printf("Fechando Conexao e encerrando o programa...\n");
    close(clientSocket);

    return 0;
}
