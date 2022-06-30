/*SUMMARY
This is the server part of a webserver.
This is meant to be a semi efficient way for client connections
to be handled. This code just does normal socket stuff,
then calls a function where the actual dynamic aspect of
web development takes place. This handles connections,
then hands them off to a function to be acted upon.

struct sockaddr_in configures a socket
socket() makes a blank socket file descriptor
bind() applies the configuration to the socket
listen() sets the socket to listening mode
accept() accepts connections, then moves the client connection to a new socket.
*NOTE* accept() does not affect the listening socket.
recv() lets the client know that the socket is ready to recieve data
send() sends bytes to the client
sendfile() sends bytes directly from a file to a client
shutdown() makes a socket stop sending or recieving data or both
close() closes a file descriptor, e.g. a socket
*/

#include <sys/socket.h> //socket() bind() listen() accept() recv() send() shutdown() socklen_t ssize_t size_t
#include <sys/sendfile.h> //sendfile()
#include <unistd.h> //close()
#include <arpa/inet.h> //htons() htonl()
#include <stdio.h> //printf()
#include <stdint.h> //fixed width integers
#include <pthread.h> //contains thread functions
#include <stdlib.h> //contains malloc()

#include "stepper.h" //this contains stepper() which is the compiled dynamic content scripts

void*
handler(void* arg)
{
    int
    sockfd;
    
    sockfd = *(int*) arg;
    free(arg);
    
    stepper(sockfd); //this is where tcp messages can be recv()'d and processed for clients
    close(sockfd);
}

int
main(void)
{
    struct sockaddr_in
    lisockaddr, newsockaddr;
    
    uint16_t
    portno;
    
    int
    lisockfd, t, newsockfd;

    socklen_t
    addrlen;
        
    pthread_t
    thread_id;
    
    portno = 1024;
    
    lisockaddr.sin_family = AF_INET;
    lisockaddr.sin_port = htons(portno);
    lisockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        
    lisockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(lisockfd < 0)
    {
        return -1;
    }
    
    addrlen = sizeof(struct sockaddr);
    
    t = bind(lisockfd, (struct sockaddr*) &lisockaddr, addrlen);

    if(t < 0)
    {
        return -2;
    }
    
    t = listen(lisockfd, 4096);
    
    if(t < 0)
    {
        return -3;
    }
    
    while(1)
    {        
        newsockfd = accept(lisockfd, (struct sockaddr*) &newsockaddr, &addrlen);
        
        if(newsockfd < 0)
        {
            continue;
        }
        
        int* a = malloc(sizeof(int));
        *a = newsockfd;
        pthread_create(&thread_id, NULL, &handler, a);
    }
    return 0;
}




































