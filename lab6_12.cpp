#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

const int N = 200;

// Функція обчислення суми цифр числа
int sumOfDigits(int number) {
    int sum = 0;
    while (number > 0) {
        sum += number % 10;
        number /= 10;
    }
    return sum;
}

// Перетасовка масиву алгоритмом Фішера-Йєтса
void fisherYatesShuffle(int arr[], int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(arr[i], arr[j]);
    }
}

// Сортування вибором за заданим критерієм
void selectionSort(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        int minIndex = i;

        for (int j = i + 1; j < size; j++) {
            int sumMin = sumOfDigits(arr[minIndex]);
            int sumCurrent = sumOfDigits(arr[j]);

            if (sumCurrent < sumMin ||
               (sumCurrent == sumMin && arr[j] < arr[minIndex])) {
                minIndex = j;
               }
        }

        if (minIndex != i) {
            swap(arr[i], arr[minIndex]);
        }
    }
}

int main() {
    int arr[N];

    // Заповнення масиву числами 1..N
    for (int i = 0; i < N; i++) {
        arr[i] = i + 1;
    }

    srand(time(0)); // Ініціалізація генератора випадкових чисел

    // Перетасовка
    fisherYatesShuffle(arr, N);

    // Замір часу сортування
    clock_t startTime = clock();

    selectionSort(arr, N);

    clock_t endTime = clock();
    double seconds = (double)(endTime - startTime) / CLOCKS_PER_SEC;

    // Вивід відсортованого масиву
    cout << "Sorted array:\n";
    for (int i = 0; i < N; i++) {
        cout << arr[i] << " ";
    }

    cout << "\n\nExecution time: " << seconds << " seconds\n";

    return 0;
}
