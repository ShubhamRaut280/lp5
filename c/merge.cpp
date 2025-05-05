#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

// Merge function
void merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right) {
        if (arr[i] < arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];

    for (int i = 0; i < temp.size(); i++)
        arr[left + i] = temp[i];
}

// Sequential Merge Sort
void sequentialMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        sequentialMergeSort(arr, left, mid);
        sequentialMergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Parallel Merge Sort using OpenMP
void parallelMergeSort(vector<int>& arr, int left, int right, int depth = 0) {
    if (left < right) {
        int mid = (left + right) / 2;

        if (depth <= 4) { // Limit depth to prevent oversubscription
            #pragma omp parallel sections
            {
                #pragma omp section
                parallelMergeSort(arr, left, mid, depth + 1);
                #pragma omp section
                parallelMergeSort(arr, mid + 1, right, depth + 1);
            }
        } else {
            sequentialMergeSort(arr, left, mid);
            sequentialMergeSort(arr, mid + 1, right);
        }
        merge(arr, left, mid, right);
    }
}

int main() {
    int n;
    cout << "Enter number of elements to sort: ";
    cin >> n;

    vector<int> arr(n), arr_copy(n);
    cout << "Enter " << n << " integers:\n";
    for (int i = 0; i < n; i++)
        cin >> arr[i];
    arr_copy = arr;

    // Sequential sort
    double start_seq = omp_get_wtime();
    sequentialMergeSort(arr, 0, n - 1);
    double end_seq = omp_get_wtime();

    // Parallel sort
    double start_par = omp_get_wtime();
    parallelMergeSort(arr_copy, 0, n - 1);
    double end_par = omp_get_wtime();

    // Output sorted arrays
    cout << "\nSorted array (Sequential): ";
    for (int x : arr) cout << x << " ";

    cout << "\nSorted array (Parallel):   ";
    for (int x : arr_copy) cout << x << " ";

    // Time comparison
    cout << "\n\nTime taken by SEQUENTIAL Merge Sort: " << (end_seq - start_seq) << " seconds";
    cout << "\nTime taken by PARALLEL Merge Sort:   " << (end_par - start_par) << " seconds\n";

    return 0;
}
