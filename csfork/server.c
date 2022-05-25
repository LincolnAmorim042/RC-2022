#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void sigchld_handler(int signo){
    while (waitpid(-1, NULL, WNOHANG)>0);    
}

int main(int argc, char** argv){
    struct sockaddr_in saddr;
    int listensock, nsock, resul, lidos, pid, val;
    char buffer[25];

    listensock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    val = 1;
    resul = setsockopt(listensock, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    if(resul<0){
        perror("erro setsockopt\n");
        return 1;
    }
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(1972);
    saddr.sin_addr.s_addr = INADDR_ANY;

    resul = bind(listensock,(struct sockaddr *) &saddr, sizeof(saddr));
    if(resul<0){
        perror("erro bind\n");
        return 1;
    }

    resul = listen(listensock, 10);
    if(resul<0){
        perror("erro listen\n");
        return 1;
    }

    signal(SIGCHLD, sigchld_handler);

    while (1){
        nsock = accept(listensock, NULL, NULL);
        if((pid = fork()) == 0){
            printf("Processo filho %i criado\n", getpid());
            close(listensock);
            lidos = recv(nsock, buffer, 25, 0);
            buffer[lidos]='\0';
            printf("%s\n", buffer);
            send(nsock, buffer, lidos, 0);
            close(nsock);
            printf("Processo filho %i finalizado\n", getpid());
            exit(0);
        }
    }
    return 0;
}