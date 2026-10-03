#include <stdio.h>
#include <time.h>

#define N 500     
#define TILE 25    

int A[N][N];
int B[N][N];
int C[N][N];


void fillMatrix(int M[N][N]) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            M[i][j] = (i + j) % 10;
        }
    }
}

void clearMatrix(int M[N][N]) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            M[i][j] = 0;
        }
    }
}

void multiply() {
    int i, j, k;
    clearMatrix(C);
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            int sum = 0;
            for (k = 0; k < N; k++) {
                sum = sum + A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

void LoopInterchange() {
    int i, j, k;
    clearMatrix(C);
    for (i = 0; i < N; i++) {
        for (k = 0; k < N; k++) {
            for (j = 0; j < N; j++) {
                C[i][j] = C[i][j] + A[i][k] * B[k][j];
            }
        }
    }
}


void LoopTiling() {
    int i, j, k, a, b, c;
    clearMatrix(C);
    for (a = 0; a < N; a = a + TILE) {
        for (c = 0; c < N; c = c + TILE) {
            for (b = 0; b < N; b = b + TILE) {
                for (i = a; i < a + TILE && i < N; i++) {
                    for (k = c; k < c + TILE && k < N; k++) {
                        for (j = b; j < b + TILE && j < N; j++) {
                            C[i][j] = C[i][j] + A[i][k] * B[k][j];
                        }
                    }
                }
            }
        }
    }
}

void LoopUnrolling() {
    int i, j, k;
    clearMatrix(C);
    for (i = 0; i < N; i++) {
        for (k = 0; k < N; k++) {
            j = 0;
            for (; j <= N - 4; j = j + 4) {
                C[i][j]     = C[i][j]     + A[i][k] * B[k][j];
                C[i][j + 1] = C[i][j + 1] + A[i][k] * B[k][j + 1];
                C[i][j + 2] = C[i][j + 2] + A[i][k] * B[k][j + 2];
                C[i][j + 3] = C[i][j + 3] + A[i][k] * B[k][j + 3];
            }
            for (; j < N; j++) {
                C[i][j] = C[i][j] + A[i][k] * B[k][j];
            }
        }
    }
}

int sameResult(int M1[N][N], int M2[N][N]) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (M1[i][j] != M2[i][j]) {
                return 0; 
            }
        }
    }
    return 1;
}

int main() 
{
    double start_time, end_time;
    int resultCopy[N][N];
    int i, j;

    printf("Matrix size: %d x %d\n\n", N, N);

    fillMatrix(A);
    fillMatrix(B);

    /* Basic implementation */
    
    start_time = (double) clock();
    multiply();
    end_time = (double) clock();
    printf("Normal multiplication : %.4f seconds\n", (end_time - start_time) / CLOCKS_PER_SEC);

    for (i = 0; i < N; i++) 
	{
        for (j = 0; j < N; j++) 
		{
            resultCopy[i][j] = C[i][j];
        }
    }

    /* Loop Interchange */
    
    start_time = (double) clock();
    LoopInterchange();
    end_time = (double) clock();
    printf("Loop Interchange : %.4f seconds  (correct = %s)\n", (end_time - start_time) / CLOCKS_PER_SEC, sameResult(C, resultCopy) ? "yes" : "no");

    /* 3. Loop Tiling */
    
    start_time = (double) clock();
    LoopTiling();
    end_time = (double) clock();
    printf("Loop Tiling : %.4f seconds  (correct = %s)\n", (end_time - start_time) / CLOCKS_PER_SEC, sameResult(C, resultCopy) ? "yes" : "no");

    /* 4. Loop Unrolling */
    
    start_time = (double) clock();
    LoopUnrolling();
    end_time = (double) clock();
    printf("Loop Unrolling : %.4f seconds  (correct = %s)\n", (end_time - start_time) / CLOCKS_PER_SEC, sameResult(C, resultCopy) ? "yes" : "no");

    return 0;
}
