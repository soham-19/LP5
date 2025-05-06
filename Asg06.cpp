#include<iostream>
#include<vector>
#include<algorithm>
#include<omp.h>
#include<chrono>
#include<ctime>
#include<cstdlib>

using namespace std;
using namespace chrono;

void print(vector<int>& arr) {
    cout << "[ ";
    for(int x : arr) {
        cout << x << " ";
    }
    cout << "]" << endl;
}

int partition(vector<int>&arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for(int j=low; j<high; j++) {
        if(arr[j] <= pivot) {
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

void sequentialQuickSort(vector<int>& arr) {

    cout << "Before sorting : ";
    print(arr);

    auto start = high_resolution_clock::now();

    sequentialQuickSort(arr, 0, arr.size()-1);

    auto end = high_resolution_clock::now();

    cout << "After sorting : ";
    print(arr);
    duration<double, micro> duration = end - start;
    cout << "Sequential sorting done in " << duration.count() << endl;
}

void parallelQuickSort(vector<int>& arr) {
    
    cout << "Before sorting : ";
    print(arr);

    auto start = high_resolution_clock::now();

    parallelQuickSort(arr, 0, arr.size()-1);

    auto end = high_resolution_clock::now();
    
    cout << "After sorting : ";
    print(arr);

    duration<double, micro> duration = end - start;
    cout << "Parallel sorting done in " << duration.count() << endl;
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    if(n <= 0) {
        cout << "Number of elememnts must be positive!";
        return 1;
    }

    vector<int> arr(n), arr2(n);

    for(int i=0; i<n; i++)
        arr[i] = arr2[i] = rand()%25;

    sequentialQuickSort(arr);
    parallelQuickSort(arr2);

    return 0;
}