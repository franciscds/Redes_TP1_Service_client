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
 

  struct addrinfo hints, *list, *item;

  /* Checking the arguments */
  if(argc != 5) {
    printf("\n[TCP Client] Argument error.\n\n");
    exit(1);
  }
  buffer_cliente = atoi(argv[4]);
  char buffer[buffer_cliente];

// ------------------------------------------------------------
// ------------------------------------------------------------
  // Neste ponto, usaremos a getaddrinfo para "traduzir"
  // o que vier pelo argumento para um IPv4 ou IPv6.
  // o argv[1] pode ser:

  // um IPv4 puro, por exemplo: 127.0.0.1 (própria máquina), 201.12.43.12
  // um IPv6 puro, por exemplo: ::1 (pŕopria máquina), 2001:db8:1::2
  // um host, por exemplo: teste.com

  memset(&hints, 0, sizeof hints); // Enche de zeros na nova estrutura
  hints.ai_family = AF_UNSPEC;     // aceitar IPv4 ou IPv6
  hints.ai_socktype = SOCK_STREAM; // Apenas TCP

  if((rv = getaddrinfo(argv[1], argv[2], &hints, &list)) != 0) { // Chama a função passando os dois argumentos
    printf("[TCP Multi Client] getaddrinfo error: %s\n", gai_strerror(rv));
    exit(1);
  }

  // Essa função retorna uma lista de IPs. Por isso, é feito
  // este laço de repetição tentando abrir o socket com cada item
  // retornado. Se o socket for criado com sucesso e a conexão for
  // bem sucedida, sai do laço.

  // IMPORTANTE: Lembre-se que esta lista precisa ser "liberada"
  // de volta, pois está usando memória RAM. Eu estou fazendo
  // isso chamando a função "freeaddrinfo" lá no final do código.

// ------------------------------------------------------------
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
  bytes = strlen(argv[3]);
  write(client_socket, argv[3], bytes); // Envia a mensagem nome_arquivo  
  write(client_socket, argv[4], bytes);//envia tamanho buffer cliente
 
//cria arquivo de saida
  FILE* arq_saida = fopen("saida",w);

  while(bytes >1) {
    memset(&buffer, 0, sizeof(buffer));
    buffer[--bytes] = '\0';
    bytes = recv(client_socket, buffer, sizeof(buffer), 0);  // Esperando por uma resposta do servidor
    fwrite(buffer,strlen(buffer),arq_saida);
  }
  fclose(arq);
  printf("\n[TCP Client] Fechando conexão...\n");
  close(client_socket); // Releasing the socket.
  freeaddrinfo(list); // liberando a memória!!
  return 0;
}
