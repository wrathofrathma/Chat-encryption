#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "cryptosockets.h"
#include <libnsqrand.h>
#include <time.h>
#include "user.c"
#include "parse.c"
#include "commands.c"
//Size of all arrays
#define SIZE 2048
//Functions
int Who();
int openSocket();
int Command();
int changeUser();
int clearStr();
int Start();
int Send(int socket,char message[SIZE], struct userInfo *use_info);
int Recieve(char *out, int socket);
//port number 
int port=38383;

pthread_t connection_thread;

struct sockaddr_in serv_addr, cli_addr;

void zap_newline (char *str)
{
     char *p = strchr (str, '\n');
     if (p)
          *p = '\0';
}

int Start(int socket, struct usertrack *new)
{
    int test=1;
    char USERNAME[SIZE];
    send_cryptomsg (socket,"What would you like your username to be?: ", 0);
    printf("Before recieve \n");
    read_cryptomsg (socket, USERNAME, SIZE);
    zap_newline (USERNAME);
    printf("After recieve \n");
    strcpy (new->username, USERNAME);
    while(test==1)
    {
        printf("Before finding item \n");
        int pos=findItem(begin,USERNAME);
        printf("POS %d \n",pos);
        if(pos<0)
        {
            printf("Segfault pos 1? \n");
            append(&begin,socket,USERNAME);
            test=0;
            break;
        }
        else
        {
            Send(socket,"Username already taken! \n",new);
            Recieve(USERNAME,socket);
        }         
    }
    return 0;
}

void *handle_client (void *arg)
{
     struct userInfo new;
     new.fat_person=NULL;
     char buf[2048];
     int socket = arg;
     int exit_status;
     int test=1;
     Start(socket, &new);
     while (test>=0) {
          Recieve (buf, socket);
          if(buf[0]=='/')
               test=Command (buf, &new);
          else
               Send (socket, buf, &new);
          puts (buf);
     }
     printf ("User exited!\n");
     exit_status = delete_item (&begin, new.username);
     if (exit_status)
          printf ("Deleted user %s\n", new.username);
     else
          printf ("User %s not found - oh my arachnophobic celestial deity!\n", new.username);
     pthread_exit (NULL);
}
     

void error(char *msg)
{
    perror(msg);
    exit(1);
}
int connections(int sockfd)
{
    pthread_t active_thread;
    int newsockfd;
    int clilen;
    int test=1;
    while(test==1)
    {
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,
    (struct sockaddr *) &cli_addr,
        &clilen);
    if (newsockfd < 0)
        error("ERROR on accept");
    pthread_create (&active_thread, NULL, handle_client, (void *) newsockfd);
    }
}
int main()
{
    nsqseed(time(NULL));
    printf("Starting....\n");
    int sockfd, newsockfd, portno, clilen,n;
    char buffer[SIZE];
    int optval = 1;
    printf("Opening Socket...\n");
    sockfd=openSocket();
    setsockopt (sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof (optval));
    printf("Socket open...\n");
    portno = (port);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    printf("Listening for users \n");
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
        sizeof(serv_addr)) < 0) 
            error("ERROR on binding");
    pthread_create (&connection_thread, NULL, connections, (void *) sockfd);
    pthread_join (connection_thread, NULL);
    return 0;
}

int Send(int socket,char message[SIZE], struct userInfo *use_info)
{
    char buf[SIZE];
    char buff[SIZE];
    strcpy(buf,use_info->username);
    strcat(buf,": ");
    strcat(buf,message);
    int n;
    if(use_info->fat_person==NULL)
    {
        n=send_cryptomsg (socket,buf, 1);
        if(n<0)
            error("Error writing to socket \n");
    }
    else
    {
        n=send_cryptomsg (use_info->fat_person,message, 1);
        if(n<0)
            error("Error writing to socket \n");
    }
}

int Recieve(char *out, int socket)
{
    int n;
    char buf[2048];
    n=read_cryptomsg(socket,buf,SIZE-1);
    if(n<0)
         error("Error reading from socket \n");
    zap_newline (buf);
    strcpy (out, buf);
}


int openSocket()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    return sockfd;
}

