#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char** argv) {
  int server_socket, client_socket, bytes, v6only,buffer_server=0,buffer_cliente=0,TAM_BUFFER=0;
  char client_ip[150];
  unsigned int i, size;
  FILE* arq;
  /* New struct ! */
  struct sockaddr_in6 server_addr, client_addr;

  /* Checking the arguments */
  if(argc != 3) {
    printf("\n[TCP Server] Argument error.\n\n");
    exit(1);
  }
  buffer_server = atoi(argv[2]);
  char buffer_temp[buffer_server];
  /* Creating TCP socket */
  server_socket = socket(AF_INET6, SOCK_STREAM, 0);  // AF_INET6 --> IPv6, SOCK_STREAM --> TCP
  if (server_socket == -1) {
    printf("\n[TCP Server] Socket error.\n\n");
    exit(1);
  }

  /* Socket now working with IPv4 and IPv6 */
  v6only = 0;
  if (setsockopt(server_socket, IPPROTO_IPV6, IPV6_V6ONLY, &v6only, sizeof(v6only)) != 0) {
    printf("\n[TCP Server] Socket IPv6/IPv4 error.\n\n");
    exit(1);
  }

  /* Configure settings in address struct */
  memset(&server_addr, 0, sizeof(server_addr));   // Cleaning the entire struct

  server_addr.sin6_family = AF_INET6;           // IPv6
  server_addr.sin6_addr = in6addr_any;          // Any IP address for the server
  server_addr.sin6_port = htons(atoi(argv[1])); // Gets the port number by the first argument


  /* Bind socket with address struct */
  if(bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    printf("\n[TCP Server] Bind Error\n\n");
    exit(1);
  }

  /* Initialize size variable to be used later on */
  size = sizeof(struct sockaddr_in6);

  listen(server_socket, 5); // Transform the socket in an TCP listener, waiting for TCP connections.

  //while(1) {
    // If a new client try to connect, ANOTHER socket is created just for it (client_socket).
    printf("[TCP Server] Waiting for clients...\n\n");

    client_socket = accept(server_socket, NULL, NULL); // Blocks the execution waiting por clients
    if (client_socket < 0) { // Test the new socket.
      perror("[TCP Server] Connection Error.\n");
      exit(1);
    }

    // Getting information about the client:
    getpeername(client_socket, (struct sockaddr *) &client_addr, &size);
    if(inet_ntop(AF_INET6, &client_addr.sin6_addr, client_ip, sizeof(client_ip)))
    {
      printf("[TCP Server] Client [%s:%d] Connected!\n\n", client_ip, ntohs(client_addr.sin6_port));
    }
    //-------------------------------------------------------------------//
  
    //abre arq
    //recebe nome arquivo
    //memset(&buffer_temp, 0, sizeof(buffer_temp));
    char byte=0;
    char nome_temp[1000];
    int cont =0;
    int tam_nome_arquivo=0;
    do{
       recv(client_socket,&byte, sizeof(byte), 0);
       nome_temp[cont] = byte;
       cont++;
        printf("\n\n[TCP server]: byte: %c\n",byte);

    }while(byte != '\0');
    
    tam_nome_arquivo = strlen(nome_temp)+1;
    char nome_arquivo[tam_nome_arquivo];
    strcpy(nome_arquivo,nome_temp);
   
    printf("\n\n[TCP server]: Nome Arquivo recebido: %s\n",nome_arquivo);

   arq = fopen(nome_arquivo,"rb");
       if(arq == NULL){
            printf("erro ao abrir arquivo de saida");
            exit(1);
       } 
      printf("\n\n[TCP server]: Arquivo aberto: %s\n",nome_arquivo);    //recebe tamanho buffer do cliente
  
    char buffer[buffer_server];
    
  //  printf("[TCP Server] Client [%s:%d] Arquivo aberto:\"%s\"\n", client_ip, ntohs(client_addr.s  in6_port), buffer);
    
    while(fread(buffer,sizeof(byte),buffer_server,arq)) 
    { 
               
      printf("\n\n[TCP server]: Entrei: %s\n",buffer);
      printf("[TCP Server] Response: %s\n\n", buffer);
      send(client_socket, buffer, sizeof(buffer), 0);
      printf("[TCP Server] Tamanho Buffer: %d\n\n", strlen(buffer));
      memset(&buffer,0, sizeof(buffer));
    }
    fclose(arq);

    printf("[TCP Server] Client [%s:%d] Connection Closed.\n\n", client_ip, ntohs(client_addr.sin6_port));
    close(client_socket); // Releasing the socket for the client
  //}
  //close(server_socket); // Releasing the socket of the server
  return 0;
}
