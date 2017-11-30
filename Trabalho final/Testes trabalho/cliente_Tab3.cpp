
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

void *thread_enviar(void *arg);
void *thread_receber(void *arg);

#define MULTICAST_ADDR "225.0.0.37"
unsigned short porta = 9707;  // o numero do nosso grupo aqui
float ADCs[2];
bool *display[8];

int main()
{
    
    int result;
    int res,res2;
    pthread_t thread_enviar,thread_receber;
    void *thread_result;

    res = pthread_create(&thread_enviar, NULL, thread_enviar,NULL);
    res2 = pthread_create(&thread_receber, NULL, thread_receber,NULL);
    if (res != 0 || res2 != 0) {
        perror("Alguma thread nao foi criada");
        exit(EXIT_FAILURE);
    }
    
   
    exit(0);
}

void *thread_enviar(void *arg) {
    int sockfd;
    int len;
    struct sockaddr_in address;
    
    sockfd  = socket(AF_INET, SOCK_DGRAM,0);  // criacao do socket
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
    address.sin_port = htons(porta);
    
    len = sizeof(address);
    while(true){
        // pega os valores aqui do adc
        ADCs[0] = 0.1;
        ADCs[1] = 0.2;
        sendto(sockfd, ADCs,sizeof(ADCs),0,(struct sockaddr *) &address, len);
        sleep(1);
    }
    close(sockfd);
}

void *thread_receber(void *arg) {
    int sockfd;
    socklen_t len_recv;
    struct sockaddr_in address;
    
    sockfd  = socket(AF_INET, SOCK_DGRAM,0);  // criacao do socket
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
    address.sin_port = htons(porta);
    
    len = sizeof(address);
    while(true){
        recvfrom(sockfd, display,sizeof(display),0,(struct sockaddr *) &address,&len_recv);
        sleep(1);
    }
    close(sockfd);
}