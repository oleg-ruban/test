#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE 28886
#define MAX_DIGITS 1000

typedef struct {
    int d[MAX_DIGITS];
    int len;
} Long;

// -----------------------------------------------
// Перетворення десяткового числа (у вигляді рядка) у Long
// -----------------------------------------------
Long fromDecimal(char *s) {
    Long R = {{0}, 1};

    // Тимчасова змінна – копія числа у десятковій формі
    char buf[5000];
    strcpy(buf, s);

    // Ділимо на BASE поки число не стане 0
    while (!(strlen(buf) == 1 && buf[0] == '0')) {
        long long carry = 0;
        char newbuf[5000];
        int pos = 0;
        int started = 0;

        for (int i = 0; buf[i]; i++) {
            int x = carry * 10 + (buf[i] - '0');
            int q = x / BASE;
            int r = x % BASE;

            if (q != 0 || started) {
                newbuf[pos++] = q + '0';
                started = 1;
            }
            carry = r;
        }

        R.d[R.len - 1] = carry;
        R.d[R.len] = 0;
        R.len++;

        if (pos == 0) {
            strcpy(buf, "0");
        } else {
            newbuf[pos] = '\0';
            strcpy(buf, newbuf);
        }
    }

    return R;
}

// -----------------------------------------------
// Перетворення Long у десяткове число (рядок)
// -----------------------------------------------
char *toDecimal(Long A) {
    static char out[10000];
    memset(out, 0, sizeof(out));

    char buf[10000] = "0";

    for (int i = A.len - 1; i >= 0; i--) {
        long long carry = A.d[i];
        int len = strlen(buf);

        for (int j = 0; j < len; j++) {
            carry += (long long)(buf[j] - '0') * BASE;
            buf[j] = (carry % 10) + '0';
            carry /= 10;
        }

        while (carry > 0) {
            buf[len++] = (carry % 10) + '0';
            carry /= 10;
        }
        buf[len] = '\0';
    }

    int n = strlen(buf);
    for (int i = 0; i < n; i++)
        out[i] = buf[n - 1 - i];

    return out;
}

// -----------------------------------------------
// Арифметичні операції у BASE 28886
// -----------------------------------------------

Long add(Long A, Long B) {
    Long R = {{0}, 0};
    int carry = 0;
    int n = (A.len > B.len ? A.len : B.len);

    for (int i = 0; i < n; i++) {
        int x = (i < A.len ? A.d[i] : 0);
        int y = (i < B.len ? B.d[i] : 0);
        int s = x + y + carry;

        R.d[R.len++] = s % BASE;
        carry = s / BASE;
    }

    if (carry) R.d[R.len++] = carry;
    return R;
}

Long sub(Long A, Long B) {
    Long R = {{0}, 0};
    int borrow = 0;

    for (int i = 0; i < A.len; i++) {
        int x = A.d[i] - borrow;
        int y = (i < B.len ? B.d[i] : 0);

        if (x < y) {
            x += BASE;
            borrow = 1;
        } else borrow = 0;

        R.d[R.len++] = x - y;
    }

    while (R.len > 1 && R.d[R.len - 1] == 0)
        R.len--;

    return R;
}

Long mul(Long A, Long B) {
    Long R = {{0}, A.len + B.len};

    for (int i = 0; i < A.len; i++) {
        long long carry = 0;
        for (int j = 0; j < B.len; j++) {
            long long cur = R.d[i + j] +
                            (long long)A.d[i] * B.d[j] +
                            carry;
            R.d[i + j] = cur % BASE;
            carry = cur / BASE;
        }
        R.d[i + B.len] += carry;
    }

    while (R.len > 1 && R.d[R.len - 1] == 0)
        R.len--;

    return R;
}

// -----------------------------------------------
// Розбір вводу виду: "123 456 789 - 95 000"
// -----------------------------------------------
void splitInput(char *line, char *A, char *op, char *B) {
    int i = 0, j = 0;

    while (line[i] != '+' && line[i] != '-' &&
           line[i] != '*' && line[i] != '/') {
        A[j++] = line[i++];
    }
    A[j] = 0;

    *op = line[i++];

    j = 0;
    while (line[i] != 0 && line[i] != '\n')
        B[j++] = line[i++];
    B[j] = 0;

    // Очистити зайві пробіли
    while (A[0] == ' ') memmove(A, A+1, strlen(A));
    while (B[0] == ' ') memmove(B, B+1, strlen(B));
}

// -----------------------------------------------
// Ділення Long на Long (спрощене, через ділення на int)
// -----------------------------------------------
Long divInt(Long A, int b) {
    Long R = {{0}, A.len};
    long long rem = 0;

    for (int i = A.len - 1; i >= 0; i--) {
        long long cur = A.d[i] + rem * BASE;
        R.d[i] = cur / b;
        rem = cur % b;
    }

    while (R.len > 1 && R.d[R.len - 1] == 0)
        R.len--;

    return R;
}

// -----------------------------------------------
// MAIN
// -----------------------------------------------
int main() {
    char line[5000];
    printf("Введіть вираз:\n");

    fgets(line, sizeof(line), stdin);

    char Adec[3000], Bdec[3000], op;
    splitInput(line, Adec, &op, Bdec);

    // Прибираємо пробіли між цифрами десяткового числа
    for (int i = 0; Adec[i]; i++)
        if (Adec[i] == ' ') memmove(Adec + i, Adec + i + 1, strlen(Adec + i));

    for (int i = 0; Bdec[i]; i++)
        if (Bdec[i] == ' ') memmove(Bdec + i, Bdec + i + 1, strlen(Bdec + i));

    Long A = fromDecimal(Adec);
    Long B = fromDecimal(Bdec);
    Long R;

    if (op == '+') R = add(A, B);
    else if (op == '-') R = sub(A, B);
    else if (op == '*') R = mul(A, B);
    else if (op == '/') {
        int divisor = atoi(Bdec);
        R = divInt(A, divisor);
    }

    printf("Внутрішнє представлення A: [");
    for (int i = 0; i < A.len; i++) {
        printf("%d", A.d[i]);
        if (i < A.len - 1) printf(",");
    }
    printf("]\n");

    printf("Внутрішнє представлення B: [");
    for (int i = 0; i < B.len; i++) {
        printf("%d", B.d[i]);
        if (i < B.len - 1) printf(",");
    }
    printf("]\n");

    printf("Внутрішнє представлення R: [");
    for (int i = 0; i < R.len; i++) {
        printf("%d", R.d[i]);
        if (i < R.len - 1) printf(",");
    }
    printf("]\n");

    printf("\nРезультат у десятковій системі:\n%s\n", toDecimal(R));

    return 0;
}
