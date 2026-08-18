#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

int main(){
   // int random = rand();
    for (int i = 0; i < 5; i++)
    {
        int random = srand(123);
        printf("%d\n",random);
    }
    
    return 0;
}