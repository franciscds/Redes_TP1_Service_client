#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

int main(int argc, char** argv) {
  int client_socket, bytes, rv;
  char buffer[128];

  struct addrinfo hints, *list, *item;

  /* Checking the arguments */
  if(argc != 5
) {
    printf("\n[TCP Client] Argument error.\n\n");
    exit(1);
  }

  memset(&hints, 0, sizeof hints); // Enche de zeros na nova estrutura
  hints.ai_family = AF_UNSPEC;     // aceitar IPv4 ou IPv6
  hints.ai_socktype = SOCK_STREAM; // Apenas TCP

  if((rv = getaddrinfo(argv[1], argv[2], &hints, &list)) != 0) { // Chama a função passando os dois argumentos
    printf("[TCP Multi Client] getaddrinfo error: %s\n", gai_strerror(rv));
    exit(1);
  }

   // Para cada item obtido da função....
  for(item = list; item != NULL; item = item->ai_next) {
    // Tenta criar o socket
    if((client_socket = socket(item->ai_family, item->ai_socktype, item->ai_protocol)) == -1) {
      printf("[TCP Multi Client]: socket error\n"); // Não deu certo, tenta criar outro socket com novos valores.
      continue;
    }

    // Se criar o socket, tenta realizar a conexão:
    if(connect(client_socket, item->ai_addr, item->ai_addrlen) == -1) {
      close(client_socket); // Não deu certo a conexão! Fecha o socket criado e tenta de novo.
      printf("[TCP Client]: connect error\n");
      continue;
    }
    break;
  }
  if(item == NULL) exit(1); // Caso todos os itens falhem, será o fim da lista.
// ------------------------------------------------------------
// A partir deste ponto, estamos conectados!
// ------------------------------------------------------------

  printf("[TCP Client] CONNECTED! Envie Alguma coisa.\n");
//envia nome do arquivo
  bytes = strlen(argv[3])+1;
  send(client_socket, argv[3], bytes,0); // Envia a mensagem nome_arquivo  
 // bytes = strlen(argv[4]);
 // send(client_socket, argv[4], bytes,0);//envia tamanho buffer cliente
  
//cria arquivo de saida
  FILE* arq_saida = fopen(argv[3],"wb");

  while(bytes >1) {
    memset(&buffer, 0, sizeof(buffer));
  //  buffer[--bytes] = '\0';
    //printf("\n[TCP Multi Client] Esperando...");
    bytes = recv(client_socket, buffer, sizeof(buffer), 0);  // Esperando por uma resposta do servidor
    conta_bytes_recebidos +=bytes;
    //printf("\n[TCP Client] numero bytes: \"%d\"\n\n", bytes);
    //escreve_arq(arq_saida,buffer);
    fwrite(buffer,sizeof(char),bytes,arq_saida);
  }
  printf("\n[TCP Client] Fechando conexão...\n");
//Fecha Tempo
    
  close(client_socket); // Releasing the socket.
  freeaddrinfo(list); // liberando a memória!!
  return 0;
}
