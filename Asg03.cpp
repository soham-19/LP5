#include<iostream>

#include<omp.h>
#include<chrono>

#include<ctime>
#include<cstdlib>

using namespace std;
using namespace chrono;

void printArray(int arr[], int n) {
    cout << "[ ";
    for(int i=0; i<n; i++)
        cout << arr[i] << " ";
    cout << "]\n\n";
}

void getMin(int arr[] , int n) {
    auto start = high_resolution_clock::now();

    int minVal = arr[0];

    #pragma omp parallel reduction(min: minVal)
    for(int i=1; i<n; i++)
        minVal = minVal < arr[i] ? minVal : arr[i];
    
    cout << "Minimum Value is " << minVal << endl;

    auto end = high_resolution_clock::now();

    duration<double, nano> duration = end - start;
    cout << "Mimimum found in " << duration.count() << " nanoseconds\n";
}

void getMax(int arr[] , int n) {
    auto start = high_resolution_clock::now();

    int maxVal = arr[0];

    #pragma omp parallel reduction(max: maxVal)
    for(int i=1; i<n; i++)
        maxVal = maxVal > arr[i] ? maxVal: arr[i];
    
    cout << "Maximum Value is " << maxVal << endl;
    
    auto end = high_resolution_clock::now();
    
    duration<double, nano> duration = end - start;
    cout << "Maximum found in " << duration.count() << " nanoseconds\n";
}

int getSum(int arr[] , int n) {
    
    int sum = 0;
    
    #pragma omp parallel reduction(+:sum)
    for(int i=0; i<n; i++)
    sum += arr[i];
    
    return sum;
}

void getAvg(int arr[] , int n) {
    auto start = high_resolution_clock::now();
    
    double avg = (double)getSum(arr, n)/n;
    
    auto end = high_resolution_clock::now();
    
    cout << "Average Value is " << avg << endl;

    duration<double, nano> duration = end - start;
    cout << "Averaeg found in " << duration.count() << " nanoseconds\n";
}
int main() {

    int n;
    cout << "Enter number of elements : ";
    cin >> n;

    if(n <= 0) {
        cerr << "Number of elements must be positive!";
        return 1;
    }

    int arr[n];

    srand(time(0));
    for(int i=0; i<n; i++) {
        arr[i] = rand()%25;
    }

    printArray(arr, n);

    getMin(arr, n);
    getMax(arr, n);
    getAvg(arr, n);

    return 0;
}