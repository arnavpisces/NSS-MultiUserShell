#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
//    printf("%d",argc);
    if (argc==0){
        printf("rm: missing operand\n");
        return -1;
    }
    if(argc==1){
//        showdir(argv[0],'y');
        if(remove(argv[0])==0){
//            printf("Deleted Successfully");
        }
        else{
            printf("Unable to delete the file\n");
        }
    }
    else if(argc==2){
        if(strlen(argv[0])==3){
            if(argv[0][1]=='v'&&argv[0][2]=='i' || argv[0][1]=='i'&&argv[0][2]=='v'){
                char ask='n';
                FILE *file;
                file=fopen(argv[1],"r");
                if(file){
                    printf("Are you sure you want to remove the file? ");
                    scanf("%c",&ask);
                    if(ask=='y'){
                        remove(argv[1]);
                        printf("Deleted Successfully\n");
                    }
                    else{
                        return 0;
                    }
                }
                else {
                    printf("rm: cannot remove ‘%s’: No such file or directory\n", argv[1]);
                }
            }
            else{
                printf("AK BASH DOESN't Support the given flags\n");
                return 0;
            }
        }
        else if(argv[0][1]=='i'){
            char ask='n';
            FILE *file;
            file=fopen(argv[1],"r");
            if(file){
                printf("Are you sure you want to remove the file? ");
                scanf("%c",&ask);
                if(ask=='y'){
                    remove(argv[1]);
//                    printf("Deleted Successfully");
                }
                else{
                    return 0;
                }
            }
            else {
                printf("rm: cannot remove ‘%s’: No such file or directory\n", argv[1]);
            }
        }
        else if(argv[0][1]=='v'){
            char ask='n';
            FILE *file;
            file=fopen(argv[1],"r");
            if(file){
                printf("Are you sure you want to remove the file? ");
                scanf("%c",&ask);
                if(ask=='y'){
                    remove(argv[1]);
                    printf("Deleted Successfully\n");
                }
                else{
                    return 0;
                }
            }
            else {
                printf("rm: cannot remove ‘%s’: No such file or directory\n", argv[1]);
            }
        }
        else{
            printf("AK BASH DOESN't Support the given flags\n");
            return 0;
        }
    }
    else if(argc==3){
        char ask='n';
        FILE *file;
        file=fopen(argv[2],"r");
        if(file){
            printf("Are you sure you want to remove the file? ");
            scanf("%c",&ask);
            if(ask=='y'){
                remove(argv[2]);
                printf("Deleted Successfully\n");
            }
            else{
                return 0;
            }
        }
        else {
            printf("rm: cannot remove ‘%s’: No such file or directory\n", argv[2]);
        }
    }
    return 0;
}