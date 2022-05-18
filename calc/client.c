#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

int main(int argc, char** argv){
    int mysocket;
    struct sockaddr_in addr;
    mysocket = socket(AF_INET, SOCK_STREAM, 0);
    if(mysocket == -1){
        printf("Erro ao criar socket");
        return 1;
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));
    printf("Tentando se conectar ao servidor....\n");

    if(connect(mysocket, (struct sockaddr *)&addr, sizeof(addr)) == -1){
        printf("Erro ao se conectar!\n");
        return 1;
    }
    printf("Conectado!");
    int recebidos, enviados;
    char mensagem[256];
    char resposta[256];

    do{
        //parte cliente
        printf("Cliente....\n");
        fgets(mensagem, 256, stdin);
        mensagem[strlen(mensagem) - 1] = '\0';
        enviados = send(mysocket, mensagem, strlen(mensagem), 0);

        //parte servidor
        recebidos = recv(mysocket, resposta, 256, 0);
        resposta[recebidos] = '\0';
        printf("Servidor: %s \n", resposta);

    }while(recebidos != -1 && enviados != -1);
    return 0;
}//fim main