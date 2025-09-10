#include <stdio.h>
#include <math.h>

int main() {
    int task;
    printf("Оберіть завдання (1 або 2): ");
    scanf("%d", &task);

    if (task == 1) {
        double a = 0.816, b = 3.4, c = 16.7;
        char choice;

        printf("Використати значення за замовчуванням a=0.816, b=3.4, c=16.7? (y/n): ");
        scanf(" %c", &choice);

        if (choice == 'n' || choice == 'N') {
            printf("Введіть a: ");
            scanf("%lf", &a);
            printf("Введіть b: ");
            scanf("%lf", &b);
            printf("Введіть c: ");
            scanf("%lf", &c);
        }

        // Перевірка області визначення
        if ((a + b) <= 0) {
            printf("Помилка: підкореневий вираз (a+b) має бути > 0.\n");
            return 1;
        }
        if (fabs(a) < 1e-12 || fabs(c) < 1e-12) {
            printf("Помилка: a і c не можуть дорівнювати нулю (e^(a*c)-1 = 0).\n");
            return 1;
        }

        double x = pow(a, 2) * pow(tan((a + b) * (a + b)), 2) + c / sqrt(a + b);
        double y = (b * pow(a, 2) - c) / (exp(a * c) - 1);

        printf("Результат:\n");
        printf("x = %.6f\n", x);
        printf("y = %.6f\n", y);
    }
    else if (task == 2) {
        double a = 0.3, c = 10.0;
        char choice;

        printf("Використати значення за замовчуванням a=0.3, c=10? (y/n): ");
        scanf(" %c", &choice);

        if (choice == 'n' || choice == 'N') {
            printf("Введіть a: ");
            scanf("%lf", &a);
            printf("Введіть c: ");
            scanf("%lf", &c);
        }

        // Перевірка області визначення
        if (fabs(c) < 1e-12) {
            printf("Помилка: c не може дорівнювати нулю (ділення на нуль).\n");
            return 1;
        }

        printf("Результати обчислень:\n");
        for (int x = 1; x <= 10; x++) {
            double expr = sin((pow(x, 2) + 1) / c);
            double y;

            if (expr > 0) {
                y = a * expr;
            } else {
                y = cos(x + 1.0 / c);
            }
            printf("x = %2d  ->  y = %.6f\n", x, y);
        }
    }
    else {
        printf("Невірний номер завдання.\n");
    }

    return 0;
}
