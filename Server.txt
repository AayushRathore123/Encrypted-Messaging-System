#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#include <ctype.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void upper_case(char src[]) {
    int i=0;
    while (src[i]!= '\0') {
        if(islower(src[i]))
            src[i]= src[i]-32;
        i++;
    }
}
 
char* encipher(const char *src, char *key, int is_encode) {
    int i, key_len, str_len;
    char *dest;
 
    dest = strdup(src);
    upper_case(dest);
    upper_case(key);
 
    for (i = 0, str_len = 0; dest[str_len] != '\0'; str_len++)
        if (isupper(dest[str_len]))
            dest[i++] = dest[str_len];
 
    dest[str_len = i] = '\0'; 
 
    key_len = strlen(key);
    for (i = 0; i < str_len; i++) {
        if (!isupper(dest[i]))
            continue;
        dest[i] = 'A' + (is_encode ? dest[i] - 'A' + key[i % key_len] - 'A' : dest[i] - key[i % key_len] + 26) % 26;
    }
 
    return dest;
}


void func(int connfd)
{
	char buff[MAX];
	int n;
	const char *cod, *dec;
    char key[] = "VIGENERECIPHER";
	for (;;) {
		bzero(buff, MAX);
		read(connfd, buff, sizeof(buff));
		printf("\nUser1: %s", buff);
		dec = encipher(buff, key, 0);
		int i=0;
		while(dec[i]!='\0'){
			buff[i]=dec[i];
			i++;
    	}
    	printf("Decrypted Text : %s", buff);
		bzero(buff, MAX);
		n = 0;
		printf("\nEnter the message : ");
		while ((buff[n++] = getchar()) != '\n');
		cod = encipher(buff, key, 1);
		i=0;
		while(cod[i]!='\0'){
			buff[i]=cod[i];
			i++;
    	}
		write(connfd, buff, sizeof(buff));
		if (strncmp("exit", buff, 4) == 0) {
			printf("User1 has left...\n");
			break;
		}
	}
}

int main()
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("Reload the app...\n");
		exit(0);
	}
	else
		;
	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("Reload the app...\n");
		exit(0);
	}
	else
		;

	if ((listen(sockfd, 5)) != 0) {
		printf("Could not reach the user...\n");
		exit(0);
	}
	else
		printf("Waiting for other user..\n");
	len = sizeof(cli);

	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("Could not connect with user..\n");
		exit(0);
	}
	else
		printf("You are now connected with User1...\n");

	func(connfd);

	close(sockfd);
}
