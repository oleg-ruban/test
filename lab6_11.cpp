#include <iostream>
#include <ctime>

using namespace std;

const int N = 1000;

// 1.2 Bubble Sort
void bubbleSort(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// 1.3 Linear Search
int linearSearch(int arr[], int size, int key) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == key)
            return i;
    }
    return -1;
}

// 1.4 Binary Search (масив повинен бути відсортований!)
int binarySearch(int arr[], int size, int key) {
    int left = 0;
    int right = size - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == key)
            return mid;
        else if (arr[mid] < key)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return -1;
}

int main() {
    int arr[N];
    int arrCopy[N];

    srand(time(0));

    // 1.1 Заповнення масиву випадковими числами
    for (int i = 0; i < N; i++) {
        arr[i] = rand() % 4001 - 2000; // від -2000 до 2000
        arrCopy[i] = arr[i];
    }

    // 1.2 Сортування бульбашкою
    bubbleSort(arr, N);

    const int TEST_COUNT = 10000;
    clock_t startTime, endTime;
    double seconds;

    // ================================
    // Тестування ЛІНІЙНОГО ПОШУКУ
    // ================================
    startTime = clock();

    for (int i = 0; i < TEST_COUNT; i++) {
        int key = rand() % 4001 - 2000;
        linearSearch(arrCopy, N, key);
    }

    endTime = clock();
    seconds = (double)(endTime - startTime) / CLOCKS_PER_SEC;

    cout << "Linear search time: " << seconds << " seconds" << endl;

    // ================================
    // Тестування БІНАРНОГО ПОШУКУ
    // ================================
    startTime = clock();

    for (int i = 0; i < TEST_COUNT; i++) {
        int key = rand() % 4001 - 2000;
        binarySearch(arr, N, key);
    }

    endTime = clock();
    seconds = (double)(endTime - startTime) / CLOCKS_PER_SEC;

    cout << "Binary search time: " << seconds << " seconds" << endl;

    return 0;
}
