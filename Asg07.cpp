#include <iostream>

#include<omp.h>
#include<chrono>

#include<vector>

#include<cstdlib>
#include<ctime>

using namespace std;
using namespace chrono;

#define MAX 100

void printMatrix(int M[MAX][MAX], int r, int c) {
    for(int i=0; i<r; i++) {
        for(int j=0; j<c; j++) {
            cout << M[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void sequentialMultiplication(int A[MAX][MAX], int B[MAX][MAX], int C[MAX][MAX], int n, int p, int m) {
    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            C[i][j] = 0;
            for(int k=0; k<p; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void parallelMultiplication(int A[MAX][MAX], int B[MAX][MAX], int C[MAX][MAX], int n, int p, int m) {
    #pragma omp parallel for collapse(2)
    for(int i=0; i<n; i++)
        for(int j=0; j<m; j++)
            for(int k=0; k<p; k++)
                C[i][j] += A[i][k] * B[k][j];    
}

int main() {

    srand(time(0));

    int n, p, m;
    cout << "Enter the dimentions n, p, m:\t(A:n*p, B:p*m, C:n*m)" << endl;
    cin >> n >> p >> m;

    int A[MAX][MAX], B[MAX][MAX], CSeq[MAX][MAX] = {}, CPar[MAX][MAX] = {};

    cout << "Generating Matrix A randomly...\n";
    for(int i=0; i<n; i++) {
        for(int j=0; j<p; j++) {
            A[i][j] = rand()%10;
        }
    }

    cout << "Generating Matrix B randomly...\n";
    for(int i=0; i<p; i++) {
        for(int j=0; j<m; j++) {
            B[i][j] = rand()%10;
        }
    }

    cout << "Matrix A\n";
    printMatrix(A, n, p);

    cout << "Matrix B\n";
    printMatrix(B, p, m);

    cout << "\n - - - Sequential Matrix Multiplication - - -\n";

    auto startSeq = high_resolution_clock::now();
    sequentialMultiplication(A, B, CSeq, n, p, m);
    auto endSeq = high_resolution_clock::now();

    printMatrix(CSeq, n, m);

    duration<double, micro> durSeq = endSeq - startSeq;
    cout << "Time taken by sequential algorith is " << durSeq.count() << endl;

    cout << "\n - - - Parallel Matrix Multiplication - - -\n";

    auto startPar = high_resolution_clock::now();
    parallelMultiplication(A, B, CPar, n, p, m);
    auto endPar = high_resolution_clock::now();

    printMatrix(CPar, n, m);

    duration<double, micro> durPar = endPar - startPar;
    cout << "Time taken by parallel algorith is " << durPar.count() << endl;
}
