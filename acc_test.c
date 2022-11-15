#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

void arrgen(int arr[], int len) {
    srand(time(NULL));
    for (int i = 0; i < len; i++)
        arr[i] = rand() % 50000;
}

void arrcpy(int destination[], int source[], int len) {
    for (int i = 0; i < len; i++)
        destination[i] = source[i];
}

void swap_next(int arr[], int index) {
    int tmp = arr[index];
    arr[index] = arr[index + 1];
    arr[index + 1] = tmp;
}

void sequential_bubble_sort(int arr[], int len) {
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len - 1; j++) {
            if (arr[j] > arr[j+1])
                swap_next(arr, j);
        }
    }
}

void cpu_parallelized_bubble_sort(int arr[], int len) {
    for (int i = 0; i < len; i++) {
        #pragma omp parallel for num_threads(24)
        for (int j = i % 2; j < len - 1; j += 2) {
            if (arr[j] > arr[j + 1])
                swap_next(arr, j);
        }
    }
}

void gpu_parallelized_bubble_sort(int arr[], int len) {
    for (int i = 0; i < len; i++) {
        #pragma acc parallel loop
        for (int j = i % 2; j < len - 1; j += 2) {
            if (arr[j] > arr[j + 1])
                swap_next(arr, j);
        }
    }
}

double elapsed_s(struct timeval start, struct timeval end) {
    long time_s = end.tv_sec - start.tv_sec;
    long time_us = end.tv_usec - start.tv_usec;
    return time_s + time_us * 1e-6;
}

int main() {
    int num_elements = 100000;
    int arr1[num_elements];
    int arr2[num_elements];
    int arr3[num_elements];

    arrgen(arr1, num_elements);
    arrcpy(arr2, arr1, num_elements);
    arrcpy(arr3, arr1, num_elements);

    printf("Sorting array of %d elements...\n", num_elements);

    struct timeval start, end;
    double elapsed;
    
    gettimeofday(&start, 0);
    sequential_bubble_sort(arr1, num_elements);
    gettimeofday(&end, 0);
    elapsed = elapsed_s(start, end);

    printf("[Sequential] Ran in %.3lfs\n", elapsed);

    gettimeofday(&start, 0);
    cpu_parallelized_bubble_sort(arr2, num_elements);
    gettimeofday(&end, 0);
    elapsed = elapsed_s(start, end);

    printf("[CPU Parallelized] Ran in %.3lfs\n", elapsed);

    gettimeofday(&start, 0);
    gpu_parallelized_bubble_sort(arr3, num_elements);
    gettimeofday(&end, 0);
    elapsed = elapsed_s(start, end);

    printf("[GPU Parallelized] Ran in %.3lfs\n", elapsed);

    return 0;
}
