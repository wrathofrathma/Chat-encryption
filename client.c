#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <pthread.h>
#include "cryptosockets.h"
#include <libnsqrand.h>
#define SIZE 2048
//Functions
int Send();
void *Recieve(void *sock);
int clearstr();
//Port number
int port=38383;
//Server, just uncomment the one to connect to
char serv[20];
char servelse[20]="localhost";
pthread_t read_thread;
pthread_mutex_t mutex;
int exit_flag = 0;              /* Tells the Receive thread when to quit */


void zap_newline (char *str)
{
     char *p = strchr (str, '\n');
     if (p)
          *p = '\0';
}

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char **argv)
{
    if(argc < 2) {
        printf ("Host not specified, defaulting to %s\n", servelse);
        strcpy(serv, servelse);
    }
    else {
        printf ("Got host %s\n", argv[1]);
        strcpy(&serv,argv[1]);
    }
    nsqseed(time(NULL));
    int sockfd,portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    portno = (port);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server=gethostbyname(serv);
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    //Impliment threading here!!!!!
    pthread_mutex_init(&mutex, NULL);
    pthread_create (&read_thread, NULL, Recieve, (void *) sockfd);
    Send(sockfd);
    pthread_exit (NULL);
}


void *Recieve(void *sock)
{
    char buf[SIZE];
    int test=1;
    int n;
    int sockfd = (int) sock;
    while(test==1)
    {
        if (exit_flag)
	    {
             exit(0);
             break;
        }
        n=read_cryptomsg(sockfd,buf,255);
        if(n<0)
            error("ERROR reading from socket");
        else if (n > 0) {
             buf[n] = '\0';
            printf("%s\n", buf);
        }
    }
    pthread_exit (NULL);
}

int Send(int sockfd)
{
    char buf[SIZE];
    int buffer[SIZE];
    int test=1;
    int n;
    while(test==1)
    {
        fgets(buf,255,stdin);
        zap_newline (buf);
        pthread_mutex_lock(&mutex);
        n=send_cryptomsg (sockfd, buf, 1);
        pthread_mutex_unlock(&mutex);
        if(n<0)
            error("ERROR writing to socket");
    }
    exit_flag = 1;
    return 1;
}

int clearstr(char *array)
{
    int test=0;
    int a=strlen(array);
    while(test<a)
    {
        array[test]=NULL;
    }
    return 0; 
    
}
