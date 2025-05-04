#include<iostream>

#include<vector>
#include<algorithm>

#include<omp.h>
#include<chrono>

using namespace std;
using namespace chrono;

void printVector(const vector<int>& arr) {
    cout << "[ ";
    for(auto it : arr) {
        cout << it << " ";
    }
    cout << "]\n";
}

void sequentialBubbleSort(vector <int>& arr) {
    cout << "\n---Sequential Bubble Sort---\n";
    printVector(arr);

    int n = arr.size();
    
    auto start = high_resolution_clock::now();
    for(int i=0; i<n-1; i++) {
        for(int j=0; j<n-i-1; j++) {
            if(arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
            }
        }
    }
    auto end = high_resolution_clock::now();

    cout << "Sorted Array is : ";
    printVector(arr);
    duration<double, milli> duration = end - start;
    cout << "Time taken for sequential sorting is : " << duration.count() << endl;
}

void parallelBubbleSort(vector <int>& arr) {
    cout << "\n---Parallel Bubble Sort---\n";
    printVector(arr);

    int n = arr.size();
    
    auto start = high_resolution_clock::now();
    for(int i=0; i<n; i++) {
        #pragma omp parallel for
        for(int j=i%2; j<n-1; j+=2) {
            if(arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
            }
        }
    }
    auto end = high_resolution_clock::now();

    cout << "Sorted Array is : ";
    printVector(arr);

    duration<double, milli> duration = end - start;
    cout << "Time taken for parallel sorting is : " << duration.count() << endl;
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    if(n <= 0) {
        cout << "Number of elements must be positive!!\n";
        return 1;
    }

    vector<int> arrSeq(n), arrPar(n);

    for(int i=0; i<n; i++) {
        cout << "arr[" << i << "] = ";
        cin >> arrSeq[i];
        arrPar[i] = arrSeq[i];
    }

    sequentialBubbleSort(arrSeq);

    parallelBubbleSort(arrPar);

    return 0;
}