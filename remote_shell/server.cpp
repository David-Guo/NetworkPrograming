#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <strings.h>
#include <stdlib.h>

#include "mysh.h"

#define SERV_TCP_POTR 127.0.0.1
#define SERV_TCP_PORT 12344
using namespace std;
char log[] = "log.txt";

void serve(int sockfd){

    int tempStdinFd = dup(0);
    int tempStdoutFd = dup(1);
    int tempStderrFd = dup(2);
    dup2(sockfd, 0);
    dup2(sockfd, 1);
    dup2(sockfd, 2);
	
	cout << "****************************************" << endl;
	cout << "** Welcome to the information server. **" << endl;
	cout << "****************************************" << endl;
    //printf("sockfd id: %d", sockfd);

	cout << "% " << flush;
	
	Mysh shell;
	
	shell.setEnv("setenv PATH bin:.");
	int logfd = creat(log, 0666);
    dup2(logfd, sockfd);
	for ( ; ; ) {	
		string tempStr;
		std::getline(std::cin, tempStr);

        size_t tempPos = tempStr.find_first_of("\r", 0);
        tempStr[tempPos]= '\n';
		//tempStr += "\n";
		size_t sizelog = write(logfd, tempStr.c_str(), tempStr.size());
        if (sizelog != tempStr.size()) cout << "write log error" << endl;
        shell.parseCommand(tempStr);
        if (shell.isExit) {
            close(sockfd);
            break;
        }
		cout << "% " << flush;
	}
    close(logfd);
	
    dup2(tempStdinFd, 0);
    dup2(tempStdoutFd, 1);
    dup2(tempStderrFd, 2);
    close(tempStdinFd);
    close(tempStdoutFd);
    close(tempStderrFd);
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd,  childpid;
    struct sockaddr_in cli_addr, serv_addr;
    socklen_t clilen;
    
    /* Open a TCP socker (an Internet stream socket). */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "Server: can't open stream socket" << endl;
    }

    /* Bind our local address so that the client can send to us. */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_TCP_PORT);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        cerr << "Server : can't bind local address\n" << endl;
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
            cerr << "server: accept error\n" << endl;

        if ( (childpid = fork()) < 0)
            cerr << "server: fork error\n" << endl;
        else if (childpid == 0) {
            close(sockfd);
            //signal(SIGCHLD, SIG_IGN);
            serve(newsockfd);
            exit(0);
        }

        close(newsockfd);
    }
    return 0;
} 

