#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int* gen_array(int* arr, size_t len, int min, int max) {
    for (size_t i = 0; i < len; i++)
        arr[i] = rand() % (max - min + 1) + min;
    return arr;
}

int* init_zero(int* arr, size_t len) {
    for (size_t i = 0; i < len; i++)
        arr[i] = 0;
    return arr;
}

int print_array(int* arr, size_t len) {
    size_t i;

    printf("[");
    for (i = 0; i < len - 1; i++)
        printf("%d ", arr[i]);
    printf("%d", arr[i]);
    printf("]");

    return i;
}

int wait_demo() {
    size_t len = 10;
    int max = len * 2;
    int A[len], B[len], C[len], D[len], E[len], F[len], G[len];

    printf("Generating initial arrays...\n");

    srand(time(NULL));
    gen_array(A, len, 0, max);
    gen_array(B, len, 0, max);
    gen_array(C, len, 0, max);
    gen_array(D, len, 0, max);

    printf("A: ");
    print_array(A, len);
    printf("\nB: ");
    print_array(B, len);
    printf("\nC: ");
    print_array(C, len);
    printf("\nD: ");
    print_array(D, len);

    printf("\n\nCalculating E...\n");

    #pragma acc parallel loop async(0)
    for (size_t i = 0; i < len; i++)
        E[i] = A[i] + B[i];

    printf("Calculating F...\n");

    #pragma acc parallel loop async(1)
    for (size_t i = 0; i < len; i++)
        F[i] = C[i] + D[i];

    #pragma acc wait

    printf("E: ");
    print_array(E, len);
    printf("\nF: ");
    print_array(F, len);

    printf("\n\nCalculating G...\n");

    #pragma acc parallel loop
    for (size_t i = 0; i < len; i++)
        G[i] = E[i] + F[i];

    printf("G: ");
    print_array(G, len);
    printf("\n");

    return 0;
}

int atom_demo() {
    size_t len = 10;
    int max_a = (len + 1) / 2;
    int max_b = (len - 1) / 2;
    int A[len], B[len], C[len];

    printf("Generating initial arrays...\n");

    srand(time(NULL));
    gen_array(A, len, 0, max_a);
    gen_array(B, len, 0, max_b);
    init_zero(C, len);

    printf("A: ");
    print_array(A, len);
    printf("\nB: ");
    print_array(B, len);

    printf("\n\nCalculating C...\n");

    #pragma acc parallel loop
    for (size_t i = 0; i < len; i++) {
        int index = A[i] + B[i];
        #pragma acc atomic update
        C[index]++;
    }

    printf("C: ");
    print_array(C, len);
    printf("\n");

    return 0;
}
