
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <iostream>
#include "BlackGPIO/BlackGPIO.h"
#include "ADC/Adc.h"

using namespace BlackLib;
void *thread_enviar(void *arg);
void *thread_receber(void *arg);
int proximo(unsigned short n);
void display(int z);
#define MULTICAST_ADDR "225.0.0.37"

// Criando aqui pra nao dar problema ao passar pras threads
float ADCs[2];//={0, 1};
bool display[8]={true, true, true, true, false, false, false, false};
// portas digitais pro display 
BlackGPIO a(GPIO_67, output);
BlackGPIO b(GPIO_68, output);
BlackGPIO c(GPIO_66, output);
BlackGPIO d(GPIO_44, output);
BlackGPIO e(GPIO_26, output);
BlackGPIO f(GPIO_69, output);
BlackGPIO g(GPIO_45, output);
//ADCS
ADC adc1(AIN3) ;
ADC adc2(AIN5);
 BlackGPIO botao(GPIO_65, input);
int bot;

int main(){
    
    int result;
    int res,res2;
    pthread_t thread_envia,thread_recebe;
    void *thread_result;

    res = pthread_create(&thread_envia, NULL, thread_enviar,NULL);
    res2 = pthread_create(&thread_recebe, NULL, thread_receber,NULL);
    if (res != 0 || res2 != 0) {
        perror("Alguma thread nao foi criada");
        exit(EXIT_FAILURE);
    }
    while(true){
         std::cout<<" valor do adc1 eh "<< ADCs[0]<<" e adc2 eh " << ADCs[1] << endl;
    }
   
    exit(0);
}

void *thread_enviar(void *arg) {
    int sockfd;
    int len;
    struct sockaddr_in address;
    unsigned short porta = 9707;  // o numero do nosso grupo aqui
    sockfd  = socket(AF_INET, SOCK_DGRAM, 0);  // criacao do socket
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
    address.sin_port = htons(porta);
    
    len = sizeof(address);
    while(true){
        // pega os valores aqui do adc
        ADCs[0] = adc1.getFloatValue();
        ADCs[1] = adc2.getFloatValue();
        sendto(sockfd, ADCs, sizeof(ADCs), 0, (struct sockaddr *) &address, len);
        sleep(1);
    }
    close(sockfd);
}

void *thread_receber(void *arg) {
    int sockfd;
    u_int yes=1;
    socklen_t len_recv;
    struct sockaddr_in address;
    struct ip_mreq mreq;
    int prodisplay;
    // a porta vira do botao 
    unsigned short porta = 9801;//9801;  // o numero do OUTRO GRUPO AQUI

    unlink("server_socket");  // remocao de socket antigo
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0) )  < 0  ){ //Cria um novo socket
        printf(" Houve erro na ebertura do socket ");
        exit(1);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))<0){ //Cria um novo socket
        printf(" Houve erro ao reusar o ADDR ");
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(porta);
    len_recv = sizeof(address);

    if(bind(sockfd, (struct sockaddr *) &address, len_recv) < 0){
        perror("Houve error no Bind com o recebedor");
        exit(1);
    }

    // use setsockopt() para requerer inscri��o num grupo multicast
    mreq.imr_multiaddr.s_addr=inet_addr(MULTICAST_ADDR);
    mreq.imr_interface.s_addr=htonl(INADDR_ANY);
    if (setsockopt(sockfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0) {
        perror("setsockopt");
        exit(1);
    }

    while(true){
        bot=botao.getNumericValue();
        if(bot){
            porta = (unsigned short)proximo(porta);
        }
        std::cout << " Escultando da porta "<< porta << endl;
        recvfrom(sockfd, &display, sizeof(display), 0, (struct sockaddr *) &address, &len_recv);
        std::cout << " O cliente recebeu " ;
        for(int i=0;i<8;i++){ 
            std::cout << display[i] << " " ;
        }
        std::cout << endl;
        prodisplay=0;
        for(int i=0;i<7;i++){ // descarta o ultimo - led do ponto
            prodisplay= prodisplay + (int)display[i] ;
        }
        display(prodisplay);
        std::cout <<" o valor pro display eh " << prodisplay << endl;
        sleep(1);
    }
    
    close(sockfd);

}
int proximo(unsigned short n){
    int aux = (int)n;
    if(aux>9814){
        aux=9801;
    }else{
        aux++;
    }
    return aux;
}


void display(int z){
  switch(z){
    case 0:
      a.setValue(low);
      b.setValue(low);
      c.setValue(low);
      d.setValue(low);
      e.setValue(low);
      f.setValue(low);
      g.setValue(high);
      break;
    case 1:
      a.setValue(high);
      b.setValue(low);
      c.setValue(low);
      d.setValue(high);
      e.setValue(high);
      f.setValue(high);
      g.setValue(high);
      break;  
    case 2:
      a.setValue(low);
      b.setValue(low);
      c.setValue(high);
      d.setValue(low);
      e.setValue(low);
      f.setValue(high);
      g.setValue(low);
      break;  
    case 3:
      a.setValue(low);
      b.setValue(low);
      c.setValue(low);
      d.setValue(low);
      e.setValue(high);
      f.setValue(high);
      g.setValue(low);
      break;  
    case 4:
      a.setValue(high);
      b.setValue(low);
      c.setValue(low);
      d.setValue(high);
      e.setValue(high);
      f.setValue(low);
      g.setValue(low);
      break;  
    case 5:
      a.setValue(low);
      b.setValue(high);
      c.setValue(low);
      d.setValue(low);
      e.setValue(high);
      f.setValue(low);
      g.setValue(low);
      break;  
    case 6:
      a.setValue(low);
      b.setValue(high);
      c.setValue(low);
      d.setValue(low);
      e.setValue(low);
      f.setValue(low);
      g.setValue(low);
      break;  
    case 7:
      a.setValue(low);
      b.setValue(low);
      c.setValue(low);
      d.setValue(high);
      e.setValue(high);
      f.setValue(high);
      g.setValue(high);
      break;  
      case 8:
      a.setValue(low);
      b.setValue(low);
      c.setValue(low);
      d.setValue(low);
      e.setValue(low);
      f.setValue(low);
      g.setValue(low);
      break;  
       case 9:
      a.setValue(low);
      b.setValue(low);
      c.setValue(low);
      d.setValue(low);
      e.setValue(high);
      f.setValue(low);
      g.setValue(low);
      break;         
  }
}