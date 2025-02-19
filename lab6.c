#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int row=0;
void *prod(void*vargp){
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
		matC[row][i]+=matA[row][k]*mat[k][i];
		}
	}
	row++;
}
int main(){

	
