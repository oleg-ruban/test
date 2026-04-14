#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <sstream>  // Додано для stringstream
#include <cstring>  // Додано для strlen
#include <gmp.h>

namespace fs = std::filesystem;

// --- Допоміжні функції ---

void clearTerminal() {
#if defined(_WIN32)
    system("cls");
#else
    system("clear");
#endif
}

// Отримання директорії, де розташований файл, що запускається
fs::path getAppDirectory() {
    return fs::current_path(); 
}

// Перевірка, чи символ відповідає системі числення
bool isValidDigit(char ch, int base) {
    ch = std::tolower(ch);
    int value = -1;
    if (std::isdigit(ch)) value = ch - '0';
    else if (ch >= 'a' && ch <= 'f') value = ch - 'a' + 10;
    return (value >= 0 && value < base);
}

bool isValidNumber(const std::string& str, int base) {
    if (str.empty()) return false;
    size_t start = (str[0] == '-') ? 1 : 0;
    if (start == str.length()) return false;
    for (size_t i = start; i < str.length(); ++i) {
        if (!isValidDigit(str[i], base)) return false;
    }
    return true;
}

// --- Ядро конвертації ---

std::string convertBase(const std::string& input, int fromBase, int toBase) {
    mpz_t number;
    mpz_init(number);
    
    // Встановлюємо значення (GMP сам обробляє знак '-')
    mpz_set_str(number, input.c_str(), fromBase);
    
    // Отримуємо результат
    char* buffer = mpz_get_str(nullptr, toBase, number);
    std::string result(buffer);
    
    // Очищення
    void (*freefunc)(void *, size_t);
    mp_get_memory_functions(nullptr, nullptr, &freefunc);
    freefunc(buffer, std::strlen(buffer) + 1);
    mpz_clear(number);
    
    return result;
}

// --- Режими роботи ---

void runFileMode() {
    int from, to;
    std::string inName, outName;

    std::cout << "Введіть вхідну та вихідну бази (напр. 10 16): ";
    std::cin >> from >> to;
    std::cout << "Ім'я вхідного файлу: "; std::cin >> inName;
    std::cout << "Ім'я вихідного файлу: "; std::cin >> outName;

    std::ifstream inFile(inName);
    std::ofstream outFile(outName);

    if (!inFile || !outFile) {
        std::cerr << "Помилка відкриття файлів!\n";
        return;
    }

    std::string token;
    int count = 0;
    while (inFile >> token) {
        if (isValidNumber(token, from)) {
            outFile << convertBase(token, from, to) << "\n";
            count++;
        }
    }
    std::cout << "Успішно оброблено чисел: " << count << "\n";
}

void runConsoleMode() {
    int from, to;
    std::cout << "Введіть вхідну та вихідну бази (напр. 2 10): ";
    std::cin >> from >> to;
    std::cin.ignore();

    std::cout << "Введіть числа через пробіл (Enter для завершення):\n> ";
    std::string line;
    std::getline(std::cin, line);
    std::stringstream ss(line);
    std::string token;

    std::cout << "\nРезультати:\n";
    while (ss >> token) {
        if (isValidNumber(token, from)) {
            std::cout << token << " (" << from << ") -> " 
                      << convertBase(token, from, to) << " (" << to << ")\n";
        } else {
            std::cout << token << " -> [Помилка: некоректне число]\n";
        }
    }
}

// --- Головне меню ---

void showHelp() {
    std::cout << "\n=== Програма конвертації чисел ===\n"
              << "Підтримуються бази від 2 до 16.\n"
              << "Для великих чисел використовується бібліотека GMP.\n"
              << "Файли мають бути в кодуванні UTF-8/ASCII.\n";
}

int main() {
    std::string choice;
    while (true) {
        std::cout << "\nМеню:\n1. Файловий режим\n2. Консольний режим\n3. Довідка\n4. Вихід\n> ";
        std::cin >> choice;

        if (choice == "1") runFileMode();
        else if (choice == "2") runConsoleMode();
        else if (choice == "3") showHelp();
        else if (choice == "4") break;
        else std::cout << "Невірний вибір.\n";
    }
    return 0;
}