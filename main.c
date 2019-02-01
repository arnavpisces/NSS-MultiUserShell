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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define SERVER_PORT 8080

#define TRUE 1
#define FALSE 0
#define print(f) printf("\n%s\n",f);

char cmsg[5000];
int clients=0;
char users[4][10];
int sockfds[100];

char loggedin[10];
char relativePath[100];

char directoryAux[1000];


// char* showdir(char *dir,char flag);

//All function prototypes
char* directoryWithPath(char *path, char *currentDir);
void fput(char *dir,int sock);
void fget(char *dir,int sock);
void cd(char *dir, char *path, char *currentDir, char* relativePath, int sock);

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
    char currentDir[500];
    memset(currentDir,0,sizeof(currentDir));
    int i=0;
    int id=++clients;
    int sock=sockaccept;
    
    char msg[5000];
    char shellHeader[200];
    char home[100];
    strcpy(home,"./sandbox/simple_slash/simple_home/");
    strcpy(currentDir,home);

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
    strcat(home,cmsg);
    strcat(home,"/");
    send(sock, msg,strlen(msg),0);

    char justUser[10]; //String for user without null terminator
    int k=0;
    for(k=0;cmsg[k]!=0;k++){ //0 means null '\0'
        justUser[k]=cmsg[k];
    }
    strcat(currentDir,loggedin); //To change the home of the current directory to the user's home
    strcat(currentDir,"/\0"); 

    // snprintf(msg,sizeof(msg),"Welcome User %s\n%s@SHELL:~$",loggedin,loggedin);
    // snprintf(msg,sizeof(msg),"%s@SHELL:~%s$",loggedin,relativePath);
    // send(sock, msg,sizeof(msg),0);
    strcpy(relativePath,"/");
    while(1){
        // printf("loop revertback\n");
        fflush(stdout);
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
        char * commands=malloc(100);
        commands = strtok (cmsg," ");
        int flag1=0;
        char first[100]; //The command that the user will be calling
        char path[100]; memset(path,'\0',sizeof(path));        
        while(commands!=NULL){

            if(flag1==0){ //Getting the 1st word
                strcpy(first,commands);
                // printf("%s\n",first);
                flag1=1;
            }
            else{

                strcpy(path,commands);
                // printf("path %s ",path);
            }
            commands = strtok (NULL, " ");

        }  
        //Now we have the first word in first and the path of file/directory in the variable path
        // printf("the length is %d \n",strlen(path));
        // if(strlen(path)!=0){
        //     strcat(currentDir,path);
        //     printf("%s ",currentDir);
        // }

        if(!(strcmp(first,"ls"))){
            // printf("hello");
            showdir(directoryWithPath(path,currentDir),sock); //For ls
        }
        else if(!(strcmp(first,"fput"))){
            // printf("%s ",directoryWithPath(path));
            fput(directoryWithPath(path,currentDir),sock); //For fput
            // showdir(directoryWithPath(path),sock); 
        }
        else if(!(strcmp(first,"fget"))){
            fget(directoryWithPath(path,currentDir),sock); //For fget
        }
        else if(!(strcmp(first,"create_dir"))){
            create_dir(directoryWithPath(path,currentDir),sock); //For create_dir
        }
        else if(!(strcmp(first,"cd"))){
            // printf("%s ",directoryWithPath(path));
            // char *auxillary=malloc(200);
            // auxillary=directoryWithPath(path,currentDir);
            // printf("\nmiddle %s %s ",auxillary,auxillary);
            // cd(auxillary,path,sock); //For cd
            if(strlen(path)==0){
                strcpy(currentDir,home);
            }
            else{
                cd(directoryWithPath(&path,currentDir),path,currentDir,relativePath,sock); //For cd
            }
        }
        else if(!(strcmp(first,"exit"))){ //Exiting the remote shell
            close(sock);
            pthread_exit(NULL);
            // exit(0);
        }
        else{
            snprintf(msg,sizeof(msg),"Unknown Command\n");
            send(sock, msg,sizeof(msg),0);
        }
        

    





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

char* directoryWithPath(char *path, char *currentDir){ //Returns a path concatenated of currentDir with path variable argument
    // char aux[1000];
    // printf("%s ",path);
    memset(directoryAux,0,sizeof(directoryAux)); 
    strcpy(directoryAux,currentDir);
    // printf("%s ",aux);
    strcat(directoryAux,path);
    // printf("%s ",aux);
    char blah[]="\0";
    strcat(directoryAux,blah);
    // int i=0;
    // for(i=0;i<100;i++){
    //     printf("%c",aux[i]);
    // }
    // printf("aux %s ",directoryAux);
    return directoryAux;
}

void showdir(char *dir,int sock) {
        // printf("ls %s %s ",dir,dir);
        struct dirent *de;
        DIR *dr = opendir(dir);
        char content[1000];
        if (dr == NULL) {
            // printf("Could not open given directory\n");
            snprintf(content,sizeof(content),"Could not open given directory\n");
            send(sock, content ,strlen(content),0);
            return;
        }
        /*Permission Handler*/
        char permission[10];
        char file[50];
        memset(content,'\0',sizeof(content));
        memset(file,'\0',sizeof(file));
        // if (flag == 'y') { //won't show hidden
        while ((de = readdir(dr)) != NULL){
            if (de->d_name[0] != '.'){
                // printf("%s\n", de->d_name);
                strcat(file,de->d_name);
                getFilePermission(permission,de->d_name,dir);
                // print(permission);
                int i;
                for(i=0;;i++){
                    if(permission[i]==' '){
                        permission[i]='-';
                        break;
                    }
                }
            }
        }
            // printf("%s", content);                
            snprintf(content,sizeof(content),"%s %s",file,permission);
            // strcat(content,"\n");
            send(sock, content ,strlen(content),0);
            // printf("blahblah");                

        closedir(dr);

    }


void fput(char *dir,int sock){

    //Check bounded path function
    //Check whether file accessible by the user logged in
    //
    printf("FPUT %s",dir);
    char content[50];
    char data[1000];
    // char filepath[200];
    // strcpy(filepath,directoryWithPath(dir));

    FILE *fptr, *fileptr;

    fptr = fopen(dir, "r"); //Checking whether the file exists
    // int flag=0;
    char owner[10];
    char group[10];
    if(fptr == NULL)
    {
        // flag=1;
        char owner[10];
        char group[10];
        snprintf(content,sizeof(content),"Enter Owner-\n");
        send(sock, content ,strlen(content),0);
        // memset(data,'\0',sizeof(data)); //Dont uncomment this, for some reason fopen is not working if I use this memset
        recv(sock,data,1000,0);
        strcpy(owner,data);

        snprintf(content,sizeof(content),"Enter Group-\n");
        send(sock, content ,strlen(content),0);
        // memset(data,'\0',sizeof(data));
        recv(sock,data,1000,0);
        strcpy(group,data);
        // system("chmod 777 yolo.txt");
        // printf("Error!");
        // exit(1);
        // return;
    }
    if(fptr!=NULL){
        fclose(fptr);
        // printf("blahblah");
    }

    //Check file permissions HERE-----
    
    fptr = fopen(dir, "a");
    printf("%s sadf", fptr);
    if(fptr == NULL)
    {
        snprintf(content,sizeof(content),"Error Opening File, Kindly Check Path/Permission\n");
        perror("fopen");

        send(sock, content ,strlen(content),0);
        // printf("Error!");
        // exit(1);
        return;
    }
    memset(content,'\0',sizeof(content));
    strcpy(content,">");
    send(sock, content ,strlen(content),0);
    
    
    memset(data,'\0',sizeof(data));
    recv(sock,data,1000,0);
    strcat(data,"\n");
    fprintf(fptr,"%s", data); //File printf
    fclose(fptr);  

}


void fget(char *dir,int sock){

    //Check bounded path function
    //Check whether file accessible by the user logged in
    //
    char content[50];
    char data[1000];
    // char filepath[200];
    // strcpy(filepath,directoryWithPath(dir));

    FILE *fptr;

    fptr = fopen(dir, "r"); //Checking whether the file exists
    
    if(fptr == NULL)
    {
        snprintf(content,sizeof(content),"Error Opening File, Kindly Check Path/Permission\n");
        send(sock, content ,strlen(content),0);

        return;
    }
    char ch;
    ch = fgetc(fptr);
    char aux[2];
    memset(data,'\0',sizeof(data));
    while (ch != EOF)
    {
        // printf ("%c", ch);
        snprintf(aux,sizeof(aux),"%c",ch);
        // strcpy(aux,ch);
        strcat(data,aux);
        ch = fgetc(fptr);
    }
    // snprintf(content,sizeof(content),"Error Opening File, Kindly Check Path/Permission\n");
    send(sock, data ,strlen(data),0);
    fclose(fptr);

}

void create_dir(char *dir, int sock){

    int check;
    check=mkdir(dir, 0700);
    if(!check){
        printf("Directory created\n");
    }
    else{
        char aux[]="Directory already exists\n";
        send(sock, aux ,strlen(aux),0);
        perror("wodafoq : ");
        printf("Unable to create directory\n");
        return 0;
    }

}

void cd(char *dir, char *path, char *currentDir, char* relativePath, int sock){

    // printf("\nINSIDE %s",dir);

    char content[100];

    // struct stat s;
    // int err = stat(dir, &s);
    // if(err == -1) {
    //     if(ENOENT == errno) {
    //         /* does not exist */
    //         printf("folder no exists\n"); 
    //         fflush(stdout); 

    //         snprintf(content,sizeof(content),"Folder does not exist\n");
    //         send(sock, content ,strlen(content),0);
    //         // printf("bitch does not exist ");
    //         // perror("stat");
    //         // printf("\n");

    //     } else {
    //         printf("folder exists\n");  
    //             snprintf(content,sizeof(content),"Folder does exist\n");
    //             send(sock, content ,strlen(content),0);
    //             strcat(currentDir,path);
    //             printf("\n");
    //             printf("%s ",currentDir);
    //         // perror("stat");
    //         // exit(1);
    //     }
    // }

        DIR* dire = opendir(dir);
        printf("\n%s\n",dir);
        //Check here for permissions or check once on directoryWithPath
        if (dire)
        {
            /* Directory exists. */
            closedir(dire);
            //Take the path one by one and keep adding on to the currentDir and keep on checking at everypoint whether within bounds or not
            //If .. then remove one and check permissions

            //Henceforth it is assuming that the necessary permissions are present.
            char * subPath=malloc(100);
            subPath = strtok (path,"/");
            char auxPath[100]; memset(auxPath,'\0',sizeof(auxPath));        
            while(subPath!=NULL){


                memset(auxPath,'\0',sizeof(auxPath));  
                strcpy(auxPath,subPath); //Copy one by one into auxPath the directories present in subPath
                printf("%s\n",auxPath);
                if(!strcmp(auxPath,"..")){
                    // printf("%s\n",currentDir);
                    removeLastDir(currentDir);
                    // strcat(currentDir,"/");

                    // printf("%s\n",currentDir);
                    // printf("its a dot\n");
                }
                else{
                    strcat(currentDir,auxPath);
                    strcat(currentDir,"/");
                }
                subPath = strtok (NULL, "/");

            }
            // strcat(currentDir ,path);
            // strcat(relativePath,path);
        }
        else if (ENOENT == errno)
        {
            /* Directory does not exist. */
            snprintf(content,sizeof(content),"No such file or directory\n");
            send(sock, content ,strlen(content),0);
        }

    // char blah[]="/";
    // strcat(dir,blah);
    // printf("%s",dir);
    // if (0 != access(dir, F_OK)) {
    //     if (ENOENT == errno) {
    //  // does not exist
    //     printf("bitch ");

    //     }
    // }
    // printf("cd : %s ",dir);
    // int y=0;
    // for(y=0;y<strlen(dir);y++){
    //     printf("%c",dir[yS]);
    // }
    // dir[strcspn(dir, "\n")] = 0;
    // printf("\nINSIDE %s",dir);
    // printf("\n");/
    
}

char output[200]; //Global Variable to prevent pointer garbage bullshit
void removeLastDir(char *currentDir){
    FILE *fp;
    // printf("%s",currentDir);
    // fflush(stdout);
    char content[100];
    // char output[1035];
    memset(output,'\0',sizeof(output));
    // printf("%s",output);
    fflush(stdout);
    /* Open the command for reading. */
    char pythonArgs[200];
    snprintf(pythonArgs,sizeof(pythonArgs),"python3 dir.py %s",currentDir);
    // printf("%s",pythonArgs);
    fp = popen(pythonArgs, "r");
    if (fp == NULL) {
        printf("Failed to run p command\n" );
        // exit(1);
        // snprintf(content,sizeof(content),"Not able to open the file\n");
        // send(sock, content ,strlen(content),0);
        return;
    }
    
    /* Read the output a line at a time - output it. */
    while (fgets(output, sizeof(output)-1, fp) != NULL) {
        // printf("\noutput from file blah %s blah again", output);
    }

    /* close */
    pclose(fp);
    strcpy(currentDir,output);
    // return output;
}

void getFilePermission(char *permission, char *name, char* currentDir){
    //Opening the file
    FILE *fptr;
    // print(name);
    // printf("\n%s\n",relativePath);
    // printf("\n%s\n",currentDir)
    char filename[50];
    char auxPerm[10];
    memset(filename,0,sizeof(filename));
    // strcat(".",filename);
    // strcat(filename,name);
    snprintf(filename, sizeof(filename),"%s.%s",currentDir,name);
    // filename[strlen(filename)]=0;
    // printf(filename);

    fptr=fopen(filename,"r");
    if(fptr==NULL){
        printf("Some error in reading file permission\n");
        perror("bata bhai ");
        return;
    }
    fgets(auxPerm,sizeof(auxPerm),fptr);
    strcpy(permission,auxPerm);
    // print(auxPerm);
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