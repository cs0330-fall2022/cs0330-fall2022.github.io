#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <string.h>

int communicate(int, struct addrinfo *);

int main(int argc, char *argv[]) {
	int s;
	int sock;
	struct addrinfo hints;
	struct addrinfo *result;
	struct addrinfo *rp;

	if (argc != 3) {
		fprintf(stderr, "Usage: client host port\n");
		exit(1);
	}

	// Step 1: find the internet address of the server
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	if ((s=getaddrinfo(argv[1], argv[2], &hints, &result)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(1);
	}

	// Step 2:set up socket for UDP
	for (rp = result; rp != NULL; rp - rp->ai_next) {
		if ((sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) >= 0) {
			break;
		}
	}
	if (rp == NULL) {
		fprintf(stderr, "Could not communicate with %s\n", argv[1]);
		exit(1);
	}
	freeaddrinfo(result);
    
    // Step 3: communicate with server
    communicate(sock, rp);

	return 0;
}

int communicate(int fd, struct addrinfo *rp) {
	while (1) {
		char buf[1024];
		int msg_size;

		if (fgets(buf, 1024, stdin) == 0)
			break;

		/* send data to server */
		if (sendto(fd, buf, strlen(buf), 0,
			rp->ai_addr,
			rp->ai_addrlen) < 0) {
			perror("sendto");
			return -1;
		}

		/* receive response from server */
		if ((msg_size = recvfrom(fd, buf, 1024, 0, 0, 0)) < 0) {
			perror("recvfrom");
			exit(1);
		}
		buf[msg_size] = 0;
		printf("Server says: %s\n", buf);
	}
	return 0;
}
