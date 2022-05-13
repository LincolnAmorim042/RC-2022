#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main(int argc, char** argv){
    struct sockaddr_in saddr;
    fd_set readset, testset;
    int listensock, nsock, resultado, nread, x, val;
    char buffer[25];

    listensock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    val = 1;
    resultado = setsockopt(listensock, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    if (resultado<0){
        perror("Servidor");
        return 0;
    }
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(1972);
    saddr.sin_addr.s_addr=INADDR_ANY;

    resultado = bind(listensock, (struct sockaddr *)&saddr, sizeof(saddr));
    if(resultado<0){
        perror("Bind");
        return 0;
    }
    resultado = listen(listensock, 5);
    if(resultado<0){
        perror("Listen");
        return 0;
    }

    FD_ZERO(&readset);
    FD_SET(listensock, &readset);

    while (1){
        testset = readset;
        resultado = select(FD_SETSIZE, &testset, NULL, NULL, NULL);
        if(resultado<0){
            perror("Select");
            return 0;
        }
        for(x=0; x<FD_SETSIZE; x++){
            if(FD_ISSET(x, &testset)){
                if(x==listensock){
                    nsock=accept(listensock, NULL, NULL);
                    FD_SET(nsock,&readset);
                }else{
                    nread = recv(x,buffer,25,0);
                    if(nread<=0){
                        close(x);
                        FD_CLR(x, &readset);
                        printf("Cliente #%i desconectado\n", x);
                    }else{
                        buffer[nread] = '\0';
                        printf("%s\n", buffer);
                        send(x,buffer,nread,0);
                    }
                }
            }
        }
    }
}