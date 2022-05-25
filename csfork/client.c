//esse cliente usa fork para gerar os processos filhos
//executar usando um parâmetro que indique a quantidade de filhos
//./multiplex 5

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <arpa/inet.h>

void filhos(int nf){
    int sock, x;
    struct sockaddr_in saddr;
    char buffer[25];
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = 0;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(sock, (const struct sockaddr *)&saddr, sizeof(saddr));

    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    saddr.sin_port = htons(1972);
    
    if((connect(sock, (const struct sockaddr *)&saddr, sizeof(saddr)))!=0){
        perror("Cliente!\n");
        return;
    }
    snprintf(buffer, 128, "dados do Cliente #%i.\n", nf);
    sleep(1);
    printf("filho #%i mandou %i caracteres\n", nf, send(sock, buffer, strlen(buffer), 0));
    sleep(1);
    printf("filho #%i recebeu %i caracteres\n", nf, recv(sock, buffer, 25, 0));
    sleep(1);

    close(sock);
}

int main (int argc, char** argv){
    int nf = 1, pid, x;

    if(argc>1){
        nf=atoi(argv[1]);
    }
    for(x=0; x<nf; x++){
        if((pid=fork())==0){
            filhos(x+1);
            exit(0);
        }
    }
    wait(NULL);
    return 0;
}