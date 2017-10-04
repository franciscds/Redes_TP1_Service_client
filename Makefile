tcp_example: tcp_client.c tcp_server.c
	gcc -o tcp_client tcp_client.c -Wall -Wextra -pedantic
	gcc -o tcp_server tcp_server.c -Wall -Wextra -pedantic
clean:
	rm tcp_client *.o *~
	rm tcp_server *.o *~
	
