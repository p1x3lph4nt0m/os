#include<stdio.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#define int long long
int32_t main(int argc,char** argv){
	if(argc!=2){
		fprintf(stderr,"You need to enter 1 positive integer only\n");
		return 1;
	}
	int num=atoll(argv[1]);
	int dig=0,n=num;
	while(n!=0){
		dig++;
		n/=10;
	}
	if(dig==0)dig=1;
	if(num<=0){
		fprintf(stderr,"Enter positive integers only\n");
		return 1;
	}
	if(dig!=strlen(argv[1])){
			fprintf(stderr,"You can't give characters other than numbers\n");
			return 1;
	}
	pid_t pid;
	pid=fork();
	if(pid<0){
		fprintf(stderr,"Error in fork\n");
	}
	else if(pid==0){
		printf("%lld ",num);
		while(num!=1){
			if(num%2){
				num=3*num+1;
			}
			else{
				num/=2;
			}
			printf("%lld ",num);
		}
		printf("\n");
		exit(0);
	}
	else{
		wait(NULL);
		printf("Process Completed\n");
	}
}



