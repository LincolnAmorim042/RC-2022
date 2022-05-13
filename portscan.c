//testar substituindo strncpy por memset ou memcpy
#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv){
    struct hostent *host;
    int busca, i, sock, inicio, fim;
    char namemach[100];
    struct sockaddr_in servidor;
    
    printf("Nome da máquina ou IP: ");
    fgets(namemach, 100, stdin);

    printf("\nNúmero inicial da porta: ");
    scanf("%d", &inicio);

    printf("\nNúmero final da porta: ");
    scanf("%d", &fim);

    strncpy((char*)&servidor, "", sizeof(servidor));
    servidor.sin_family = AF_INET;
    if(isdigit(namemach[0])){
        printf("inet_addr...");
        servidor.sin_addr.s_addr = inet_addr(namemach);
        printf("Feito");
    }else if ((host = gethostbyname(namemach)) != 0){
        printf("gethostbyname");
        strncpy((char*)&servidor.sin_addr, (char*)host->h_addr, sizeof(servidor.sin_addr));
        printf("Feito");
    }else{
        herror(namemach);
        exit(2);
    }
    printf("Escaneamento de portas");
    for(i = inicio; i<= fim; i++){
        servidor.sin_port = htons(i);
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if(sock<0){
            perror("Falha na conexão");
            exit(1);
        }
        busca = connect(sock, (struct sockaddr*)&servidor,sizeof(servidor));
        if (busca<0){
            printf("Falha na busca, porta %d fechada", i);
            fflush(stdout);
        }else{
            printf("%-5d aberta\n", i);
        }        
    }
    printf("\r");
    fflush(stdout);
    return 0;
}