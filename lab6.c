#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

typedef struct {
    int **a1;
    int **a2;
    int **a3;
    int n1;
    int n2;
} MatrixData;

void *prod(void *vargp) {
    MatrixData *data = (MatrixData *)vargp;
    int x = 0;
    for (int i = 0; i < data->n1; i++) {
        x += (data->a1)[data->n1][i] * (data->a2)[i][data->n2];
    }
    data->a3[data->n1][data->n2] = x;
    free(data);
    pthread_exit(NULL);
}

void populate_matrix(int n, int m, int o, int **a1, int **a2) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            a1[i][j] = rand() % 10;
        }
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < o; j++) {
            a2[i][j] = rand() % 10;
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 4) {
        printf("Usage: %s <rows_A> <cols_A_rows_B> <cols_B>\n", argv[0]);
        return 1;
    }
    
    srand(time(NULL));
    
    int n = atoi(argv[1]), m = atoi(argv[2]), o = atoi(argv[3]);
    int **matA = (int **)malloc(n * sizeof(int *));
    int **matB = (int **)malloc(m * sizeof(int *));
    int **matC = (int **)malloc(n * sizeof(int *));
    
    for (int i = 0; i < n; i++) matA[i] = (int *)malloc(m * sizeof(int));
    for (int i = 0; i < m; i++) matB[i] = (int *)malloc(o * sizeof(int));
    for (int i = 0; i < n; i++) matC[i] = (int *)malloc(o * sizeof(int));
    
    populate_matrix(n, m, o, matA, matB);
    
    pthread_t th[n][o];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < o; j++) {
            MatrixData *data = (MatrixData *)malloc(sizeof(MatrixData));
            data->n1 = i;
            data->n2 = j;
            data->a1 = matA;
            data->a2 = matB;
            data->a3 = matC;
            pthread_create(&th[i][j], NULL, prod, data);
        }
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < o; j++) {
            pthread_join(th[i][j], NULL);
        }
    }
    
    printf("Matrix A:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%d ", matA[i][j]);
        }
        printf("\n");
    }
    
    printf("\nMatrix B:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < o; j++) {
            printf("%d ", matB[i][j]);
        }
        printf("\n");
    }
    
    printf("\nResult Matrix C:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < o; j++) {
            printf("%d ", matC[i][j]);
        }
        printf("\n");
    }
    
    for (int i = 0; i < n; i++) free(matA[i]);
    for (int i = 0; i < m; i++) free(matB[i]);
    for (int i = 0; i < n; i++) free(matC[i]);
    free(matA);
    free(matB);
    free(matC);
    
    return 0;
}
what is the meaning of this?
