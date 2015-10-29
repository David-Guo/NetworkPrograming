#include "inet.h"


#define MAXLINE 512
void str_cli(FILE *fp, int sockfd) {
    int n;
    char sendline[MAXLINE], recvline[MAXLINE];

    while (fgets(sendline, MAXLINE, fp) != NULL) {
        n = strlen(sendline);
        if (writen(sockfd, sendline, n) != n)
            err_dump("str_cli: writen error on socket");

        n = readline(sockfd, recvline, MAXLINE);
        if (n < 0)
            err_dump("str_cli: readline error");
        recvline[n] = 0;
        fputs(recvline, stdout);
    }

    if (ferror(fp))
        err_dump("str_cli: error reading file");
}


int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in serv_addr;

    pname = argv[0];

    /* 
     * Fill in the structure "serv_addr" with the address of
     * the server that we want to connet with
     */

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
    serv_addr.sin_port = htons(SERV_TCP_PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr);  
    /* Open a TCP socket (an Internet stream socket) */

    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0) < 0))
        err_dump("client: can't open stream socket");

    /* Connet to the serve */
    if (connect(sockfd, (struct sockaddr *) & serv_addr, sizeof(serv_addr)) < 0)
        err_dump("client: can't connect to server");

    str_cli(stdin, sockfd);

    close(sockfd);
    exit(0);
}

