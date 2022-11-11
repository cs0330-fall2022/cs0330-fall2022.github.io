#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char *argv[ ]) {
 	if (argc != 2) {
		fprintf(stderr, "Usage: server port\n");
		exit(1);
	}
    int udp_socket;
    struct addrinfo udp_hints;
    struct addrinfo *result;

    memset(&udp_hints, 0, sizeof(udp_hints));
    udp_hints.ai_family = AF_INET;
    udp_hints.ai_socktype = SOCK_DGRAM;
    udp_hints.ai_flags = AI_PASSIVE;

    int err;
    if ((err = getaddrinfo(NULL, argv[1], &udp_hints, &result)) != 0) {
        fprintf(stderr,"%s\n", gai_strerror(err));
        exit(1);
    }

    struct addrinfo *r;
    for (r = result; r != NULL; r = r->ai_next) {
        if ((udp_socket =
                 socket(r->ai_family, r->ai_socktype, r->ai_protocol)) < 0) {
            continue;
        }
        if (bind(udp_socket, r->ai_addr, r->ai_addrlen) >= 0) {
            break;
        }
        close(udp_socket);
    }
    if (r == NULL) {
        fprintf(stderr, "Could not bind to %s\n", argv[1]);
        exit(1);
    }
    freeaddrinfo(result);

	while (1) {
		char buf[1024];
		struct sockaddr from_addr;
		int from_len = sizeof(struct sockaddr);
		int msg_size;

		/* receive message from client */
		if ((msg_size = recvfrom(udp_socket, buf, 1024, 0,
			(struct sockaddr *)&from_addr, &from_len)) < 0) {
			perror("recvfrom");
			exit(1);
		}
		buf[msg_size] = 0;
        char host_name[256];
        char serv_name[256];
        if ((err = getnameinfo((struct sockaddr *)&from_addr, from_len, host_name, sizeof(host_name), serv_name, sizeof(serv_name), 0))) {
            fprintf(stderr, "%s/n", gai_strerror(err));
            exit(1);
        }
		printf("message from %s port %s:\n%s\n", host_name, serv_name, buf);

		/* respond to client */
		if (sendto(udp_socket, "thank you", 9, 0,
				(const struct sockaddr *)&from_addr,
				from_len) < 0) {
			perror("sendto");
			exit(1);
		}
	}
}
