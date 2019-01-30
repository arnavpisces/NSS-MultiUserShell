//Arnav Kumar : 2016017
//OS Assignment 0

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
//#include <conio.h>

//All internal commands
void pwd();
void echo(char**,int);
void echoplus(char**,int);
void history();
void exitcom();
void cd(char**);
void cdDesktop();


int main() {
    printf("###~~~Rudimentary C Shell (AK BASH), Proceed with caution~~~###\n");
    int historycounter=1;
    char *lspath=realpath("./ls",NULL);
    char *datepath=realpath("./date",NULL);
    char *catpath=realpath("./cat",NULL);
    char *rmpath=realpath("./rm",NULL);
    char *mkdirpath=realpath("./mkdir",NULL);
    FILE *history,*histcount;
    history=fopen("history.txt","a");
    histcount=fopen("histcount.txt","r");
    fscanf(histcount,"%d",&historycounter);
    fclose(histcount);
//    histcount=fopen("histcount.txt","w");
    while(1) {
        printf(">>>");
        char inputcom[1000];
        char first[100]; //first is the command that i am calling
        char *commands[100]={0}; //this is basically an array of words (pointer of array)
//        printf("%s",commands[0]);
//        char *forExec[10];
        int i = 0;
        char flag = 'y';
        fgets(inputcom, 1000, stdin);
        fprintf(history,"%d %s",historycounter++,inputcom);
        char *tokenPtr = strtok(inputcom, " \n");
        while (tokenPtr != NULL) {

//            printf("%s\n", tokenPtr);
            if (flag == 'y') {
                strcpy(first,tokenPtr);
//                printf("%s\n",first);
                flag = 'n';
            } else {
//                commands[i]=(char*)malloc(100);
                commands[i++] = tokenPtr;
//                printf("%s",commands[i-1]);
//                exit(0);
            }
//            forExec[i]=tokenPtr;
            tokenPtr = strtok(NULL, " \n");
        }
        commands[i]=NULL;
//        printf("%s",commands[i]);
//        exit(0);
//        printf("%s %s", first, commands[0]);
        //here I am start calling the functions using what is in first
//        printf("~~~HEREGOES~~~\n");
        if (!strcmp(first,"pwd")) {
//            printf("asdfsdF");

            pwd();
            printf("\n");
            continue;
        }
            //here i have to fix the problem when i give only one argument so command[1] doesnt exist
        else if(!strcmp(first,"echo")){
//            printf("asdfsdF\n");
            if(!strcmp(commands[0],"-n")||!strcmp(commands[0],"-E")){
                echoplus(commands,i);
            }
            else {
//                printf("SADfasdf\n");
                echo(commands, i);
            }
            continue;
        }
        else if(!strcmp(first,"exit")){
//          printf("asdfsdF\n");
            histcount=fopen("histcount.txt","w");
            fprintf(histcount,"%d",historycounter);
            fclose(history);
            printf("\n");
            exitcom();
        }
        else if(!strcmp(first,"cd")){
            if(commands[0]==NULL || !strcmp(commands[0]," ")) {
                cdDesktop();
            }
            else{
                cd(commands);
            }
            printf("\n");
        }

        else if(!strcmp(first,"history")){
            fclose(history);
            if(commands[0]!=NULL && !strcmp(commands[0],"-c")){
                history=fopen("history.txt","w");
                historycounter=1;
            }
            history=fopen("history.txt","r");
            char histline[1000];
            while(fgets(histline, sizeof(histline),history)){
                printf("%s",histline);
            }
            fclose(history);
            history=fopen("history.txt","a");
            printf("\n");
        }
        else if(!strcmp(first,"ls") || (!strcmp(first,"cat") || (!strcmp(first,"date")) || (!strcmp(first,"rm")) || (!strcmp(first,"mkdir")))){
            pid_t pid=fork();
            if(pid==-1){
                fprintf(stderr,"Can't Fork process: %s\n",strerror(errno));
                return 1;
            }
            else if(pid>0){
                int status;
                waitpid(pid,&status,0);
//                printf("%d",status);
            }
            else{
                if(!strcmp(first,"ls")){
                    char pd[1024];
//                    char *args[]={"./ls",NULL};
                    if(commands[0]==NULL){
                        commands[0]=getcwd(pd, sizeof(pd));
                    }
                    else{
                        if(commands[0][0]=='-'){
                            if(commands[1]==NULL){
                                commands[1]=getcwd(pd, sizeof(pd));
                            }
                            else{
                                if(commands[1][0]=='-'){
                                    if(commands[2]==NULL){
                                        commands[2]=getcwd(pd, sizeof(pd));
                                    }
                                }
                            }
                        }
                    }
                    if(execv(lspath,commands)==-1){
                        printf("ERROR\n");
                        printf("%d",errno);
                    }
                    printf("\n");
                }
                else if(!strcmp(first,"date")){
                    if(execv(datepath,commands)==-1){
                        printf("ERROR\n");
                    }
                    printf("\n");
                }
                else if(!strcmp(first,"cat")){
                    if(execv(catpath,commands)==-1){
                        printf("ERROR\n");
                    }
                    printf("\n");
                }
                else if(!strcmp(first,"rm")){
                    if(execv(rmpath,commands)==-1){
                        printf("ERROR\n");
                    }
                    printf("\n");
                }
                else if(!strcmp(first,"mkdir")){
                    if(execv(mkdirpath,commands)==-1){
                        printf(("ERROR\n"));
                    }
                    printf("\n");
                }
            }
        }
        else if(!strcmp(first,"clear")){
            system("clear");
        }
    }
    return 0;
}

void pwd(){
    char pd[1024];
    if(getcwd(pd,sizeof(pd))!=NULL)
        printf("%s\n",pd);
    else
        perror("getcwd() error");
}

void echo(char *commands[100],int i){
    int k=0;
    while(k++<i) {
        printf("%s ", *commands++);
    }
    printf("\n");
//    *commands=NULL;
}
void echoplus(char *commands[100],int i){
    if(commands[0][1]=='n'){
        int k=0;
        while(k++<i) {
            printf("%s ", *commands++);
        }
    }

}

void exitcom(){
    exit(0);
}

void cdDesktop(){
    chdir("D:\\Desktop");
    pwd();
}

void cd(char *commands[100]){
//    printf("%s",commands[0]);
    if(chdir(commands[0])==-1){
        printf("cd: %s : No such file or directory",commands[0]);
    }
    else{
        pwd();
    }
//    chdir(commands[0]);
}


void history(){

}