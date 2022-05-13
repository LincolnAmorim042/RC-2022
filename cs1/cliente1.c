//compilar: gcc clienteServidor -o clienteServidor
//Vou usar o TCP

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define closesocket close
#define PROTOPORT 5193 //número de porta padrão - poderia ser qualquer outro valor

extern int errno;
char localhost[]="localhost"; //nesta versão este cliente roda na própria máquina

int main(int argc, char** argv){
	struct hostent *ptrh; //ponteiro para tabela de entrada do host
	struct protoent *ptrp; //ponteiro para tabela de entrada do protocolo
	struct sockaddr_in sad; //estrutura para manter o endereço do servidor
	int sd; //descritor de socket
	int port; //número da porta do protocolo
	char *host; //ponteiro para o nome do host
	int n; //número de caracteres lidos
	char buffer[1000]; //buffer para os dados recebidos do servidor
	
	memset((char*)&sad, 0, sizeof(sad));//limpa a estrutua sockaddr
	sad.sin_family = AF_INET; //configura a família de protocolos internet
	
	if(argc > 0) port = atoi(argv[2]);
	else port = PROTOPORT;

	if(port > 0) sad.sin_port = htons((u_short)port);
	else {fprintf(stderr, "número de portas inválido %s\n", argv[2]); exit(1);}

	if(argc>1) host = argv[1];
	else host = localhost;
	
	ptrh = gethostbyname(host);
	if( ((char*)ptrh) == NULL ){
		fprintf(stderr, "host inválido: %s\n", host);
		exit(1);	
	}
	
	memcpy(&sad.sin_addr, ptrh->h_addr, ptrh->h_length);

	if( ((int)(ptrp = getprotobyname("TCP"))) == 0){
		fprintf(stderr, "Erro ao mapear 'TCP' para o número do protocolo\n ");
		exit(1);
	}		
	
	sd = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);
	if(sd<0){
		fprintf(stderr, "erro ao criar o socket\n");
		exit(1);
	}

	if(connect(sd, (struct sockaddr *)&sad, sizeof(sad)) < 0){
		fprintf(stderr, "Falha na conexão\n");
		exit(1);
	}

	n = recv(sd, buffer, sizeof(buffer), 0);
	while(n>0){
		buffer[n] = '\0';
		fprintf(stderr, "Cliente: %s", buffer);
		n = recv(sd, buffer, sizeof(buffer), 0);
	}	

	closesocket(sd);
	exit(0);




return 0;
}

