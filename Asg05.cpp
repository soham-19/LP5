#include<iostream>
#include<vector>
#include<algorithm>
#include<chrono>
#include<omp.h>
#include<ctime>
#include<cstdlib>

using namespace std;
using namespace chrono;

void printArr(vector<int>& arr) {
    cout << "[ ";
    for(auto it : arr)
        cout << it << " ";
    cout << "]" << endl;
}

void merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> temp;

    int i = left;
    int j = mid+1;

    while(i <= mid && j <= right) {
        if(arr[i] <= arr[j]) {
            temp.push_back(arr[i++]);
        } else {
            temp.push_back(arr[j++]);
        }
    }

    while(i <= mid) {
        temp.push_back(arr[i++]);
    }

    while(j <= right) {
        temp.push_back(arr[j++]);
    }

    for(int k=0; k<temp.size(); k++) 
        arr[left+k] = temp[k];
        
}

void sequentialMergeSort(vector<int>& arr, int left, int right) {
    if(left < right) {
        int mid = (left+right)/2;

        sequentialMergeSort(arr, left, mid);
        sequentialMergeSort(arr, mid+1, right);

        merge(arr, left, mid, right);
    }
}

void parallelMergeSort(vector<int>& arr, int left, int right) {
    if(left < right) {
        int mid = (left+right)/2;

        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, left, mid);

            #pragma omp section
            parallelMergeSort(arr, mid+1, right);
        }

        merge(arr, left, mid, right);
    }
}

void sequentialMergeSort(vector<int>& arr) {
    cout << "Before Sorting: ";
    printArr(arr);
    auto start = high_resolution_clock::now();
    sequentialMergeSort(arr, 0, arr.size()-1);
    auto end = high_resolution_clock::now();
    duration<double, micro> duration = end - start;
    cout << "After Sorting: ";
    printArr(arr);
    cout << "Sequential Sorting Done in " << duration.count() << " microseconds\n";
}

void parallelMergeSort(vector<int>& arr) {
    cout << "Before Sorting: ";
    printArr(arr);
    auto start = high_resolution_clock::now();
    parallelMergeSort(arr, 0, arr.size()-1);
    auto end = high_resolution_clock::now();
    duration<double, micro> duration = end - start;
    cout << "After Sorting: ";
    printArr(arr);
    cout << "Parallel Sorting Done in " << duration.count() << " microseconds\n";
}

int main() {
    int n;
    cout << "Enter value of n: ";
    cin >> n;

    if(n<=0){
        cout << "N must be positive integer";
        return 1;
    }

    vector<int> arr(n), arr2(n);

    srand(time(0));
    for(int i=0; i<n; i++)
        arr[i] = arr2[i] = rand()%25;

    sequentialMergeSort(arr);
    parallelMergeSort(arr2);
}