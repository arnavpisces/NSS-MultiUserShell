#include <stdio.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[]){
//    struct date d;
//    time_T current_time;
//    char *c_time_string;
//    current_time=time(NULL);
//    c_time_string=ctime(&current_time);
//    getdate(&d);
//    printf("%d/%d/%d %s",d.da_day,d.da_mon,d.da_year,c_time_string);
    time_t t = time(NULL);
    struct tm *local,* gm;
    local=localtime(&t);
    gm=gmtime(&t);
//    if(!strcmp(argv[0],"-R")){
//        printf("now: %d-%d-%d %d:%d:%d\n", tm.tm_mon + 1, tm.tm_mday,  tm.tm_year + 1900,   tm.tm_hour, tm.tm_min, tm.tm_sec);
//        return 0;
//    }
//
//
//    printf("now: %d-%d-%d %d:%d:%d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,   tm.tm_hour, tm.tm_min, tm.tm_sec);
////    return 0;

    if(argc==0){
//        printf("now: %d-%d-%d %d:%d:%d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,   tm.tm_hour, tm.tm_min, tm.tm_sec);
        printf("%s\n",asctime(local));
    }
    else if(argc==1){
        if(strlen(argv[0])==3){
            if(argv[0][1]=='R'&&argv[0][2]=='u' || argv[0][1]=='u'&&argv[0][2]=='R') {
                system("date -Ru");

            }
            else{
                printf("AK BASH DOESN't Support the given flags\n");
                return 0;
            }
        }
        else if(argv[0][1]=='R'){
            char *astime= asctime(gm);
            astime[strlen(astime)-1]='\0';
            printf("%s +0530\n",astime);
        }
        else if(argv[0][1]=='u'){
            char *astime= asctime(gm);
            astime[strlen(astime)-1]='\0';
            printf("%s UTC\n",astime);
        }
        else{
            printf("AK BASH DOESN't Support the given flags\n");
            return 0;
        }
    }
    else if(argc==2){
        system("date -Ru");
    }

}