#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int N, choice;
    double **A;
    double maxBelow = -1e9; // Дуже мале число
    double sum = 0;
    int found = 0;

    printf("Введіть розмірність матриці N: ");
    scanf("%d", &N);

    // Динамічне виділення пам’яті
    A = (double **)malloc(N * sizeof(double *));
    for (int i = 0; i < N; i++) {
        A[i] = (double *)malloc(N * sizeof(double));
    }

    // Вибір способу заповнення
    printf("Оберіть спосіб заповнення (1 - вручну, 2 - випадково): ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Введіть елементи матриці:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                scanf("%lf", &A[i][j]);
            }
        }
    } else {
        srand(time(NULL));
        printf("Матриця згенерована випадково:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i][j] = (rand() % 200 - 100) / 10.0; // від -10.0 до 10.0
                printf("%6.2f ", A[i][j]);
            }
            printf("\n");
        }
    }

    // Знаходимо максимальний елемент нижче головної діагоналі
    for (int i = 1; i < N; i++) {
        for (int j = 0; j < i; j++) {
            if (A[i][j] > maxBelow) {
                maxBelow = A[i][j];
            }
        }
    }

    // Обчислення суми
    for (int i = 0; i < N; i++) {
        for (int j = i; j < N; j++) { // тільки головна діагональ і вище
            if (A[i][j] > maxBelow) {
                sum += A[i][j];
                found = 1;
            }
        }
    }

    // Вивід результату
    if (found)
        printf("Сума елементів = %.2f\n", sum);
    else
        printf("Елементів з такою властивістю не знайдено.\n");

    // Звільнення пам’яті
    for (int i = 0; i < N; i++) {
        free(A[i]);
    }
    free(A);

    return 0;
}