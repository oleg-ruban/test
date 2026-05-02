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
    if (!inFile.is_open()) {
        std::cerr << "Помилка: не вдалося відкрити файл '" << inName << "' для читання.\n";
        return;
    }

    std::ofstream outFile(outName);
    if (!outFile.is_open()) {
        std::cerr << "Помилка: не вдалося створити файл '" << outName << "'.\n";
        return;
    }

    std::string token;
    int successCount = 0;
    int errorCount = 0;
    // Цикл зчитує файл слово за словом (число за числом),
    // ігноруючи будь-яку кількість пробілів або переносів рядків.
    while (inFile >> token) {
        if (isValidNumber(token, from)) {
            std::string result = convertBase(token, from, to);
            outFile << result << "\n";
            successCount++;
        } else {
            std::cerr << "Пропущено некоректне число для бази " << from << ": " << token << "\n";
            errorCount++;
        }
    }

    inFile.close();
    outFile.close();

    std::cout << "\n--- Обробка завершена ---\n";
    std::cout << "Успішно конвертовано: " << successCount << "\n";
    if (errorCount > 0) {
        std::cout << "Знайдено помилок у числах: " << errorCount << " (див. вище)\n";
    }
    std::cout << "Результати збережено в: " << outName << "\n";
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
    std::cout << "\n======================================================\n"
              << "ПРОГРАМА КОНВЕРТАЦІЇ ВЕЛИКИХ ЧИСЕЛ (GMP)\n"
              << "======================================================\n"
              << "Призначення: переведення цілих чисел між системами числення\n"
              << "від 2 до 16. Завдяки бібліотеці GMP підтримуються числа\n"
              << "будь-якої довжини.\n\n"
              << "ОПИС РЕЖИМІВ:\n"
              << "1. Файловий режим: зчитує числа з текстового файлу (кожен\n"
              << "   токен через пробіл/рядок) та записує результат у файл.\n"
              << "2. Консольний режим: швидке переведення чисел, введених\n"
              << "   користувачем безпосередньо в терміналі.\n"
              << "3. Довідка: виведення цієї інформації.\n"
              << "======================================================\n";
}

int main() {
    clearTerminal(); // Початкове очищення екрану
    showHelp();      // Початковий help
    std::string choice;
    while (true) {
        std::cout << "\nМеню:\n1. Файловий режим\n2. Консольний режим\n3. Довідка\n4. Вихід\n> ";
        std::cin >> choice;

        if (choice == "1") runFileMode();
        else if (choice == "2") runConsoleMode();
        else if (choice == "3"){
            clearTerminal();
            showHelp();
        }
        else if (choice == "4") break;
        else std::cout << "Невірний вибір.\n";
    }
    return 0;
}