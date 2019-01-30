#include <stdio.h>
#include <dirent.h>
#include <getopt.h>
#include <string.h>

void showdir(char*,char);

int main(int argc, char *argv[]){
//    printf("%d",argc);
//    int option=0;
//    while((option=getopt(argc,argv,"aC:"))!=-1){
//        printf("SAdfasdf");
//        printf("\n%c",option);
//        switch (option){
//            case 'a': printf("aaaaaaaaaa");
//                break;
//            case 'C':printf("CCCCCCCCCC");
//                break;
//        }
//    }
    if(argc==1){
        showdir(argv[0],'y');
    }
    else if(argc==2){
        if(strlen(argv[0])==3){
            if(argv[0][1]=='C'&&argv[0][2]=='a' || argv[0][1]=='a'&&argv[0][2]=='C') {
                showdir(argv[1], 'n');
            }
            else{
                printf("AK BASH DOESN't Support the given flags\n");
                return 0;
            }
        }
        else if(argv[0][1]=='a'){
            showdir(argv[1],'n');
        }
        else if(argv[0][1]=='C'){
            showdir(argv[1],'y');
        }
        else{
            printf("AK BASH DOESN't Support the given flags\n");
            return 0;
        }
    }
    else if(argc==3){
        showdir(argv[1],'n');
    }

    return 0;
}

void showdir(char *dir,char flag) {
    struct dirent *de;
    DIR *dr = opendir(dir);
    if (dr == NULL) {
        printf("Could not open current directory\n");
        return;
    }
    if (flag == 'y') { //won't show hidden
    while ((de = readdir(dr)) != NULL)
        if (de->d_name[0] != '.')
            printf("%s\n", de->d_name);
    closedir(dr);
    }
    else{ //will show hidden
        while ((de = readdir(dr)) != NULL)
//            if (de->d_name[0] != '.')
                printf("%s\n", de->d_name);
        closedir(dr);

    }

}
