#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv){
    int mysocket, socketclient;
    struct sockaddr_in addr;
    mysocket = socket(AF_INET, SOCK_STREAM, 0);
    if(mysocket==-1){
        printf("Erro ao criar socket\n");
        return 1;
    }
    addr.sin_family=AF_INET;
    addr.sin_port=htons(1234);
    addr.sin_addr.s_addr=INADDR_ANY;
    memset(&addr.sin_zero,0,sizeof(addr.sin_zero));

    if(bind(mysocket, (struct sockaddr *)&addr, sizeof(addr))==-1){
        printf("Erro no bind\n");
        return 1;
    }
    if(listen(mysocket,1)==1){
        printf("Erro no listen\n");
        return 1;
    }
    printf("Aguardando conexões....\n");
    socketclient=accept(mysocket,0,0);
    if(socketclient==-1){
        printf("Erro na conexão!\n");
        return 1;
    }
    printf("Pedido de conexão feito!\n");

    int recebidos, enviados, n1, n2, indnum, ind1, ind2;
    char mensagem[256], resposta[256], cnum[2][6], op, mensagemi;

    do{
        printf("Cliente... ");
        recebidos = recv(socketclient,resposta,256,0);
        if(recebidos==-1){
            printf("Erro ao receber\n");
            return 1;
        }
        resposta[recebidos]='\0';
        indnum = 1;
        ind1 = ind2 = 0;
        while(resposta[ind1]!='\0'){
            if(resposta[ind1]!='+' && resposta[ind1]!='-' && resposta[ind1]!='/' && resposta[ind1]!='*'){
                cnum[indnum][ind2]=resposta[ind1];
                ind2++;
                ind1++;
            }else{
                op=resposta[ind1];
                indnum=2;
                cnum[1][ind2]='\0';
                ind2=0;
                ind1++;
            }
        }

        cnum[indnum][ind2]='\0';
        n1=atoi(cnum[1]);
        n2=atoi(cnum[2]);

        switch (op){
        case '+':
            mensagemi=n1+n2;
            break;
        case '-':
            mensagemi=n1-n2;
            break;
        case '/':
            mensagemi=n1/n2;
            break;
        case '*':
            mensagemi=n1*n2;
            break;
        default:
            break;
        }
        sprintf(mensagem, "%d", mensagemi);
        if(send(socketclient,mensagem, strlen(mensagem),0)==-1){
            printf("Erro ao enviar dados\n");
            return 1;
        }
    }while(1);
    close(socketclient);
    close(mysocket);
    return 0;


}