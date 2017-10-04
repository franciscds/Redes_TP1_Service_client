tcp_example: clienteFTP.c servidorFTP.c
	gcc -o clienteFTP clienteFTP.c -Wall -Wextra -pedantic
	gcc -o servidorFTP servidorFTP.c -Wall -Wextra -pedantic
clean:
	rm clienteFTP *.o *~
	rm servidorFTP *.o *~
