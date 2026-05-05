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

// --- Освітній режим ---
void runStepByStepMode() {
    std::string input;
    int from, to;
    std::cout << "\n--- КРОК ЗА КРОКОМ (Освітній режим) ---\n";
    std::cout << "Введіть вхідну базу: "; std::cin >> from;
    std::cout << "Введіть вихідну базу: "; std::cin >> to;
    std::cout << "Введіть число: "; std::cin >> input;

    if (!isValidNumber(input, from)) {
        std::cout << "Помилка: Некоректне число для цієї бази.\n";
        return;
    }

    mpz_t n, quotient, remainder, baseTo;
    mpz_inits(n, quotient, remainder, baseTo, NULL);
    mpz_set_str(n, input.c_str(), from);
    mpz_set_ui(baseTo, to);

    std::cout << "\nАлгоритм: Ділимо число на основу " << to << " і записуємо остачі.\n";
    std::cout << "Початкове число в десятковій системі: " << mpz_get_str(NULL, 10, n) << "\n\n";

    std::string finalResult = "";
    int step = 1;

    while (mpz_cmp_ui(n, 0) > 0) {
        mpz_fdiv_qr(quotient, remainder, n, baseTo);

        char* q_str = mpz_get_str(NULL, 10, quotient);
        char* r_str = mpz_get_str(NULL, to, remainder); // Цифра в цільовій системі
        char* n_str = mpz_get_str(NULL, 10, n);

        if (step <= 15) { // Обмежуємо вивід для дуже великих чисел
            std::cout << "Крок " << step << ": " << n_str << " / " << to
                      << " = " << q_str << ", остача: [ " << r_str << " ]\n";
        } else if (step == 16) {
            std::cout << "... (далі кроки виконуються аналогічно для великого числа) ...\n";
        }

        finalResult += r_str;
        mpz_set(n, quotient);
        step++;

        free(q_str); free(r_str); free(n_str);
    }

    std::reverse(finalResult.begin(), finalResult.end());
    std::cout << "\nЗбираємо остачі у зворотному порядку: " << finalResult << "\n";
    std::cout << "Результат: " << finalResult << " (база " << to << ")\n";

    mpz_clears(n, quotient, remainder, baseTo, NULL);
}

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
              << "3. Крок за кроком: пояснення алгоритму переведення.\n"
              << "4. Довідка: виведення цієї інформації.\n"
              << "======================================================\n";
}

int main() {
    clearTerminal(); // Початкове очищення екрану
    showHelp();      // Початковий help
    std::string choice;
    while (true) {
        std::cout << "\nМеню:\n1. Файловий режим\n2. Консольний режим\n3. Крок за кроком\n4. Довідка\n5. Вихід\n> ";
        std::cin >> choice;

        if (choice == "1") runFileMode();
        else if (choice == "2") runConsoleMode();
        else if (choice == "3") runStepByStepMode();
        else if (choice == "4"){
            clearTerminal();
            showHelp();
        }
        else if (choice == "5") break;
        else std::cout << "Невірний вибір.\n";
    }
    return 0;
}