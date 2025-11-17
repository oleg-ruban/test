#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX 100

typedef struct {
    char surname[50];
    char name[50];
    char group[10];
    float avg;
} Student;


// Перевірка голосної
int is_vowel(char c) {
    char v[] = "aeiouy";
    for (int i = 0; v[i]; i++)
        if (c == v[i]) return 1;
    return 0;
}


// Нормалізація ASCII-рядка (нижній регістр, видалення пробілів, дефісів та ASCII-апострофів)
void normalize(const char *src, char *dst) {
    int j = 0;
    for (int i = 0; src[i]; i++) {
        unsigned char c = tolower((unsigned char)src[i]);

        if (c == ' ' || c == '-' || c == '\'' || c == '`')
            continue;

        dst[j++] = c;
    }
    dst[j] = '\0';
}


// Підрахунок приголосних у імені
int count_consonants(const char *name) {
    int cnt = 0;
    for (int i = 0; name[i]; i++) {
        char c = name[i];
        if (isalpha(c) && !is_vowel(c))
            cnt++;
    }
    return cnt;
}


// Частка голосних у прізвищі
double vowel_share(const char *surname) {
    int v = 0, total = 0;

    for (int i = 0; surname[i]; i++) {
        char c = surname[i];
        if (isalpha(c)) {
            total++;
            if (is_vowel(c)) v++;
        }
    }
    return total ? (double)v / total : 0.0;
}


// Підрахунок для однієї літери
void count_for_letter(const char *surname, char letter, int *count, int *first, int *last) {
    *count = 0;
    *first = -1;
    *last = -1;

    for (int i = 0; surname[i]; i++) {
        if (surname[i] == letter) {
            (*count)++;
            if (*first == -1) *first = i;
            *last = i;
        }
    }
}


// Виведення статистики для кожної літери імені
void print_letter_stats(const char *surname, const char *name) {

    printf(": ");

    for (int i = 0; name[i]; i++) {

        char letter = name[i];
        int cnt, first, last;

        count_for_letter(surname, letter, &cnt, &first, &last);

        if (cnt == 0)
            printf("%c/%d[-,-] ", letter, cnt);
        else
            printf("%c/%d[%d,%d] ", letter, cnt, first, last);
    }

    printf("\n");
}


// Заповнення вручну
void fill_manual(Student *s, int n) {
    printf("Введіть записи у форматі: Прізвище Ім'я Група Бал\n");

    for (int i = 0; i < n; i++) {
        printf("Студент %d: ", i + 1);
        scanf("%s %s %s %f", s[i].surname, s[i].name, s[i].group, &s[i].avg);
    }
}


// Перевірка унікальності
int unique(Student *arr, int size, Student s) {
    for (int i = 0; i < size; i++)
        if (strcmp(arr[i].surname, s.surname) == 0 &&
            strcmp(arr[i].name, s.name) == 0 &&
            strcmp(arr[i].group, s.group) == 0)
            return 0;

    return 1;
}


// Заповнення випадково
void fill_random(Student *s, int n) {

    char surnames[][20] = {"Ruban", "Petrenko", "Shevchenko", "Bondar"};
    char names[][20]     = {"Maria", "Oleg", "Mykola", "Serhii"};
    char groups[][10]    = {"KA-5","KA-52","KA-53","KA-54"};

    int ns = 4, nn = 4, ng = 4;

    for (int i = 0; i < n; ) {

        Student tmp;
        strcpy(tmp.surname, surnames[rand() % ns]);
        strcpy(tmp.name, names[rand() % nn]);
        strcpy(tmp.group, groups[rand() % ng]);
        tmp.avg = 60 + rand() % 41;

        if (unique(s, i, tmp)) {
            s[i] = tmp;
            i++;
        }
    }
}


// Пошук групи з найкращим середнім балом
char* best_group(Student *s, int n) {

    static char best[10];
    float sum[100] = {0};
    int count[100] = {0};
    char groups[100][10];

    int gcount = 0;

    for (int i = 0; i < n; i++) {

        int found = -1;

        for (int j = 0; j < gcount; j++)
            if (strcmp(groups[j], s[i].group) == 0)
                found = j;

        if (found == -1) {
            strcpy(groups[gcount], s[i].group);
            sum[gcount] += s[i].avg;
            count[gcount]++;
            gcount++;
        } else {
            sum[found] += s[i].avg;
            count[found]++;
        }
    }

    float bestAvg = -1;

    for (int i = 0; i < gcount; i++) {
        float cur = sum[i] / count[i];
        if (cur > bestAvg) {
            bestAvg = cur;
            strcpy(best, groups[i]);
        }
    }

    return best;
}


// Основна обробка студентів
void process(Student *s, int n) {

    char normSurname[50], normName[50];

    for (int i = 0; i < n; i++) {

        printf("\n%s %s", s[i].surname, s[i].name);

        normalize(s[i].surname, normSurname);
        normalize(s[i].name, normName);

        print_letter_stats(normSurname, normName);

        printf("  Частка голосних у прізвищі: %.2f\n",
               vowel_share(normSurname));

        printf("  Приголосних у імені: %d\n",
               count_consonants(normName));
    }
}


int main() {

    int n, mode;

    printf("Кількість студентів: ");
    scanf("%d", &n);

    Student *arr = malloc(n * sizeof(Student));

    printf("Спосіб заповнення (1 — вручну, 2 — випадково): ");
    scanf("%d", &mode);

    if (mode == 1)
        fill_manual(arr, n);
    else
        fill_random(arr, n);

    printf("\n=== Обробка ===\n");
    process(arr, n);

    printf("\nГрупа з найкращим середнім балом: %s\n",
           best_group(arr, n));

    free(arr);
    return 0;
}
