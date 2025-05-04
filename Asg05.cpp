#include<iostream>
#include<vector>

#include<omp.h>
#include<chrono>

#include<cstdlib>
#include<ctime>

using namespace std;
using namespace std::chrono;

void printArray(const vector<int>& arr) {
    cout << "[ ";
    for(auto x : arr)
        cout << x << " ";
    cout << "]\n";
}

void merge(vector<int>& arr, int left, int mid, int right) {
    
    vector<int> temp;
    int i = left, j = mid+1;

    while(i<= mid && j<= right) {
        if(arr[i] < arr[j])
            temp.push_back(arr[i++]);
        else    
            temp.push_back(arr[j++]);
    }

    while(i <= mid)
        temp.push_back(arr[i++]);

    while(j <= right)
        temp.push_back(arr[j++]);

    for(int k=0; k<temp.size(); k++)
        arr[left+k] = temp[k];
}

void sequentialMergeSort(vector <int>& arr, int left, int right) {
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

int main() {

    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    if(n<=0) {
        cout << "Number of elements must be an positive integer!\n\n";
        return 1;
    }

    vector<int> arr1(n), arr2(n);

    cout << "Generating array of random " << n << " numbers...\n";

    srand(time(0));
    for (int i = 0; i < n; i++){
        arr1[i] = arr2[i] = rand() % n;
    }

    cout << "\n - - - Sequential Merge Sort - - - \n\n";

    // cout << "Before Sorting:\t";
    // printArray(arr1);

    auto startSeq = high_resolution_clock::now();
    sequentialMergeSort(arr1, 0, n-1);
    auto endSeq = high_resolution_clock::now();
    
    // cout << "After Sorting:\t";
    // printArray(arr1);

    duration<double, micro> durSeq = endSeq - startSeq;
    cout << "Time taken by sequential merge sort is " << durSeq.count() << "ms\n\n\n";

    cout << "\n - - - Parallel Merge Sort - - - \n\n";

    // cout << "Before Sorting:\t";
    // printArray(arr2);
    auto startPar = high_resolution_clock::now();
    parallelMergeSort(arr2, 0, n-1);
    auto endPar = high_resolution_clock::now();

    cout << "After Sorting:\t";
    // printArray(arr2);

    duration<double, micro> durPar = endPar - startPar;
    cout << "Time taken by parallel merge sort is " << durPar.count() << "ms\n";

    return 0;
}