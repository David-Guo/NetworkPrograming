/* 
 * Example of server using TCP protoclo.
 */
#include "inet.h"
#define MAXLINE 512

void str_echo(int sockfd) {
    int n;
    char line[MAXLINE];

    for( ; ; ) {
        n = readline(sockfd, line, MAXLINE);
        if (n == 0)
            return;
        else if (n < 0)
            err_dump("str_echo: readline error:");
        
        strcat(line, " from server\n");
        if (writen(sockfd, line, n+14) != n+14)
            err_dump("str_echo: writen error");
    }
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd,  childpid;
    struct sockaddr_in cli_addr, serv_addr;
    socklen_t clilen;
    
    pname = argv[0];

    /* Open a TCP socker (an Internet stream socket). */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_dump("Server: can't open stream socket\n");

    /* Bind our local address so that the client can send to us. */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_TCP_PORT);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        err_dump("Server : can't bind local address\n");
    printf("\t[Info] binding...\n");

    listen(sockfd, 5);
    printf("\t[Info] listening...\n");

    for( ; ; ) {
        /* 
         * Wait for a connection from a client process.
         * This is an example of a concurrent server.
         */
        printf("\t[Info] wait for connection...\n");
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        char *paddr_str = inet_ntoa(cli_addr.sin_addr);
        printf("\t[Info] Receive connection from %s\n",paddr_str);
        if (newsockfd < 0)
            err_dump("server: accept error\n");

        if ( (childpid = fork()) < 0)
            err_dump("server: fork error\n");
        else if (childpid == 0) {
            close(sockfd);
            str_echo(newsockfd);
            exit(0);
        }

        close(newsockfd);
    }

    return 0;
}

