#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
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
 
    for (i = 0, str_len = 0; dest[str_len] != '\0'; str_len++){
    	char c = dest[str_len];
        if(isupper(c))
            dest[i++] = c;
    }
 	str_len = i;
    dest[str_len] = '\0';
    key_len = strlen(key);
    for(i = 0; i < str_len; i++){
        if(!isupper(dest[i]))
            continue;
        dest[i] = 'A' + (is_encode ? dest[i] - 'A' + key[i % key_len] - 'A' : dest[i] - key[i % key_len] + 26) % 26;
    }
 
    return dest;
}

void func(int sockfd)
{
	char buff[MAX];
	int n;
	const char *cod, *dec;
    char key[] = "VIGENERECIPHER";
	for (;;) {
		bzero(buff, sizeof(buff));
		printf("\nEnter the message : ");
		n = 0;
		while ((buff[n++] = getchar()) != '\n');
		cod = encipher(buff, key, 1);
		int i=0;
		while(cod[i]!='\0'){
			buff[i]=cod[i];
			i++;
    	}
		write(sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
		printf("\nUser2 : %s", buff);
		dec = encipher(buff, key, 0);
		i=0;
		while(dec[i]!='\0'){
			buff[i]=dec[i];
			i++;
    	}
    	printf("Decrypted Text : %s", buff);
		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("User2 has left...\n");
			break;
		}
	}
}

int main()
{
	int sockfd, connfd;
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
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
		!= 0) {
		printf("connection with the user failed...\n");
		exit(0);
	}
	else
		printf("You are now connected with User2..\n");

	func(sockfd);

	close(sockfd);
}
