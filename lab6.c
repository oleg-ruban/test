#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// -------------------------------
//        ПРЕПРОЦЕСОР
// -------------------------------
#define MAX_STUDENTS 200
#define MAX_NAME     64
#define TXT_FILE     "students.txt"
#define BIN_FILE     "students.bin"

// Макроси
#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))
#define SWAP(a,b) { typeof(a) _t=a; a=b; b=_t; }

#define IS_VOWEL(c) ( strchr("aeiouyAEIOUY", (c)) != NULL )

// Логування (активне тільки якщо визначено VERBOSE)
#ifdef VERBOSE
    #define LOG(...)  printf(__VA_ARGS__)
#else
    #define LOG(...)
#endif

// -------------------------------
//        СТРУКТУРИ
// -------------------------------

typedef struct {
    char surname[MAX_NAME];
    char name[MAX_NAME];
    char group[16];
    float avg;
} Student;

// -------------------------------
//        ПРОТОТИПИ
// -------------------------------

void menu();
void lab5(Student *arr, int *count);
void lab6();
void save_menu(Student *arr, int count);

void fill_manual(Student *s, int n);
void fill_random(Student *s, int n);
int  unique(Student *arr, int size, Student s);
char* best_group(Student *s, int n);

void normalize(const char *src, char *dst);
int count_consonants(const char *name);
double vowel_share(const char *surname);
void print_letter_stats(const char *surname, const char *name);
void process(Student *s, int n);

void save_text(const char *file, Student *arr, int n);
void save_bin(const char *file, Student *arr, int n);

void lab6_longest_name();
void lab6_replace_surname();

// -------------------------------
//        ОСНОВНЕ МЕНЮ
// -------------------------------
void menu() {
    Student arr[MAX_STUDENTS];
    int count = 0;

    while (1) {
        printf("\n------- МЕНЮ -------\n");
        printf("1. Лабораторна #5\n");
        printf("2. Лабораторна #6\n");
        printf("3. Зберегти дані у файл\n");
        printf("4. ВИХІД\n");
        printf("Ваш вибір: ");

        int c;
        scanf("%d", &c);

        switch (c) {
            case 1: lab5(arr, &count); break;
            case 2: lab6(); break;
            case 3: save_menu(arr, count); break;
            case 4: printf("Вихід...\n"); return;
            default: printf("Невірний вибір!\n");
        }
    }
}

// -------------------------------
//    ЛАБОРАТОРНА №5
// -------------------------------
void lab5(Student *arr, int *count) {
    int mode, n;
    char fname[128];

    printf("1. Заповнення вручну / випадково\n");
    printf("2. Зчитати з текстового файлу\n");
    printf("3. Зчитати з бінарного файлу\n");
    printf("Ваш вибір: ");
    scanf("%d", &mode);

    if (mode == 1) {
        printf("Кількість студентів: ");
        scanf("%d", &n);
        *count = n;

        int type;
        printf("1 — вручну, 2 — випадково: ");
        scanf("%d", &type);

        if (type == 1) fill_manual(arr, n);
        else           fill_random(arr, n);

    } else if (mode == 2) {
        printf("Введіть назву файлу: ");
        scanf("%s", fname);

        FILE *f = fopen(fname, "r");
        if (!f) { printf("Помилка відкриття файлу!\n"); return; }

        *count = 0;
        while (!feof(f) && *count < MAX_STUDENTS) {
            fscanf(f, "%s %s %s %f",
                   arr[*count].surname,
                   arr[*count].name,
                   arr[*count].group,
                   &arr[*count].avg);
            (*count)++;
        }

        fclose(f);
        LOG("Прочитано %d записів\n", *count);

    } else if (mode == 3) {
        printf("Введіть назву файлу: ");
        scanf("%s", fname);

        FILE *f = fopen(fname, "rb");
        if (!f) { printf("Помилка відкриття файлу!\n"); return; }

        *count = fread(arr, sizeof(Student), MAX_STUDENTS, f);
        fclose(f);
        LOG("Прочитано %d структур із бінарного файлу\n", *count);
    }

    printf("\n=== Обробка ===\n");
    process(arr, *count);

    printf("\nГрупа з найкращим середнім балом: %s\n",
           best_group(arr, *count));
}

// -------------------------------
//    ЛАБОРАТОРНА №6
// -------------------------------
void lab6() {
    int c;

    printf("\n--- Лабораторна 6 ---\n");
    printf("1. Пошук найдовшого імені у текстовому файлі\n");
    printf("2. Пошук і заміна прізвища у бінарному файлі\n");
    printf("Ваш вибір: ");
    scanf("%d", &c);

    if (c == 1) lab6_longest_name();
    else if (c == 2) lab6_replace_surname();
    else printf("Невірний вибір!\n");
}

// Пошук найдовшого імені
void lab6_longest_name() {
    char file[128];
    char bestName[MAX_NAME] = "";
    char sname[MAX_NAME], name[MAX_NAME], group[16];
    float avg;

    printf("Назва текстового файлу: ");
    scanf("%s", file);

    FILE *f = fopen(file, "r");
    if (!f) { printf("Помилка відкриття файлу!\n"); return; }

    while (fscanf(f, "%s %s %s %f", sname, name, group, &avg) == 4) {
        if (strlen(name) > strlen(bestName))
            strcpy(bestName, name);
    }

    fclose(f);

    printf("Найдовше ім'я: %s\n", bestName);
}

// Пошук і заміна прізвища у бінарному файлі
void lab6_replace_surname() {
    char file[128], old[MAX_NAME], repl[MAX_NAME];

    printf("Назва бінарного файлу: ");
    scanf("%s", file);

    printf("Прізвище для пошуку: ");
    scanf("%s", old);

    printf("Нове прізвище: ");
    scanf("%s", repl);

    FILE *f = fopen(file, "rb+");
    if (!f) { printf("Помилка відкриття!\n"); return; }

    Student s;
    int found = 0;

    while (fread(&s, sizeof(Student), 1, f) == 1) {
        if (strcmp(s.surname, old) == 0) {
            strcpy(s.surname, repl);

            fseek(f, -sizeof(Student), SEEK_CUR);
            fwrite(&s, sizeof(Student), 1, f);

            LOG("Заміна у позиції %ld\n",
                ftell(f) / sizeof(Student));

            found = 1;
        }
    }

    fclose(f);

    if (found) printf("Заміна виконана.\n");
    else       printf("Прізвище не знайдено.\n");
}

// -------------------------------
//         Збереження
// -------------------------------
void save_menu(Student *arr, int count) {
    if (count == 0) {
        printf("Немає даних для збереження!\n");
        return;
    }

    int c;
    char file[128];

    printf("1. В текстовий файл\n");
    printf("2. В бінарний файл\n");
    printf("Ваш вибір: ");
    scanf("%d", &c);

    printf("Назва файлу: ");
    scanf("%s", file);

    if (c == 1) save_text(file, arr, count);
    else        save_bin(file, arr, count);
}

void save_text(const char *file, Student *arr, int n) {
    FILE *f = fopen(file, "w");
    if (!f) { printf("Помилка відкриття!\n"); return; }

    for (int i = 0; i < n; i++)
        fprintf(f, "%s %s %s %.2f\n",
                arr[i].surname, arr[i].name,
                arr[i].group, arr[i].avg);

    fclose(f);

    LOG("Записано %d записів у текстовий файл.\n", n);
    printf("Дані збережено.\n");
}

void save_bin(const char *file, Student *arr, int n) {
    FILE *f = fopen(file, "wb");
    if (!f) { printf("Помилка відкриття!\n"); return; }

    fwrite(arr, sizeof(Student), n, f);
    fclose(f);

    LOG("Записано %d структур у бінарний файл.\n", n);
    printf("Дані збережено.\n");
}

// -------------------------------
//      *Ваш вихідний функціонал*
// -------------------------------

void normalize(const char *src, char *dst) {
    int j = 0;
    for (int i = 0; src[i]; i++) {
        char c = tolower((unsigned char)src[i]);
        if (c==' ' || c=='-' || c=='\'' || c=='`') continue;
        dst[j++] = c;
    }
    dst[j] = '\0';
}

int count_consonants(const char *name) {
    int cnt = 0;
    for (int i = 0; name[i]; i++)
        if (isalpha(name[i]) && !IS_VOWEL(name[i]))
            cnt++;
    return cnt;
}

double vowel_share(const char *surname) {
    int v = 0, t = 0;

    for (int i = 0; surname[i]; i++) {
        if (isalpha(surname[i])) {
            t++;
            if (IS_VOWEL(surname[i])) v++;
        }
    }
    return t ? (double)v/t : 0.0;
}

void count_for_letter(const char *surname, char letter,
                      int *count, int *first, int *last)
{
    *count = 0;
    *first = -1;
    *last  = -1;

    for (int i = 0; surname[i]; i++) {
        if (surname[i] == letter) {
            (*count)++;
            if (*first == -1) *first = i;
            *last = i;
        }
    }
}

void print_letter_stats(const char *surname, const char *name) {
    printf(": ");
    for (int i=0; name[i]; i++) {
        int c,f,l;
        count_for_letter(surname, name[i], &c, &f, &l);
        if (c==0) printf("%c/0[-,-] ", name[i]);
        else printf("%c/%d[%d,%d] ", name[i], c, f, l);
    }
    printf("\n");
}

void process(Student *s, int n) {
    char normS[64], normN[64];

    for (int i = 0; i < n; i++) {
        printf("\n%s %s", s[i].surname, s[i].name);

        normalize(s[i].surname, normS);
        normalize(s[i].name, normN);

        print_letter_stats(normS, normN);

        printf("  Частка голосних у прізвищі: %.2f\n",
                vowel_share(normS));
        printf("  Приголосних у імені: %d\n",
                count_consonants(normN));
    }
}

// Середній бал групи
char* best_group(Student *s, int n) {
    static char best[16];
    float sum[100] = {0};
    int count[100] = {0};
    char groups[100][16];
    int g = 0;

    for (int i=0; i<n; i++) {
        int f = -1;
        for (int j=0; j<g; j++)
            if (strcmp(groups[j], s[i].group)==0) f=j;

        if (f == -1) {
            strcpy(groups[g], s[i].group);
            sum[g] = s[i].avg;
            count[g] = 1;
            g++;
        } else {
            sum[f] += s[i].avg;
            count[f]++;
        }
    }

    float bestAvg = -1;
    for (int i=0; i<g; i++) {
        float cur = sum[i] / count[i];
        if (cur > bestAvg) {
            bestAvg = cur;
            strcpy(best, groups[i]);
        }
    }
    return best;
}

// Унікальність
int unique(Student *arr, int size, Student s) {
    for (int i=0; i<size; i++)
        if (!strcmp(arr[i].surname,s.surname) &&
            !strcmp(arr[i].name,s.name) &&
            !strcmp(arr[i].group,s.group))
            return 0;
    return 1;
}

// Вручну
void fill_manual(Student *s, int n) {
    printf("Введіть: Прізвище Ім'я Група Бал\n");
    for (int i=0;i<n;i++) {
        printf("[%d] ", i+1);
        scanf("%s %s %s %f",
              s[i].surname, s[i].name,
              s[i].group, &s[i].avg);
    }
}

// Випадково
void fill_random(Student *s, int n) {
    char surn[][20] = {"Ruban","Petrenko","Shevchenko","Bondar"};
    char names[][20] = {"Maria","Oleg","Mykola","Serhii"};
    char gr[][10] = {"KA-51","KA-52","KA-53","KA-54"};

    printf("\nАвтоматично згенерований список:\n");

    for (int i=0; i<n; ) {
        Student t;
        strcpy(t.surname, surn[rand()%4]);
        strcpy(t.name,    names[rand()%4]);
        strcpy(t.group,   gr[rand()%4]);
        t.avg = 5.0 + (rand()%51)/10.0;

        if (unique(s, i, t)) {
            s[i] = t;
            printf("%d) %-12s %-10s %-6s %.2f\n",
                   i+1, s[i].surname, s[i].name, s[i].group, s[i].avg);
            i++;
        }
    }
}

// -------------------------------
//        main()
// -------------------------------
int main() {
    menu();
    return 0;
}
