#include<iostream>

#include<omp.h>
#include<chrono>

#include<climits>

using namespace std;
using namespace chrono;

void findMin(int arr[], int n) {
    auto start = high_resolution_clock::now();
    int minVal = INT_MAX;
    
    #pragma omp for reduction (min: minVal)
    for(int i=0; i<n; i++) {
        minVal = arr[i] < minVal ? arr[i] : minVal;
    }
    
    cout << "Minimum is : " <<  minVal << endl;

    auto end = high_resolution_clock::now();
    duration<double, micro> duration = end - start;
    cout << "Time taken to find out minimum value is : " << duration.count() << endl << endl;
}

int findMax(int arr[], int n) {
    auto start = high_resolution_clock::now();

    int maxVal = INT_MIN;

    #pragma omp for reduction(max: maxVal)
    for(int i=0; i<n; i++) {
        maxVal = arr[i] > maxVal ? arr[i] : maxVal;
    }

    cout << "Maximum is : " <<  maxVal << endl;

    auto end = high_resolution_clock::now();
    duration<double, micro> duration = end - start;
    cout << "Time taken to find out maximum value is : " << duration.count() << endl << endl;
}

int findSum(int arr[], int n) {
    int sum = 0;
    #pragma omp for reduction(+:sum)
    for(int i=0; i<n; i++) {
        sum += arr[i];
    }
    return sum;
}

int findAverage(int arr[], int n) {
    auto start = high_resolution_clock::now();

    double avg =  static_cast<double> (findSum(arr, n)/n);
    cout << "Average is : " <<  avg << endl;

    auto end = high_resolution_clock::now();
    duration<double, micro> duration = end - start;
    cout << "Time taken to find out average value is : " << duration.count() << endl << endl;
}

void printArray(int arr[], int n) {
    for(int i=0; i<n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    int n; 
    cout <<  "Enter number of elements: ";
    cin >> n;

    if(n <= 0) {
        cout << "Number of elements must be positive!\n";
        return 1;
    }

    int * arr = new int[n];

    for(int i=0; i<n; i++) {
        cout << "arr[" << i << "] = ";
        cin >> arr[i];
    }

    printArray(arr, n);

    findMin(arr, n);

    findMax(arr, n);

    findAverage(arr, n);

    return 0;
}