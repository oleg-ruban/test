#include <iostream>
#include <string>

using namespace std;

// =======================
// Вузол зв’язного списку
// =======================
struct Node {
    char data;
    Node* next;
};

// =======================
// Стек на основі списку
// =======================
class Stack {
private:
    Node* topNode;

public:
    Stack() {
        topNode = nullptr;
    }

    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }

    void push(char value) {
        Node* newNode = new Node;
        newNode->data = value;
        newNode->next = topNode;
        topNode = newNode;
    }

    void pop() {
        if (isEmpty()) return;

        Node* temp = topNode;
        topNode = topNode->next;
        delete temp;
    }

    char top() {
        if (!isEmpty())
            return topNode->data;
        return '\0';
    }

    bool isEmpty() {
        return topNode == nullptr;
    }
};

// =====================================
// Функція перевірки дужкової послідовності
// =====================================
bool isBalanced(string str) {
    Stack stack;

    for (char ch : str) {

        // Якщо відкриваюча дужка
        if (ch == '(' || ch == '[' || ch == '{') {
            stack.push(ch);
        }

        // Якщо закриваюча
        else if (ch == ')' || ch == ']' || ch == '}') {

            if (stack.isEmpty())
                return false;

            char top = stack.top();
            stack.pop();

            if ((ch == ')' && top != '(') ||
                (ch == ']' && top != '[') ||
                (ch == '}' && top != '{')) {
                return false;
            }
        }
    }

    return stack.isEmpty();
}

// =======================
// Головна функція
// =======================
int main() {
    string input;

    cout << "Введіть дужкову послідовність: ";
    cin >> input;

    if (isBalanced(input))
        cout << "Послідовність КОРЕКТНА" << endl;
    else
        cout << "Послідовність НЕКОРЕКТНА" << endl;

    return 0;
}