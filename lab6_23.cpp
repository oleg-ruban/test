#include <iostream>
using namespace std;

// Структура вузла
struct Node {
    int data;
    Node* next;
};

// Функція створення нового вузла
Node* createNode(int value) {
    Node* newNode = new Node;
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}

// =============================
// 1️⃣ РОЗВОРОТ СПИСКУ
// =============================
Node* reverse(Node* head) {
    Node* prev = NULL;
    Node* current = head;
    Node* next = NULL;

    while (current != NULL) {
        next = current->next;   // зберігаємо наступний
        current->next = prev;   // розвертаємо посилання
        prev = current;         // рухаємо prev вперед
        current = next;         // рухаємо current вперед
    }

    return prev; // нова голова
}

// =============================
// 2️⃣ ПЕРЕВІРКА НА ЦИКЛ
// =============================
int hasCycle(Node* head) {
    if (head == NULL) return 0;

    Node* slow = head;
    Node* fast = head;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) {
            return 1; // цикл знайдено
        }
    }

    return 0; // циклу немає
}

// =============================
// ДОДАТКОВІ ФУНКЦІЇ
// =============================

// Додавання елемента в кінець
void append(Node*& head, int value) {
    Node* newNode = createNode(value);

    if (head == NULL) {
        head = newNode;
        return;
    }

    Node* temp = head;
    while (temp->next != NULL)
        temp = temp->next;

    temp->next = newNode;
}

// Вивід списку
void printList(Node* head) {
    while (head != NULL) {
        cout << head->data << " ";
        head = head->next;
    }
    cout << endl;
}

// =============================
// MAIN
// =============================
int main() {
    Node* head = NULL;

    append(head, 1);
    append(head, 2);
    append(head, 3);
    append(head, 4);

    cout << "Початковий список: ";
    printList(head);

    // Перевірка на цикл
    cout << "Чи є цикл? " << hasCycle(head) << endl;

    // Розворот списку
    head = reverse(head);

    cout << "Розвернутий список: ";
    printList(head);

    return 0;
}