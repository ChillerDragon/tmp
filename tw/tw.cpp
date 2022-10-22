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

//                              size=7 flags=1       <   empty srv token  >  ctrl  <   my token is 1234 >
const unsigned char MSG_TOKEN[] = {0x04, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x05, 0x01, 0x02, 0x03, 0x04};

void dump_sockaddr(const sockaddr_in *addr)
{
	printf("sockaddr_in {\n");
	printf("  sin_family=%d\n", addr->sin_family);
	printf("  sin_port=%d\n", addr->sin_port);
	printf("  sin_addr {\n");
	printf("    s_addr=%d.%d.%d.%d\n",
			addr->sin_addr.s_addr & 0xFF,
			(addr->sin_addr.s_addr >> 8) & 0xFF,
			(addr->sin_addr.s_addr >> 16) & 0xFF,
			(addr->sin_addr.s_addr >> 24) & 0xFF);
	printf("  }\n");
	printf("}\n");
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
	memcpy(aData, MSG_TOKEN, sizeof(aData));
	int DataSize = sizeof(MSG_TOKEN);

	if(argc > 1)
	{
		if(!strncmp(argv[1], "--help", 6))
		{
			help();
			exit(0);
		}
		strncpy(aHost, argv[1], sizeof(aHost));
		if(argc > 2)
		{
			strncpy(aData, argv[2], sizeof(aData));
			DataSize = strlen(aData);
		}
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; // set to AF_INET to use IPv4
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

	dump_sockaddr((sockaddr_in *)p->ai_addr);
	numbytes = sendto(sockfd, aData, DataSize, 0, p->ai_addr, p->ai_addrlen);
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

