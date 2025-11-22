#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// === Прототипи функцій ===
int allocateMatrix(int ***A, int N, int M);                  // Виділення пам'яті
void freeMatrix(int **A, int N);                             // Звільнення пам'яті
void fillMatrixManual(int **A, int N, int M);                // Ручне заповнення
void fillMatrixRandom(int **A, int N, int M);                // Випадкове заповнення
void printMatrix(int **A, int N, int M);                     // Виведення матриці
int countNegativesIterative(int **A, int N, int M, int *res);// Ітеративний підрахунок
int countNegativesRecursiveWrapper(int **A, int N, int M, int *res);// Рекурсивний підрахунок (з обгорткою)
int countNegativesRecursive(int **A, int N, int M, int *res, int i, int j);// Допоміжна рекурсивна

// === Основна функція програми ===
int main() {
    int N, M; // Розміри матриці
    int **A = NULL; // Вказівник на матрицю
    int *resIter = NULL, *resRec = NULL; // Масиви результатів
    int mode; // Режим заповнення
    int status = 0; // Код завершення

    // --- Введення розмірів ---
    printf("Введіть кількість рядків N: ");
    scanf("%d", &N);
    printf("Введіть кількість стовпців M: ");
    scanf("%d", &M);

    // --- Виділення пам'яті під матрицю ---
    if (allocateMatrix(&A, N, M) != 0) {
        printf("Помилка виділення пам'яті для матриці.\n");
        return 1;
    }

    // --- Вибір режиму заповнення ---
    printf("Оберіть режим заповнення (1 - вручну, 2 - випадково): ");
    scanf("%d", &mode);

    if (mode == 1)
        fillMatrixManual(A, N, M);
    else
        fillMatrixRandom(A, N, M);

    // --- Виведення введеної матриці ---
    printf("\nМатриця A[%d][%d]:\n", N, M);
    printMatrix(A, N, M);

    // --- Виділення пам'яті під результати ---
    resIter = (int *)calloc(N, sizeof(int));
    resRec = (int *)calloc(N, sizeof(int));
    if (!resIter || !resRec) {
        printf("Помилка виділення пам'яті для результатів.\n");
        freeMatrix(A, N);
        return 1;
    }

    // --- Ітеративний підрахунок ---
    countNegativesIterative(A, N, M, resIter);

    // --- Рекурсивний підрахунок ---
    // Обнулення результатів
    for (int i = 0; i < N; i++)
        resRec[i] = 0;

    // Старт рекурсії з [0][0]
    countNegativesRecursive(A, N, M, resRec, 0, 0);

    // --- Виведення результатів ---
    printf("\nКількість від’ємних елементів у кожному рядку (ітеративно):\n");
    for (int i = 0; i < N; i++)
        printf("Рядок %d: %d\n", i + 1, resIter[i]);

    printf("\nКількість від’ємних елементів у кожному рядку (рекурсивно):\n");
    for (int i = 0; i < N; i++)
        printf("Рядок %d: %d\n", i + 1, resRec[i]);

    // --- Перевірка еквівалентності ---
    int equivalent = 1;
    for (int i = 0; i < N; i++) {
        if (resIter[i] != resRec[i]) {
            equivalent = 0;
            break;
        }
    }

    if (equivalent)
        printf("\n✅ Результати обох варіантів еквівалентні.\n");
    else
        printf("\n❌ Результати відрізняються!\n");

    // --- Звільнення пам'яті ---
    free(resIter);
    free(resRec);
    freeMatrix(A, N);

    return status;
}

// === Реалізація функцій ===

// Виділення пам’яті під матрицю
int allocateMatrix(int ***A, int N, int M) {
    *A = (int **)malloc(N * sizeof(int *));
    if (*A == NULL) return 1;

    for (int i = 0; i < N; i++) {
        (*A)[i] = (int *)malloc(M * sizeof(int));
        if ((*A)[i] == NULL) return 1;
    }
    return 0;
}

// Звільнення пам’яті
void freeMatrix(int **A, int N) {
    for (int i = 0; i < N; i++)
        free(A[i]);
    free(A);
}

// Ручне заповнення матриці
void fillMatrixManual(int **A, int N, int M) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++) {
            printf("A[%d][%d] = ", i, j);
            scanf("%d", &A[i][j]);
        }
}

// Випадкове заповнення матриці
void fillMatrixRandom(int **A, int N, int M) {
    srand(time(NULL));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            A[i][j] = rand() % 21 - 10; // Діапазон [-10, 10]
}

// Виведення матриці на екран
void printMatrix(int **A, int N, int M) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++)
            printf("%4d", A[i][j]);
        printf("\n");
    }
}

// Ітеративний підрахунок
int countNegativesIterative(int **A, int N, int M, int *res) {
    for (int i = 0; i < N; i++) {
        int count = 0;
        for (int j = 0; j < M; j++)
            if (A[i][j] < 0)
                count++;
        res[i] = count; // Передача результату через вказівник
    }
    return 0;
}

int countNegativesRecursive(int **A, int N, int M, int *res, int i, int j)
{
    if (i >= N)               // База рекурсії — матриця закінчилася
        return 0;

    if (A[i][j] < 0)          // Лічимо від’ємні елементи
        res[i]++;

    if (j + 1 < M)            // Переходимо до наступного стовпця
        return countNegativesRecursive(A, N, M, res, i, j + 1);

    return countNegativesRecursive(A, N, M, res, i + 1, 0);
}
