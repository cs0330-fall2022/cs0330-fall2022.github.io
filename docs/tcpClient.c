#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int s;
	int sock;
	struct addrinfo hints;
	struct addrinfo *result;
	struct addrinfo *rp;

	char buf[1024];
	if (argc != 3) {
		fprintf(stderr, "Usage: tcpClient host port\n");
		exit(1);
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if ((s=getaddrinfo(argv[1], argv[2], &hints, &result)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(1);
	}

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		if ((sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) < 0) {
			continue;
		}
		if (connect(sock, rp->ai_addr, rp->ai_addrlen) >= 0) {
			break;
		}
		close(sock);
	}

	if (rp == NULL) {
		fprintf(stderr, "Could not connect to %s\n", argv[1]);
		exit(1);
	}
	freeaddrinfo(result);

	while(fgets(buf, 1024, stdin) != 0) {
		if (write(sock, buf, strlen(buf)) < 0) {
			perror("write");
			exit(1);
		}
	}
	return 0;
}
