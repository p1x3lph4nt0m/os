#include<stdio.h>
#include<stdlib.h>
#include<time.h>
void populate_matrix(int n,int m,int o,int a1[n][m],int a2[m][o]){
	for(int i=0;i<n;i++){
		for(int j=0;j<m;j++){
			a1[i][j]=rand()%10;
		}
	}
	for(int i=0;i<m;i++){
		for(int j=0;j<o;j++){
			a2[i][j]=rand()%10;
		}
	}
}
int main(int argc,char **argv){
	int n=atoi(argv[1]),m=atoi(argv[2]),o=atoi(argv[3]);
	int matA[n][m],matB[m][o],matC[n][o];
	srand(time(NULL));
	populate_matrix(n,m,o,matA,matB);
	for(int i=0;i<n;i++){
		for(int j=0;j<o;j++){
			matC[i][j]=0;
			for(int k=0;k<m;k++){
				matC[i][j]+=matA[i][k]*matB[k][j];
			}
		}
	}
	/*
	printf("Resultant Matrix\n");
	for(int i=0;i<n;i++){
		for(int j=0;j<o;j++){
			printf("%d ",matC[i][j]);
		}
		printf("\n");
	}
	*/
	return 0;
}
