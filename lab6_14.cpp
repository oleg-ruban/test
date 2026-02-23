#include <iostream>

using namespace std;

int recursiveCalls = 0;

int maxDigit(unsigned long long n) {
    recursiveCalls++;  // рахуємо виклик

    // Базовий випадок: якщо число однозначне
    if (n < 10) {
        return n;
    }

    // Рекурсивний випадок
    int lastDigit = n % 10;
    int maxOfRest = maxDigit(n / 10);

    return (lastDigit > maxOfRest) ? lastDigit : maxOfRest;
}

int main() {
    unsigned long long N;

    cout << "Enter natural number N: ";
    cin >> N;

    int result = maxDigit(N);

    cout << "Maximum digit: " << result << endl;
    cout << "Recursive calls: " << recursiveCalls << endl;

    return 0;
}