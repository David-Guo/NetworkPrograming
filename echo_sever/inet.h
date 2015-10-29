/* 
 * Definitions for TCP client/server programs
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>

#define SERV_TCP_PORT 12345
#define SERV_HOST_ADDR "127.0.0.1"

char *pname;

int readline(int fd, char *ptr, int maxlen) {
    int n, rc;
    char c;

    for (n = 1; n < maxlen; n++) {
        if ((rc = read(fd, &c, 1)) == 1) {
            *ptr++ = c;
            if (c == '\n')
                break;
         }
        else if (rc == 0) {
            if (n == 1)
                return 0;           /* EOF, no data read */
            else
                break;              /* EOF, some data was read */
        }
        else 
            return -1;              /* error */
    }

    *ptr = 0;
    return(n);
}

void err_dump(const char *string) {
    printf("%s\n", string) ;  
    perror( "System Error Message:" ) ;  
    return ;   
}

int readn(int fd, char *ptr, int nbytes) {
    int nleft, nread;

    nleft = nbytes;
    while (nleft > 0) {
        nread = read(fd, ptr, nleft);
        if (nread < 0)
            return(nread);
        else if (nread == 0)
            break;

        nleft -= nread;
        ptr   += nread;
    }
    return(nbytes - nleft);
}


int writen(int fd, char *ptr, int nbytes) {
    int nleft, nwritten;

    nleft = nbytes;
    while (nleft > 0) {
        nwritten = write(fd, ptr, nleft);
        if (nwritten <= 0)
            return(nwritten);

        nleft -= nwritten;
        ptr   += nwritten;

    }
    return(nbytes - nleft);
}
