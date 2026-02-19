#include <iostream>
#include <ctime>

using namespace std;

const int N = 200;

// --------------------
// Рекурсивна сума цифр
// --------------------
int sumOfDigits(int number) {
    if (number == 0)
        return 0;
    return number % 10 + sumOfDigits(number / 10);
}

// --------------------------------------
// Рекурсивний пошук мінімального елемента
// --------------------------------------
int findMinIndex(int arr[], int size, int start, int currentMin) {
    if (start >= size)
        return currentMin;

    int sumStart = sumOfDigits(arr[start]);
    int sumMin = sumOfDigits(arr[currentMin]);

    if (sumStart < sumMin ||
       (sumStart == sumMin && arr[start] < arr[currentMin]))
        currentMin = start;

    return findMinIndex(arr, size, start + 1, currentMin);
}

// --------------------------------------
// Рекурсивне сортування вибором
// --------------------------------------
void recursiveSelectionSort(int arr[], int size, int index = 0) {
    if (index >= size - 1)
        return;

    int minIndex = findMinIndex(arr, size, index + 1, index);

    if (minIndex != index)
        swap(arr[index], arr[minIndex]);

    recursiveSelectionSort(arr, size, index + 1);
}

// --------------------
// Головна функція
// --------------------
int main() {
    int arr[N];

    // Заповнення масиву числами 1..N
    for (int i = 0; i < N; i++)
        arr[i] = i + 1;

    // Замір часу
    clock_t startTime = clock();

    recursiveSelectionSort(arr, N);

    clock_t endTime = clock();

    double seconds = (double)(endTime - startTime) / CLOCKS_PER_SEC;

    // Вивід відсортованого масиву
    cout << "Sorted array:\n";
    for (int i = 0; i < N; i++)
        cout << arr[i] << " ";

    cout << "\n\nExecution time: " << seconds << " seconds\n";

    return 0;
}
