#include <iostream>
#include <string>
#include <vector>
#include <cmath>

/**
 * @param text  Основний текст для пошуку
 * @param pattern Зразок, який шукаємо
 * @return Індекс першого входження або -1
 */
int rabinKarp(const std::string& text, const std::string& pattern) {
    int n = text.length();
    int m = pattern.length();

    if (m == 0) return 0;
    if (m > n) return -1;

    const int d = 256;      // Кількість символів в алфавіті (ASCII)
    const int q = 101;      // Просте число для модуля
    int h = 1;              // Значення d^(m-1) % q
    int p = 0;              // Хеш зразка
    int t = 0;              // Хеш поточного вікна тексту

    // Обчислюємо h = pow(d, m-1) % q
    for (int i = 0; i < m - 1; i++) {
        h = (h * d) % q;
    }

    // Обчислюємо початкові хеші для зразка та першого вікна тексту
    for (int i = 0; i < m; i++) {
        p = (d * p + pattern[i]) % q;
        t = (d * t + text[i]) % q;
    }

    // Проходимо по тексту за допомогою "ковзного вікна"
    for (int i = 0; i <= n - m; i++) {
        // Якщо хеші збігаються, перевіряємо символи по черзі
        if (p == t) {
            bool match = true;
            for (int j = 0; j < m; j++) {
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            if (match) return i;
        }

        // Обчислюємо хеш для наступного вікна:
        // Видаляємо перший символ, додаємо наступний
        if (i < n - m) {
            t = (d * (t - text[i] * h) + text[i + m]) % q;

            // Якщо t стало від'ємним, перетворюємо його на додатне
            if (t < 0) t = (t + q);
        }
    }

    return -1;
}

int main() {
    std::string text = "Hello, this is a simple example.";
    std::string pattern = "simple";

    int result = rabinKarp(text, pattern);

    if (result != -1) {
        std::cout << "Зразок знайдено за індексом: " << result << std::endl;
    } else {
        std::cout << "Зразок не знайдено." << std::endl;
    }

    return 0;
}