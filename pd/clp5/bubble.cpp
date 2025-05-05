#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

void sequentialBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n; i++) {
        // Odd phase
        #pragma omp parallel for
        for (int j = 1; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }

        // Even phase
        #pragma omp parallel for
        for (int j = 0; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

int main() {
    int n;
    cout << "Enter number of elements to sort: ";
    cin >> n;

    vector<int> arr(n), arr_copy(n);

    cout << "Enter " << n << " integers:\n";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    arr_copy = arr; // Copy for parallel sort

    // Sequential sort
    double start_seq = omp_get_wtime();
    sequentialBubbleSort(arr);
    double end_seq = omp_get_wtime();

    // Parallel sort
    double start_par = omp_get_wtime();
    parallelBubbleSort(arr_copy);
    double end_par = omp_get_wtime();

    // Output sorted arrays (optional)
    cout << "\nSorted array (Sequential): ";
    for (int num : arr) cout << num << " ";

    cout << "\nSorted array (Parallel):   ";
    for (int num : arr_copy) cout << num << " ";

    // Output performance
    cout << "\n\nTime taken by SEQUENTIAL Bubble Sort: " << (end_seq - start_seq) << " seconds";
    cout << "\nTime taken by PARALLEL Bubble Sort:   " << (end_par - start_par) << " seconds\n";

    return 0;
}
