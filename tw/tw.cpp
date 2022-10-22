// tw client

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SERVERPORT "8303"

void help()
{
	puts("usage: tw hostname data");
}

int main(int argc, char *argv[])
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;

	char aHost[128];
	strncpy(aHost, "localhost", sizeof(aHost));
	char aData[512];
	memset(aData, 1, sizeof(aData));

	if(argc > 1)
	{
		if(!strncmp(argv[1], "--help", 6))
		{
			help();
			exit(0);
		}
		strncpy(aHost, argv[1], sizeof(aHost));
		if(argc > 2)
			strncpy(aData, argv[2], sizeof(aData));
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET6; // set to AF_INET to use IPv4
	hints.ai_socktype = SOCK_DGRAM;

	if((rv = getaddrinfo(aHost, SERVERPORT, &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p = p->ai_next)
	{
		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if(sockfd == -1)
		{
			perror("tw: socket");
			continue;
		}

		break;
	}

	if(p == NULL)
	{
		fprintf(stderr, "tw: failed to create socket\n");
		return 2;
	}

	numbytes = sendto(sockfd, aData, strlen(aData), 0, p->ai_addr, p->ai_addrlen);
	if(numbytes == -1)
	{
		perror("tw: sendto");
		exit(1);
	}

	freeaddrinfo(servinfo);

	printf("tw: sent %d bytes to %s\n", numbytes, aHost);
	close(sockfd);
	return 0;
}

