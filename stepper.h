/*
this file essentially acts as a handler for connections
it takes the role of a standard cgi script, however it is
much more efficient because it is compiled with the actual
program.
this program still needs to recv() data from sock
then the data needs to be processed and delivered related to sock
this just handles processing and delivering data.

START LINE
HEADERS

BODY

REQUESTS:
GET / HTTP/1.0
method target HTTPversion

RESPONSES:
HTTP/1.0 200 OK
version code string
*/
#include <string.h> //strlen() strcpy()

#define ERROR -1
#define RESPONSE 0
#define GET 1
#define PUT 2
#define POST 3
#define DELETE 4

int
stepper(int sockfd)
{
    ssize_t
    byrec;

    char //setting the buffers to "\0" effectively zeroes out the buffers
    bybuf[1000000] = "\0", target[1024] = "\0";

    int_fast32_t
    method, target_start, target_end, j;
    
    do
    { //makes sure that we recv() the whole message.
        byrec = recv(sockfd, bybuf, sizeof(bybuf) - 1, 0);
    }
    while(byrec > 0);

    //printf("%s\n", bybuf);
    
    //find the http message method
    if(bybuf[0] == 'H' && bybuf[1] == 'T')
    {
        method = RESPONSE; //no idea how to handle responses yet
        printf("The Browser Sent an HTTP response\n");
        return -1;
    }
    else if(bybuf[0] == 'G' && bybuf[1] == 'E')
    {
        method = GET;
    }
    else if(bybuf[0] == 'P' && bybuf[1] == 'U')
    {
        method = PUT;
    }
    else if(bybuf[0] == 'P' && bybuf[1] == 'O')
    {
        method = POST;
    }
    else if(bybuf[0] == 'D' && bybuf[1] == 'E')
    {
        method = DELETE;
    }
    else
    {
        method = ERROR;
        printf("Unknown HTTP Method Type\n");
        return -1;
    }
    
    //find the http message target
    //find the start of the http message target
    for(int_fast32_t i = 0; i < strlen(bybuf); i++)
    {
        if(bybuf[i - 1] == ' ' && bybuf[i] == '/')
        {
            target_start = i;
            //printf("target_start: bybuf[%ld] == '%c'\n", target_start, bybuf[target_start]); //used in testing
            break;
        }
    }
    
    //find the end of the http message target
    for(int_fast32_t i = 0; i < strlen(bybuf); i++)
    {
        if(bybuf[i - 1] == ' ' && bybuf[i] == 'H' && bybuf[i + 1] == 'T')
        {
            target_end = i;
            //printf("target_end: bybuf[%ld] == '%c'\n", target_end, bybuf[target_end]); //used in testing
            break;
        }
    }
    
    j = 0;
    for(int_fast32_t i = target_start + 1; i < target_end - 1; i++)
    {
        target[j] = bybuf[i];
        //printf("target[%ld] == '%c'\n", j, target[j]); //used in testing
        j++;    
    }
    
    //http://localhost:1024/0123456789ABCDEF //for use in testing
    //printf("strlen(target) == %ld | target[] == \"%s\"\n", strlen(target), target);
    //for use in testing
    char mestr[10] = "\0";
    switch(method)
    {
        case GET:
            strcpy(mestr, "GET");
            break;
        
        case PUT:
            strcpy(mestr, "PUT");
            break;
            
        case POST:
            strcpy(mestr, "POST");
            break;
            
        case DELETE:
            strcpy(mestr, "DELETE");
            break;
            
    }
    
    printf("%s /%s\n", mestr, target);
    printf("%s\n", bybuf);
    
    return 0;
}









































