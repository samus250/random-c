#include <stdio.h>

// Play around with the values
#define TOTAL 12000
#define START 1
#define ADD 1

int calpercent(int done, int total);
void percent(int at,int total);

int main(){
    int i=START;
    while(i<=TOTAL){
        percent(i,TOTAL);
        i+=ADD;
    }
    
    system("pause > nul");
    return 0;
}

int calpercent(int done, int total){
    int temp;
    temp = done*100;
    return temp/total;
}

void percent(int at,int total){
   //Write function
   int i, percent = calpercent(at,total), k; 
   if(percent <10){
         printf("%d%%  |",percent);
   }else{
         printf("%d%% |",percent);
   }
   for(i=1;i<=25;i++){
       if ((i-1)*4 < percent){
          if (percent <= i*4){
              printf("*");
          }else{
              printf("-");
          }
       }else{
          printf("-");
       }
   }
   printf("|");
   for (k=1;k<=32;k++){
            printf("\b");
        }
}
