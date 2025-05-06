#include<iostream>

#include<vector>
#include<omp.h>
#include<chrono>

using namespace std;
using namespace chrono;

#define MAX 100

void seqMVM(
    int A[MAX][MAX],
    const vector<int>& x,
    vector<int>& result, int n, int m
) {
    for(int i=0; i<n; i++) {
        result[i] = 0;
        for(int j=0; j<m; j++)
            result[i] += A[i][j] * x[j];
    }
}

void parMVM(
    int A[MAX][MAX],
    const vector<int>& x,
    vector<int>& result, int n, int m
) {
    #pragma omp parallel for
    for(int i=0; i<n; i++) {
        result[i] = 0;
        for(int j=0; j<m; j++)
            result[i] += A[i][j] * x[j];
    }
}

void printVector(const vector<int>& result) {
    for(auto it: result)
        cout << it << " ";
    cout << endl;
}

int main() {

    int n, m;
    cout << "Enter dimentions (A: n*m, x:m*1)" << endl;
    cout << "Enter n ";
    cin >> n;
    cout << "Enter m ";
    cin >> m;

    int A[MAX][MAX];
    vector<int> x(m);
    vector<int> resSeq(n,0), resPar(n,0);

    
    cout << "Enter matrix A elements:\n";
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) {
            cout << "A[" << i << "][" << j << "]: ";
            cin >> A[i][j];
        }

    cout << "Enter vector x elements:\n";
    for (int i = 0; i < m; ++i) {
        cout << "x[" << i << "]: ";
        cin >> x[i];
    }

    auto startSeq = chrono::high_resolution_clock::now();
    seqMVM(A, x, resSeq, n, m);
    auto endSeq = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> durationSeq = endSeq - startSeq;

    auto startPar = chrono::high_resolution_clock::now();
    parMVM(A, x, resPar, n, m);
    auto endPar = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> durationPar = endPar - startPar;

    cout << "\n----- Sequential Matrix-Vector Multiplication Output -----\n";
    printVector(resSeq);
    cout << "Time Taken: " << durationSeq.count() << " milliseconds\n";

    cout << "\n----- Parallel Matrix-Vector Multiplication Output -----\n";
    printVector(resPar);
    cout << "Time Taken: " << durationPar.count() << " milliseconds\n";

    return 0;
}
