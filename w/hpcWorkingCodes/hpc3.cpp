#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <limits.h>

// Parallel Min
void parallel_reduce_min(int* data, int size, int* min_val) {
    int local_min = data[0];

    #pragma omp parallel for reduction(min:local_min)
    for (int i = 0; i < size; i++) {
        if (data[i] < local_min)
            local_min = data[i];
    }

    *min_val = local_min;
}

// Parallel Max
void parallel_reduce_max(int* data, int size, int* max_val) {
    int local_max = data[0];

    #pragma omp parallel for reduction(max:local_max)
    for (int i = 0; i < size; i++) {
        if (data[i] > local_max)
            local_max = data[i];
    }

    *max_val = local_max;
}

// Parallel Sum
void parallel_reduce_sum(int* data, int size, int* sum_val) {
    int local_sum = 0;

    #pragma omp parallel for reduction(+:local_sum)
    for (int i = 0; i < size; i++) {
        local_sum += data[i];
    }

    *sum_val = local_sum;
}

// Parallel Average (calls reduce_sum and divides)
void parallel_reduce_avg(int* data, int size, float* avg_val) {
    int sum = 0;
    parallel_reduce_sum(data, size, &sum);
    *avg_val = (float)sum / size;
}

int main() {
    int size;
    printf("Enter number of elements: ");
    scanf("%d", &size);

    int* data = (int*)malloc(size * sizeof(int));
    if (data == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    printf("Enter %d elements:\n", size);
    for (int i = 0; i < size; i++) {
        scanf("%d", &data[i]);
    }

    int min_val, max_val, sum_val;
    float avg_val;

    parallel_reduce_min(data, size, &min_val);
    parallel_reduce_max(data, size, &max_val);
    parallel_reduce_sum(data, size, &sum_val);
    parallel_reduce_avg(data, size, &avg_val);

    printf("\nResults using Parallel Reduction (OpenMP):\n");
    printf("Minimum: %d\n", min_val);
    printf("Maximum: %d\n", max_val);
    printf("Sum: %d\n", sum_val);
    printf("Average: %.2f\n", avg_val);

    free(data);
    return 0;
}
