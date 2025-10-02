#include <stdio.h>
#include <math.h>

double exp_series(double x, double eps, int *iterations) {
    double term = 1.0;   // первый член ряда: x^0 / 0! = 1
    double sum = 0.0;
    int n = 0;

    while (fabs(term) >= eps) {
        sum += term;        // добавляем член ряда
        n++;
        term *= x / n;      // рекуррентная формула: a_n = a_{n-1} * (x / n)
    }

    *iterations = n;        // записываем количество шагов
    return sum;
}

int main() {
    double x, eps, result;
    int iterations;

    printf("Введите x: ");
    scanf("%lf", &x);

    printf("Введите точность eps: ");
    scanf("%lf", &eps);

    result = exp_series(x, eps, &iterations);

    printf("Значение e^%.2f ≈ %.10f\n", x, result);
    printf("Количество итераций: %d\n", iterations);

    return 0;
}
