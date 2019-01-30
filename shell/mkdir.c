#include <stdio.h>
#include <dirent.h>
#include <getopt.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void showdir(char*,char);

int main(int argc, char *argv[]){
    if (argc==0){
        printf("mkdir: missing operand\n");
        return -1;
    }
    if(argc==1){
        int check;
        struct stat st = {0};

        if (stat(argv[0], &st) == -1) {

            check=mkdir(argv[0], 0700);
            if(!check){
//                printf("Directory created\n");
            }
            else{
                printf("Unable to create directory\n");
                return 0;
            }
        }
    }
    else if(argc==2){

        if(argv[0][1]=='v'){
            int check;
            struct stat st = {0};

            if (stat(argv[1], &st) == -1) {

                check=mkdir(argv[1], 0700);
                if(!check){
                    printf("Directory created\n");
                }
                else{
                    printf("Unable to create directory\n");
                    return 0;
                }
            }
        }
        else{
            printf("AK BASH DOESN't Support the given flags\n");
            return 0;
        }
    }


    return 0;
}
