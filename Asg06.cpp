#include<iostream>

#include<algorithm>
#include<vector>

#include<omp.h>
#include<chrono>

#include<cstdlib>
#include<ctime>

using namespace std;
using namespace chrono;

void printArray(const vector<int>& arr) {
    cout << "[ ";
    for(auto x : arr)
        cout << x << " ";
    cout << "]\n";
}

int partition(vector<int>& arr, int low, int high) {

    int pivot = arr[high];
    int i = low - 1;

    for(int j=low; j < high; j++) {
        if(arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i+1], arr[high]);
    return i+1;
}

void sequentialQuickSort(vector<int>& arr, int low, int high) {
    if(low < high) {
        int pi = partition(arr, low, high);

        sequentialQuickSort(arr, low, pi-1);
        sequentialQuickSort(arr, pi+1, high);
    }
}

void parallelQuickSort(vector<int>& arr, int low, int high) {
    if(low < high) {
        int pi = partition(arr, low, high);

        #pragma omp parallel sections
        {
            #pragma omp section
            parallelQuickSort(arr, low, pi-1);

            #pragma omp section
            parallelQuickSort(arr, pi+1, high);
        }
    }
}

int main(){

    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    if(n<=0) {
        cout << "Number of elements must be a positive integer\n";
        return 1;
    }

    vector<int> arr1(n), arr2(n);

    srand(time(0));

    for(int i=0; i<n; i++) 
        arr1[i] = arr2[i] = rand()%10;

    
    cout << "- - - Sequential Quick Sort - - -\n";
    
    cout << "Before Sorting: ";
    printArray(arr1);

    auto startSeq = high_resolution_clock::now();

    sequentialQuickSort(arr1, 0 , n-1);

    auto endSeq = high_resolution_clock::now();

    cout << "After sorting: ";
    printArray(arr1);

    duration<double, micro> durSeq = endSeq - startSeq;
    cout << "*** Time taken by sequential algorithm is " << durSeq.count() << endl << endl;
    
    cout << "- - - Parallel Quick Sort - - -\n";
    
    cout << "Before Sorting: ";
    printArray(arr2);

    auto startPar = high_resolution_clock::now();

    parallelQuickSort(arr2, 0 , n-1);

    auto endPar = high_resolution_clock::now();

    cout << "After sorting: ";
    printArray(arr2);

    duration<double, micro> durPar = endPar - startPar;
    cout << "*** Time taken by parallel algorithm is " << durPar.count() << endl << endl;

}