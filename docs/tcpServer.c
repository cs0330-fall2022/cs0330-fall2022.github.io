#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

void serve(int);

int main(int argc, char *argv[ ]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: port\n");
		exit(1);
	}
    
    int lsocket;
    struct addrinfo tcp_hints;
    struct addrinfo *result;

    memset(&tcp_hints, 0, sizeof(tcp_hints));
    tcp_hints.ai_family = AF_INET;
    tcp_hints.ai_socktype = SOCK_STREAM;
    tcp_hints.ai_flags = AI_PASSIVE;

    int err;
    if ((err = getaddrinfo(NULL, argv[1], &tcp_hints, &result)) != 0) {
        fprintf(stderr,"%s\n", gai_strerror(err));
        exit(1);
    }

    struct addrinfo *r;
    for (r = result; r != NULL; r = r->ai_next) {
        if ((lsocket =
                 socket(r->ai_family, r->ai_socktype, r->ai_protocol)) < 0) {
            continue;
        }
        if (bind(lsocket, r->ai_addr, r->ai_addrlen) >= 0) {
            break;
        }
        close(lsocket);
    }
    
    if (r == NULL) {
        fprintf(stderr, "Could not find local interface %s\n");
        exit(1);
    }
    freeaddrinfo(result);

	if (listen(lsocket, 5) < 0) {
		perror("listen");
		exit(1);
	}

	while (1) {
		int csock;
		struct sockaddr client_addr;
		int client_len = sizeof(client_addr);

		csock = accept(lsocket, &client_addr, &client_len);
        if (csock == -1) {
            perror("accept");
            exit(1);
        }
        char host_name[256];
        char serv_name[256];
        int err;
        if ((err = getnameinfo(&client_addr, client_len, host_name, sizeof(host_name), serv_name, sizeof(serv_name), 0))) {
            fprintf(stderr, "%s/n", gai_strerror(err));
            exit(1);
        }
        printf("received connection from %s port %s\n", host_name, serv_name);

        switch (fork( )) {
			case -1:
				perror("fork");
				exit(1);
			case 0:
				serve(csock);
				exit(0);
			default:	
				close(csock);
				break;
		}
	}
	return 0;
}

void serve(int fd) {
	char buf[1024];
	int count;

	while ((count = read(fd, buf, 1024)) > 0) {
		write(1, buf, count);
	}
	if (count == -1) {
		perror("read");
		exit(1);
	}
	printf("connection terminated\n");
}
