#include <stdio.h>
#include <math.h>

// Глобальні константи
const double a = 0.816;
const double b = 3.4;
const double c = 16.7;

// Функція для обчислення y
double f(double x, double a, double c) {
    double u = sin((x * x + 1) / c);
    if (u > 0) {
        return a * u;
    } else {
        return cos(x + 1.0 / c);
    }
}

int main() {
    printf(" x\t u\t\t y\n");
    printf("-------------------------------\n");

    for (int x = 1; x <= 10; x++) {
        double u = sin((x * x + 1) / c);
        double y;

        if (u > 0) {
            y = a * u;
        } else {
            y = cos(x + 1.0 / c);
        }

        printf("%2d\t%.6f\t%.6f\n", x, u, y);
    }

    return 0;
}
