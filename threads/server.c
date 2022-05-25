//gcc server.c -o server -lpthread

#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* threadhandler(void* arg){
    int sock, nlidos;
    char buff[25];

    printf("Thread filha %li com pid %i criada\n", pthread_self(), getpid());
    sock = (int)arg;
    nlidos=recv(sock, buff, 25, 0);
    buff[nlidos]='\0';
    printf("%s\n", buff);
    send(sock, buff, nlidos, 0);
    close(sock);
    printf("Thread filha %li com pid %i finalizada\n", pthread_self(), getpid());
}

int main(int argc, char** argv){
    struct sockaddr_in saddr;
    int listensock, nsock, resul, lidos, val;
    char buffer[25];
    pthread_t id;

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
    }
    resul = listen(listensock, 10);
    if(resul<0){
        perror("erro listen\n");
        return 1;
    }

    while (1){
        nsock = accept(listensock, NULL, NULL);

        resul=pthread_create(&id, NULL, threadhandler, (void *)nsock);
        if(resul!=0){
            perror("erro ao criar thread\n");
            return 1;
        }
        pthread_detach(id);
        sched_yield();
    }
}
    