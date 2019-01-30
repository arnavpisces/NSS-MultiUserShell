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

#include <dirent.h>

#define SERVER_PORT 8080

#define TRUE 1
#define FALSE 0

char cmsg[5000];
int clients=0;
char users[4][10];
int sockfds[100];
char currentDir[200];
char loggedin[10];
char relativePath[100];


// char* showdir(char *dir,char flag);
char* directoryWithPath(char *path);

void * sockThread(int sockaccept){
//     pid_t pid=fork();
//     printf("fork number %d ", pid);
//     if(pid==-1){
//         fprintf(stderr,"Can't Fork process: %s\n",strerror(errno));
//         return 1;
//     }
//     else if(pid>0){
//         int status;
//         waitpid(pid,&status,0);
// //                printf("%d",status);
//     }
//     else{ //Child has pid = 0
//     }
                   
    int i=0;
    int id=++clients;
    int sock=sockaccept;
    
    char msg[5000];
    char shellHeader[200];
    
    strcpy(currentDir,"./sandbox/simple_slash/simple_home/");

    // snprintf(msg,sizeof(msg),"%s ",currentDir);
    // send(sock, msg,strlen(msg),0);
    // printf("%s ",currentDir);
    //Initialize the existing users into the users array;
    strcpy(users[0],"u1");
    strcpy(users[1],"u2");
    strcpy(users[2],"u3");
    strcpy(users[3],"u4");
    // printf("%d ",strlen(users[0]));

    int incorrect=0, out=0;
    // snprintf(msg,sizeof(msg),"Welcome Client Number %d\n",id);
    while(incorrect!=3){ //The loop will keep asking for the username until it is correctly written
        snprintf(msg,sizeof(msg),"Enter SHELL username: ");
        send(sock, msg,strlen(msg),0);
        memset(cmsg,'\0',sizeof(cmsg));
        recv(sock,cmsg,5000,0);
        // strtok(cmsg, "\n");
        
        // printf("%d ",strlen(cmsg));
        // printf("%d ",strlen(users[0]));
        int u, flag=0;
        for(u=0;u<4;u++){
            if(!strcmp(users[u],cmsg)){
                flag=1;
                out=1;
                break;
            }
        }   
        if(out==1){
            break;
        }
        if(flag==0){ //The username entered by the client does not exist
            snprintf(msg,sizeof(msg),"The username entered is incorrect\n");
            send(sock, msg,strlen(msg),0);
            incorrect++;
        }
    }
    if(out==0){
        snprintf(msg,sizeof(msg),"Goodbye\n");
        send(sock, msg,strlen(msg),0);
        close(sock);
        pthread_exit(NULL);
        // exit(0);
    }

    //IF THE LOGIN IS SUCCESSFUL, WELCOME MESSAGE
    strcpy(loggedin, cmsg); //Copying the logged in user to the loggedin variable
    // memset(cmsg,'\0',sizeof(cmsg));
    snprintf(msg,sizeof(msg),"Welcome User %s\n",cmsg);
    send(sock, msg,strlen(msg),0);

    char justUser[10]; //String for user without null terminator
    int k=0;
    for(k=0;cmsg[k]!=0;k++){ //0 means null '\0'
        justUser[k]=cmsg[k];
    }
    strcat(currentDir,loggedin); //To change the home of the current directory to the user's home
    strcat(currentDir,"/"); 

    // snprintf(msg,sizeof(msg),"Welcome User %s\n%s@SHELL:~$",loggedin,loggedin);
    // snprintf(msg,sizeof(msg),"%s@SHELL:~%s$",loggedin,relativePath);
    // send(sock, msg,sizeof(msg),0);
    strcpy(relativePath,"/");
    while(1){
        printf("loop revertback\n");
        snprintf(msg,sizeof(msg),"%s@SHELL:~%s$",loggedin,relativePath);
        send(sock, msg,sizeof(msg),0);
        memset(cmsg,'\0',sizeof(cmsg));
        recv(sock,cmsg,5000,0);
        
        
        // if(strlen(cmsg)==0){
        //     // printf("CLIENT %d LEFT THE SERVER\n",id);
        //     close(sock);
        //     sockfds[sock]=-1;
        //     pthread_exit(NULL);
        // }
        // char format[5000];
        // snprintf(format,sizeof(format),"%d: %s",id,cmsg);
        // for(i=0;i<100;i++){
        //     if(sockfds[i]!=-1){                
        //         send(sockfds[i],format,strlen(format),0);
        //     }
        // }
        
        // // printf("%s ----  YAH YAH YAH %d\n",cmsg,strlen(cmsg));
        // if(!strcmp(cmsg,"CLOSE\n\0")){
        //     char closemsg[1024];
        //     strcpy(closemsg,"server says - CLOSING CONNECTION WITH YOU\n");
        //     send(sock,closemsg,strlen(closemsg),0);
        //     printf("CLOSING CONNECTION WITH Client number %d\n",id);
        //     close(sock);
        //     sockfds[sock]=-1;
        //     pthread_exit(NULL);
        // }

        /*The shell functionality starts from here*/
        // printf("%s\n",loggedin);

        char *commands = strtok (cmsg," ");
        int flag1=0;
        char first[10]; //The command that the user will be calling
        char path[100]; memset(path,'\0',sizeof(path));        
        while(commands!=NULL){

            if(flag1==0){ //Getting the 1st word
                strcpy(first,commands);
                // printf("%s\n",first);
                flag1=1;
            }
            else{
                strcpy(path,commands);
                // printf("%s\n",path);
            }
            commands = strtok (NULL, " ");

        }  
        //Now we have the first word in first and the path of file/directory in the variable path
        printf("the length is %d \n",strlen(path));
        // if(strlen(path)!=0){
        //     strcat(currentDir,path);
        //     printf("%s ",currentDir);
        // }
        showdir(directoryWithPath(path),sock);
        

    





        /*The shell functionality ends here*/



    // snprintf(msg,sizeof(msg),"%s@SHELL:~%s$",loggedin,relativePath);
    // send(sock, msg,sizeof(msg),0);
    }
    pthread_exit(NULL);
}


// void * writer(int nouse){
//     char msg[1000];
//     int i=0;
//     fgets(msg, sizeof(msg),stdin);
//     // printf("sdhfkjasdhfjksahdjfk");
//     char* shut;
//     shut="SERVER is going to shutdown now\n";
//     if(!strcmp(msg,"SHUTDOWN\n\0")){
//         for(i=0;i<100;i++){
//             if(sockfds[i]!=-1){
//                 send(sockfds[i],shut,strlen(shut),0);                
//             }
//         }
//         // delay(300);
//         exit(-1);
//     }
// }

char* directoryWithPath(char *path){ //Returns a path concatenated of currentDir with path variable provided
    char aux[1000];
    strcpy(aux,currentDir);
    strcat(aux,path);
    return aux;
}

void showdir(char *dir,int sock) {
        struct dirent *de;
        DIR *dr = opendir(dir);
        char content[1000];
        if (dr == NULL) {
            // printf("Could not open given directory\n");
            snprintf(content,sizeof(content),"Could not open given directory\n");
            send(sock, content ,strlen(content),0);
            return;
        }
        
        memset(content,'\0',sizeof(content));
        // if (flag == 'y') { //won't show hidden
        while ((de = readdir(dr)) != NULL){
            if (de->d_name[0] != '.'){
                // printf("%s\n", de->d_name);
                strcat(content,de->d_name);
                strcat(content,"\n");
            }
        }
            // printf("%s", content);                
            // snprintf(content,sizeof(content),"%s",content);
            send(sock, content ,strlen(content),0);
            // printf("blahblah");                

        closedir(dr);
        // return (content);
        // }
    //     else{ //will show hidden
    //         while ((de = readdir(dr)) != NULL)
    // //            if (de->d_name[0] != '.')
    //                 printf("%s\n", de->d_name);
    //         closedir(dr);

    //     }

    }










int main(int argc, char const *argv[])
{
    int count=0;
    int i=0;
    for (i=0;i<100;i++){
        sockfds[i]=-1;
    }
    struct sockaddr_in addr={0}, cli;
    int listensd=-1;
    int rc=0, on=1;
    listensd=socket(AF_INET, SOCK_STREAM,0);
    if(listensd<0){
        perror("Socket() creation failed");
        exit(-1);
    }

    // rc=setsockopt(listensd,SOL_SOCKET,SO_REUSEADDR,(char *)&on, sizeof(on));
    // if(rc<0){
    //     perror("setsockopt() failed");
    //     close(listensd);
    //     exit(-1);
    // }
    // memset(&addr,0, sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=INADDR_ANY;
    // memcpy(&addr.sin_addr, INADDR_ANY, sizeof(INADDR_ANY));
    addr.sin_port=htons(SERVER_PORT); //converts little-endian to big-endian format, host to network short
    rc=bind(listensd,(struct sockaddr *)&addr, sizeof(addr));
    
    if(rc<0){
        perror("bind() failed");
        close(listensd);
        exit(-1);
    }

    rc=listen(listensd,64);
    printf("Listening\n");
    if(rc<0){
		perror("listen() failed");
		close(listensd);
		exit(-1);
	}
    int sockaccept=0;
    pthread_t threads[100];
    // pthread_t write;
    int tnum=0;
    int blah;
    // blah=pthread_create(&write,NULL,writer,0);
    while(1){
        int clisize=sizeof(cli);
        sockaccept=accept(listensd,(struct sockaddr *)&cli, &clisize);
        
        if(sockaccept==-1){
            perror("Failed To Accept Connection\n");
            close(listensd);
            exit(-1);
        }
        sockfds[sockaccept]=sockaccept;
        int create=-1;
        create=pthread_create(&threads[tnum++],NULL, sockThread, sockaccept);
        if(create==-1){
            perror("Failed To Create Thread\n");
            close(listensd);
            exit(-1);
        }
        // tnum++;
    }
        // pthread_join(threads[tnum++],NULL);

    return 0;
}
