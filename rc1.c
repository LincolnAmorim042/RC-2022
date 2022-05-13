#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main (int argc, char** argv){
    int mysocket, newsocket, c;
    struct sockaddr_in servidor, cliente;
    
    mysocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mysocket == -1){
        printf("Erro ao criar socket");
    }
    servidor.sin_family = AF_INET;
    servidor.sin_addr.s_addr = INADDR_ANY;
    servidor.sin_port = htons(8888);

    if(bind(mysocket, (struct sockaddr*)&servidor, sizeof(servidor)) < 0){
        puts("Erro no bind");
    }
    puts("Sucesso no bind");

    listen(mysocket, 3);

    puts("Aguardando conexões...");
    c=sizeof(struct sockaddr_in);
    while((newsocket = accept(mysocket, (struct sockaddr*)&cliente, (socklen_t*)&c))){
        if(newsocket < 0){
            perror("Erro ao aceitar a conexao");
        }
        puts("Aceito");
        char* mensagem = "sexo";
        write(newsocket, mensagem, strlen(mensagem));
    }
    
    
    return 0;
}