#include <iostream>
#include <cmath>
#include <ctime>

using namespace std;

// Функція f(x) = x^2 + ax + b
double f(double x, double a, double b) {
    return x * x + a * x + b;
}

// Тернарний пошук
double ternarySearch(double left, double right, double a, double b, double eps) {
    while (right - left > eps) {
        double m1 = left + (right - left) / 3.0;
        double m2 = right - (right - left) / 3.0;

        if (f(m1, a, b) < f(m2, a, b))
            right = m2;
        else
            left = m1;
    }
    return (left + right) / 2.0;
}

// Метод золотого перетину
double goldenSectionSearch(double left, double right, double a, double b, double eps) {
    const double phi = (1 + sqrt(5)) / 2.0;
    double x1 = right - (right - left) / phi;
    double x2 = left + (right - left) / phi;

    while (fabs(right - left) > eps) {
        if (f(x1, a, b) < f(x2, a, b)) {
            right = x2;
            x2 = x1;
            x1 = right - (right - left) / phi;
        } else {
            left = x1;
            x1 = x2;
            x2 = left + (right - left) / phi;
        }
    }
    return (left + right) / 2.0;
}

int main() {
    double a, b;
    cout << "Enter a and b: ";
    cin >> a >> b;

    double left = -100.0;
    double right = 100.0;
    double eps = 1e-7;

    // -------- Тернарний пошук --------
    clock_t startTime1 = clock();
    double minTernary = ternarySearch(left, right, a, b, eps);
    clock_t endTime1 = clock();
    double timeTernary = (double)(endTime1 - startTime1) / CLOCKS_PER_SEC;

    // -------- Золотий перетин --------
    clock_t startTime2 = clock();
    double minGolden = goldenSectionSearch(left, right, a, b, eps);
    clock_t endTime2 = clock();
    double timeGolden = (double)(endTime2 - startTime2) / CLOCKS_PER_SEC;

    cout << "\nTernary Search:\n";
    cout << "Minimum x = " << minTernary << endl;
    cout << "f(x) = " << f(minTernary, a, b) << ", where a=" << a << ", b=" << b << endl;
    cout << "Time = " << timeTernary << " seconds\n";

    cout << "\nGolden Section Search:\n";
    cout << "Minimum x = " << minGolden << endl;
    cout << "f(x) = " << f(minGolden, a, b) << ", where a=" << a << ", b=" << b << endl;
    cout << "Time = " << timeGolden << " seconds\n";

    return 0;
}
