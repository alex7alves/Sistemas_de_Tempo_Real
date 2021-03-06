//
//  servidor_multicast.cpp
//  str
//
//  Created by Affonso on 18/05/16.
//  Copyright © 2016 Affonso. All rights reserved.
//

// #include "servidor_multicast.hpp"

// programa servidor_multicast.cpp - Servidor que lê e escreve em um vetor de caracter
// Protocolo UDP

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

int piso(float n);
void Vetorbool(int x, bool *vetor);
#define MULTICAST_ADDR "225.0.0.37"

int main(){

  float media;
  int retorno;
  bool *vetor = new bool[8];
  int server_sockfd;
  size_t server_len;
  socklen_t client_len;
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;

  unsigned short receber = 9708,enviar = 9808;
  struct ip_mreq mreq;  // para endere�o multicast

  unlink("server_socket");  // remocao de socket antigo
  if ( (server_sockfd = socket(AF_INET, SOCK_DGRAM, 0) )  < 0  )  // cria um novo socket
  {
    printf(" Houve erro na ebertura do socket ");
    exit(1);
  }
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(receber);


  server_len = sizeof(server_address);

  if(  bind(server_sockfd, (struct sockaddr *) &server_address, server_len) < 0 )
  {
    perror("Houve error no Bind");
    exit(1);
  }


  // use setsockopt() para requerer inscri��o num grupo multicast
  mreq.imr_multiaddr.s_addr=inet_addr(MULTICAST_ADDR);
  mreq.imr_interface.s_addr=htonl(INADDR_ANY);
  if (setsockopt(server_sockfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0) {
    perror("setsockopt");
    exit(1);
  }
     // tem que ter 2 sockets um pra receber e outro pra enviar
    
    // socket pra  enviar
    
  int sockfd;
  int len;
  struct sockaddr_in address;

  sockfd = socket(AF_INET, SOCK_DGRAM,0);  // criacao do socket

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
  address.sin_port = htons(enviar);

  len = sizeof(address);
  
    float valor[2] = {0, 1};
  
    while(1){

        printf("Servidor esperando ...\n");
        client_len = sizeof(client_address);
        if(recvfrom(server_sockfd, &valor, sizeof(valor),0,
                (struct sockaddr *) &client_address, &client_len) < 0 )
        {
          perror(" erro no RECVFROM( )");
          exit(1);
        }
        printf(" Valor recebido foi = %.2f e %.2f\n", valor[0], valor[1]);
        media = (valor[0]+valor[1])/2;
        media = media*10;
        retorno = piso(media);

        Vetorbool(retorno,vetor);
        cout << " o retorno foi " << retorno << endl;

        sendto(sockfd, vetor,sizeof(vetor),0,(struct sockaddr *) &address, len);
        cout << " o servidor enviou  ";
        for(int i=0;i<8;i++){
            cout << vetor[i] << " ";
        }
        cout << endl;
    }
}


int piso(float n){
    float divisao;
    int val;
    divisao = n/10.0;
    val =n;
    if(val>8){
      val=8;
    }
    return val;
}

void Vetorbool(int x, bool *vetor){

  switch(x){
    case 0:
      vetor[0]= false;
      vetor[1]= false;
      vetor[2]= false;
      vetor[3]= false;
      vetor[4]= false;
      vetor[5]= false;
      vetor[6]= false;
      vetor[7]= false; 
      break;
    case 1:
      vetor[0]= true;
      vetor[1]= false;
      vetor[2]= false;
      vetor[3]= false;
      vetor[4]= false;
      vetor[5]= false;
      vetor[6]= false;
      vetor[7]= false;
      break;  
    case 2:
      vetor[0]= true;
      vetor[1]= true;
      vetor[2]= false;
      vetor[3]= false;
      vetor[4]= false;
      vetor[5]= false;
      vetor[6]= false;
      vetor[7]= false;
      break;  
    case 3:
      vetor[0]= true;
      vetor[1]= true;
      vetor[2]= true;
      vetor[3]= false;
      vetor[4]= false;
      vetor[5]= false;
      vetor[6]= false;
      vetor[7]= false;
      break;  
    case 4:
      vetor[0]= true;
      vetor[1]= true;
      vetor[2]= true;
      vetor[3]= true;
      vetor[4]= false;
      vetor[5]= false;
      vetor[6]= false;
      vetor[7]= false;
      break;  
    case 5:
      vetor[0]= true;
      vetor[1]= true;
      vetor[2]= true;
      vetor[3]= true;
      vetor[4]= true;
      vetor[5]= false;
      vetor[6]= false;
      vetor[7]= false;
      break;  
    case 6:
      vetor[0]= true;
      vetor[1]= true;
      vetor[2]= true;
      vetor[3]= true;
      vetor[4]= true;
      vetor[5]= true;
      vetor[6]= false;
      vetor[7]= false;
      break;  
    case 7:
      vetor[0]= true;
      vetor[1]= true;
      vetor[2]= true;
      vetor[3]= true;
      vetor[4]= true;
      vetor[5]= true;
      vetor[6]= true;
      vetor[7]= false;
      break;    
    case 8:
      vetor[0]=true ;
      vetor[0]= true;
      vetor[1]= true;
      vetor[2]= true;
      vetor[3]= true;
      vetor[4]= true;
      vetor[5]= true;
      vetor[6]= true;
      vetor[7]= true;
      break;  
                            
  }

}