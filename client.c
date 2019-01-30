//Arnav Kumar - 2016017
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#define SERVER_PORT 8080

#define TRUE 1
#define FALSE 0

void * reader(int sock){
    int rc;
    char message[5000];
    while(1){
        memset(message,'\0',sizeof(message));
        // printf("Before read\n");
        rc=read(sock,message,sizeof(message));
        // printf("After read\n");
        if(rc==-1){
                perror("read() error");
                close(sock);
                pthread_exit(NULL);
            }
        if(!strcmp(message,"\0")){
                printf("Connection closed by server\n");
                close(sock);
                exit(0);
                pthread_exit(NULL);
            }
        printf("%s",message); //So if i add a newline in printing message then it would print properly but if i dont add new line then the OS doesn't print to the terminal unless a new line is encountered so the make it print to the terminal even before the newline, i added fflush
        fflush( stdout );


        // int rc;
        // message[5000];
        // while(1){
            // printf("Before write\n");
            // fgets(message,sizeof(message),stdin); //*
            // printf("After write\n");

            // // printf("%s ",message);
            // message[strcspn(message, "\n\0")] = 0; //*
            // // printf("%s",message);
            // // rc=write(sock,message,strlen(message));
            // rc=send(sock, message,strlen(message),0);//*
            // if(rc==-1){
            //     perror("write() error");
            //     close(sock);
            //     exit(-1);
            // }
        // }
    }
}

void * writer(int sock){
    // int rc;
    // char message[5000];
    // while(1){
    //     rc=read(sock,message,sizeof(message));
    //     if(rc==-1){
    //             perror("read() error");
    //             close(sock);
    //             pthread_exit(NULL);
    //         }
    //     if(!strcmp(message,"\0")){
    //             printf("Connection closed by server\n");
    //             close(sock);
    //             exit(0);
    //             pthread_exit(NULL);
    //         }
    //     printf("%s",message);
    //     memset(message,'\0',sizeof(message));
        

    // }

    int rc;
    char message[5000];
    while(1){
        // printf("Before write\n");
        fgets(message,sizeof(message),stdin); //*
        // printf("After write\n");
        message[strcspn(message, "\n\0")] = 0; //*
        // rc=write(sock,message,strlen(message));
        rc=send(sock, message,strlen(message),0);
        if(rc==-1){
            perror("write() error");
            close(sock);
            exit(-1);
        }
    }
}

int main(int argc, char const *argv[])
{
    char address[1024];
    strcpy(address,argv[1]);
    char message[5000];
    int sock=-1;
    
    int rc;
    struct sockaddr_in servaddr={0};
    sock=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock<0){
        perror("socket() creation failed");
        exit(-1);
    }
    // printf("YAH YAH YAH\n");

    servaddr.sin_family=AF_INET;
    // inet_pton(AF_INET6, address, &servaddr.sin6_addr);
    servaddr.sin_addr.s_addr=inet_addr(address);
    servaddr.sin_port=htons(SERVER_PORT);
    rc=connect(sock,(struct sockaddr *)&servaddr,sizeof(servaddr));
    if(rc==-1){
        perror("connect() failed");
        close(sock);
        exit(-1);
    }
    pthread_t threads[2];
    int create=-1;

    create=pthread_create(&threads[0],NULL,reader,sock);
    create=pthread_create(&threads[1],NULL,writer,sock);
    pthread_join(threads[0],NULL);
    pthread_join(threads[1],NULL);
    return 0;
}
