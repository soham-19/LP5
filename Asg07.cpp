#include<iostream>
#include<chrono>
#include<omp.h>
#include<ctime>
#include<cstdlib>

using namespace std;
using namespace chrono;

#define MAX 100

void print(int A[MAX][MAX], int n, int m) {
    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++)
            cout << A[i][j] << "\t";
        cout << endl;
    }
    cout << endl;
}

void sequentialMatrixMultiplication(
    int A[MAX][MAX], 
    int B[MAX][MAX],
    int C[MAX][MAX],
    int n, 
    int p,
    int m
) {
    cout << "Matrix A\n";
    print(A, n, p);
    cout << "Matrix B\n";
    print(B, p , m);

    auto start = high_resolution_clock::now();

    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++) {
            for(int k=0; k<p; k++){
                C[i][j] += A[i][k]* B[k][j];
            }
        }
    }
    
    auto end = high_resolution_clock::now();

    cout << "Resultant Matrix\n";
    print(C, n, m);

    duration<double, micro> duration = end - start;
    cout << "Time taken for sequential multiplication is " << duration.count() << " microseconds\n";
}

void parallelMatrixMultiplication(
    int A[MAX][MAX], 
    int B[MAX][MAX],
    int C[MAX][MAX],
    int n, 
    int p,
    int m
) {
    cout << "Matrix A\n";
    print(A, n, p);
    cout << "Matrix B\n";
    print(B, p , m);

    auto start = high_resolution_clock::now();

    #pragma omp parallel for collapse(2)
    for(int i=0; i<n; i++)
        for(int j=0; j<m; j++)
            for(int k=0; k<p; k++)
                C[i][j] += A[i][k]* B[k][j];

    auto end = high_resolution_clock::now();

    cout << "Resultant Matrix\n";
    print(C, n, m);

    duration<double, micro> duration = end - start;
    cout << "Time taken for parallel multiplication is " << duration.count() << " microseconds\n";
}

int main() {
    int n, p, m;
    cout << "Enter the dimentions n, p, m\tA(n*p), B(p*m), Result(n*m)\t";
    cin >> n >> p >> m;

    int A[MAX][MAX], B[MAX][MAX], Cs[MAX][MAX] = {}, Cp[MAX][MAX] = {};

    srand(time(0));
    for(int i=0; i<n; i++){
        for(int j=0; j<p; j++)
            A[i][j] = (rand()%9) + 1;
    }

    for(int i=0; i<p; i++){
        for(int j=0; j<m; j++)
            B[i][j] = (rand()%9) + 1;
    }

    sequentialMatrixMultiplication(A, B , Cs, n, p, m);

    parallelMatrixMultiplication(A, B , Cp, n, p, m);
}