#include<iostream>
#include<vector>
#include<algorithm>
#include<ctime>
#include<cstdlib>
#include<omp.h>
#include<chrono>

using namespace std;
using namespace chrono;

void printArray(int arr[], int n) {
    cout << "[ ";
    for(int i=0; i<n; i++)
        cout << arr[i] << " ";
    cout << "]\n";
}

void sequentialBubbleSort(int arr[], int n) {
    cout << "   --- SEQUENTIAL BUBBLE SORT ---\n";

    cout << "Before Sorting: ";
    printArray(arr, n);

    auto start = high_resolution_clock::now();

    for(int i=0; i<n-1; i++) {
        for(int j=0; j<n-i-1; j++) {
            if(arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
            }
        }
    }
    auto end = high_resolution_clock::now();

    cout << "After Sorting: ";
    printArray(arr, n);

    duration<double, nano> duration = end - start;
    cout << "Sequential Sorting done in " << duration.count() << " nanoseconds\n";
}

void parallelBubbleSort(int arr[], int n) {
    cout << "   --- PARALLEL BUBBLE SORT ---\n";

    cout << "Before Sorting: ";
    printArray(arr, n);

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

    cout << "After Sorting: ";
    printArray(arr, n);

    duration<double, nano> duration = end - start;
    cout << "Parallel Sorting done in " << duration.count() << " nanoseconds\n";
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    if(n <= 0) {
        cout << "Number of elements must be posiitve!";
        return 1;
    }

    int arr[n], arr2[n];

    srand(time(0));
    for(int i=0; i<n; i++)
        arr2[i] = arr[i] = rand()%50;


    sequentialBubbleSort(arr, n);
    parallelBubbleSort(arr2, n);
    
    return 0;
}