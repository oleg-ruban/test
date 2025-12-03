#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE 28886
#define MAX_DIGITS 2000

typedef struct {
    int d[MAX_DIGITS];
    int len;
    int sign;      // 1 або -1
} Long;

// Ініціалізація Long
void initLong(Long *A) {
    A->len = 1;
    A->sign = 1;
    for (int i = 0; i < MAX_DIGITS; i++)
        A->d[i] = 0;
}

// Порівняння |A| та |B|
int cmpAbs(Long *A, Long *B) {
    if (A->len > B->len) return 1;
    if (A->len < B->len) return -1;
    for (int i = A->len - 1; i >= 0; i--) {
        if (A->d[i] > B->d[i]) return 1;
        if (A->d[i] < B->d[i]) return -1;
    }
    return 0;
}

// Додавання без урахування знака
Long addAbs(Long A, Long B) {
    Long R;
    initLong(&R);

    int carry = 0;
    int n = (A.len > B.len ? A.len : B.len);
    R.len = 0;

    for (int i = 0; i < n; i++) {
        int x = (i < A.len ? A.d[i] : 0);
        int y = (i < B.len ? B.d[i] : 0);
        int s = x + y + carry;
        R.d[R.len++] = s % BASE;
        carry = s / BASE;
    }
    if (carry)
        R.d[R.len++] = carry;

    return R;
}

// Віднімання |A| - |B|, A >= B
Long subAbs(Long A, Long B) {
    Long R;
    initLong(&R);
    int borrow = 0;

    R.len = A.len;
    for (int i = 0; i < A.len; i++) {
        int x = A.d[i] - borrow;
        int y = (i < B.len ? B.d[i] : 0);
        if (x < y) {
            x += BASE;
            borrow = 1;
        } else borrow = 0;
        R.d[i] = x - y;
    }

    while (R.len > 1 && R.d[R.len - 1] == 0)
        R.len--;

    return R;
}

// Додавання з урахуванням знака
Long add(Long A, Long B) {
    if (A.sign == B.sign) {
        Long R = addAbs(A, B);
        R.sign = A.sign;
        return R;
    }

    int cmp = cmpAbs(&A, &B);
    if (cmp == 0) {
        Long R;
        initLong(&R);
        return R;
    }
    if (cmp > 0) {
        Long R = subAbs(A, B);
        R.sign = A.sign;
        return R;
    } else {
        Long R = subAbs(B, A);
        R.sign = B.sign;
        return R;
    }
}

// Віднімання з урахуванням знака
Long sub(Long A, Long B) {
    B.sign *= -1;
    return add(A, B);
}

// Множення
Long mul(Long A, Long B) {
    Long R;
    initLong(&R);
    R.len = A.len + B.len;

    for (int i = 0; i < A.len; i++) {
        long long carry = 0;
        for (int j = 0; j < B.len; j++) {
            long long cur = R.d[i + j] + (long long)A.d[i] * B.d[j] + carry;
            R.d[i + j] = cur % BASE;
            carry = cur / BASE;
        }
        R.d[i + B.len] += carry;
    }

    while (R.len > 1 && R.d[R.len - 1] == 0)
        R.len--;

    R.sign = A.sign * B.sign;
    return R;
}

// Конвертація десяткового рядка → Long
Long fromDecimal(const char *s) {
    Long R;
    initLong(&R);

    char buf[5000];
    strcpy(buf, s);

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

        if (pos == 0) strcpy(buf, "0");
        else {
            newbuf[pos] = '\0';
            strcpy(buf, newbuf);
        }
    }

    return R;
}

// Перетворення Long → десятковий рядок
char *toDecimal(Long A) {
    static char out[21000];
    memset(out, 0, sizeof(out));

    char buf[21000] = "0";

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

    // Вставка знака без додаткового буфера
    if (A.sign < 0) {
        memmove(out + 1, out, n + 1); // +1 для '\0'
        out[0] = '-';
    }

    return out;
}

// Розбір виразу "123 456 - 55 000"
void splitInputLine(const char *line, char *A, char *op, char *B) {
    int i = 0;
    while (line[i] == ' ') i++;
    int j = 0;
    while (line[i] && line[i] != '+' && line[i] != '-' &&
           line[i] != '*' && line[i] != '/') {
        A[j++] = line[i++];
    }
    A[j] = 0;
    *op = line[i++];
    while (line[i] == ' ') i++;
    j = 0;
    while (line[i] && line[i] != '\n') {
        B[j++] = line[i++];
    }
    B[j] = 0;
}


int main() {
    char line[5000];
    printf("Введіть вираз (наприклад, 954 654 565 - 879 654 004):\n");

    fgets(line, sizeof(line), stdin);

    char Araw[3000], Braw[3000], op;
    splitInputLine(line, Araw, &op, Braw);

    // Прибираємо пробіли
    for (int i = 0; Araw[i]; i++)
        if (Araw[i] == ' ') memmove(Araw + i, Araw + i + 1, strlen(Araw + i));
    for (int i = 0; Braw[i]; i++)
        if (Braw[i] == ' ') memmove(Braw + i, Braw + i + 1, strlen(Braw + i));

    Long A = fromDecimal(Araw);
    Long B = fromDecimal(Braw);
    Long R;

    if (op == '+') R = add(A, B);
    else if (op == '-') R = sub(A, B);
    else if (op == '*') R = mul(A, B);
    else {
        printf("Ділення поки не реалізовано\n");
        return 0;
    }

    printf("Внутрішній формат A: [");
    for (int i = 0; i < A.len; i++) {
        printf("%d", A.d[i]);
        if (i < A.len - 1) printf(",");
    }
    printf("]\n");

    printf("Внутрішній формат B: [");
    for (int i = 0; i < B.len; i++) {
        printf("%d", B.d[i]);
        if (i < B.len - 1) printf(",");
    }
    printf("]\n");

    printf("Внутрішній формат R: %s[", (R.sign < 0 ? "-" : ""));
    for (int i = 0; i < R.len; i++) {
        printf("%d", R.d[i]);
        if (i < R.len - 1) printf(",");
    }
    printf("]\n");

    printf("Десятковий результат:\n%s\n", toDecimal(R));

    return 0;
}
