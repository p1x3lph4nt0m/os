#include<pthread.h>
#include<stdio.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<sys/syscall.h>
#define int long long
int* fib;
bool dig_verifier(int n,int x){
	int dig=0;
	while(n!=0){
		dig++;
		n/=10;
	}
	if(dig==0)dig=1;
	return dig==x;
}
int n;
void *fib_thread(void* vargp){
	for(int i=2;i<n;i++){
		fib[i]=fib[i-1]+fib[i-2];
	}
	printf("Thread %ld has generated the fibonacci sequence of %lld numbers\n",syscall(SYS_gettid),n);
	pthread_exit(NULL);
}
int32_t main(int argc,char ** argv){
	//just some basic checks and error handlers
	if(argc!=2){
		fprintf(stderr,"You need to enter 1 non negative integer only\n");
		return 1;
	}
	n=atoll(argv[1]);
	if(n<0){
		fprintf(stderr,"fibonacci of negative numbers is not defined, so give non negative integers only\n");
		return 1;
	}
	int chk=strlen(argv[1]);
	if(!dig_verifier(n,chk)){
		fprintf(stderr,"You can't give characters other than numbers\n");
		return 1;
	}
	if(n>93){
		fprintf(stderr,"fib[%lld] exceeds the range of long long data type unfortunately! Please restrict yourself to n<=93\n",n-1);
		return 1;
	}
	//main code for problem execution
	fib=realloc(fib,n*sizeof(int));
	fib[0]=0,fib[1]=1;
	pthread_t thread_id;
	pthread_create(&thread_id,NULL,fib_thread,NULL);
	pthread_join(thread_id,NULL);
	printf("Now the main process %d will print the output\n",getpid());
	for(int i=0;i<n;i++){
		printf("%lld ",fib[i]);
	}
}		
