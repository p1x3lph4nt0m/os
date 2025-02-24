#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
typedef struct{
	int** a1;
	int** a2;
	int** a3;
	int n1;
	int n2;
}MatrixData;
int n,m,o;
void *prod(void* vargp){
	MatrixData* data =(MatrixData*) vargp;
	int** matA= data->a1;int** matB=data->a2;
	int p=data->n1,q=data->n2,x=0;
	for(int i=0;i<m;i++){
		x+=matA[p][i]*matB[i][q];
	}
	data->a3[p][q]=x;
	free(data);
	pthread_exit(NULL);
}
int main(){
	scanf("%d %d %d",&n,&m,&o);
	
	//Matrix A,B,C are of n*n order
	int** matA=(int**)malloc(n*sizeof(int*));
	int** matB=(int**)malloc(m*sizeof(int*));
	int** matC=(int**)malloc(n*sizeof(int*));
	for(int i=0;i<n;i++)matA[i]=(int*)malloc(m*sizeof(int));
	for(int i=0;i<m;i++)matB[i]=(int*)malloc(o*sizeof(int));
	for(int i=0;i<m;i++)matC[i]=(int*)malloc(o*sizeof(int));
	for(int i=0;i<n;i++){
		for(int j=0;j<m;j++){
			scanf("%d",&matA[i][j]);
		}
	}
	for(int i=0;i<m;i++){
		for(int j=0;j<o;j++){
			scanf("%d",&matB[i][j]);
		}
	}
	for(int i=0;i<n;i++){
		for(int j=0;j<o;j++){
			MatrixData* data;
			data->n1=i,data->n2=j;
			data->a1=matA,data->a2=matB,data->a3=matC;

			pthread_t th;
			pthread_create(&th,NULL,prod,&data);
			pthread_join(th,NULL);
		}
	}
	//for(int i=0;i<n;i++){
	//	for(int j=0;j<o;j++){
	//		pthread_join(th,NULL);
	//	}
	//}
	for(int i=0;i<n;i++){
		for(int j=0;j<o;j++){
			printf("%d ",matC[i][j]);
		}
		printf("\n");
	}
	for(int i=0;i<n;i++)free(matA[i]);
	for(int i=0;i<m;i++)free(matB[i]);
	for(int i=0;i<n;i++)free(matC[i]);
	free(matA),free(matB),free(matC);
	return 0;
}


	
