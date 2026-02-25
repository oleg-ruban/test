#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

//////////////////////////////////////////////////////
// STRUCT: Sticker
//////////////////////////////////////////////////////

struct Sticker {
    string name;
    int popularity;
    int funniness;

    Sticker(string n = "", int p = 0, int f = 0)
        : name(n), popularity(p), funniness(f) {}
};

//////////////////////////////////////////////////////
// DYNAMIC ARRAY IMPLEMENTATION
//////////////////////////////////////////////////////

class DynamicArray {
private:
    Sticker* data;
    int capacity;
    int length;

    void resize(int newCapacity) {
        Sticker* newData = new Sticker[newCapacity];
        for (int i = 0; i < length; i++)
            newData[i] = data[i];

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    DynamicArray() {
        capacity = 10;
        length = 0;
        data = new Sticker[capacity];
    }

    ~DynamicArray() {
        delete[] data;
    }

    void push_back(const Sticker& s) {
        if (length == capacity)
            resize(capacity * 2);

        data[length++] = s;
    }

    Sticker pop_back() {
        if (length == 0) throw out_of_range("Empty array");
        return data[--length];
    }

    void push_front(const Sticker& s) {
        insert(0, s);
    }

    Sticker pop_front() {
        return remove(0);
    }

    Sticker get(int index) {
        if (index < 0 || index >= length)
            throw out_of_range("Index error");
        return data[index];
    }

    int size() {
        return length;
    }

    void insert(int index, const Sticker& s) {
        if (index < 0 || index > length)
            throw out_of_range("Index error");

        if (length == capacity)
            resize(capacity * 2);

        for (int i = length; i > index; i--)
            data[i] = data[i - 1];

        data[index] = s;
        length++;
    }

    Sticker remove(int index) {
        if (index < 0 || index >= length)
            throw out_of_range("Index error");

        Sticker temp = data[index];

        for (int i = index; i < length - 1; i++)
            data[i] = data[i + 1];

        length--;
        return temp;
    }

    void clear() {
        delete[] data;
        capacity = 10;
        length = 0;
        data = new Sticker[capacity];
    }

    void print() {
        for (int i = 0; i < length; i++)
            cout << data[i].name << " "
                 << data[i].popularity << " "
                 << data[i].funniness << endl;
    }
};

//////////////////////////////////////////////////////
// LINKED LIST IMPLEMENTATION
//////////////////////////////////////////////////////

struct Node {
    Sticker data;
    Node* next;

    Node(const Sticker& s) : data(s), next(nullptr) {}
};

class LinkedList {
private:
    Node* head;
    int length;

public:
    LinkedList() : head(nullptr), length(0) {}

    ~LinkedList() {
        clear();
    }

    void push_front(const Sticker& s) {
        Node* newNode = new Node(s);
        newNode->next = head;
        head = newNode;
        length++;
    }

    void push_back(const Sticker& s) {
        Node* newNode = new Node(s);

        if (!head) {
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next)
                temp = temp->next;
            temp->next = newNode;
        }
        length++;
    }

    Sticker pop_front() {
        if (!head) throw out_of_range("Empty list");

        Node* temp = head;
        Sticker s = temp->data;
        head = head->next;
        delete temp;
        length--;
        return s;
    }

    Sticker pop_back() {
        if (!head) throw out_of_range("Empty list");

        if (!head->next) {
            Sticker s = head->data;
            delete head;
            head = nullptr;
            length--;
            return s;
        }

        Node* temp = head;
        while (temp->next->next)
            temp = temp->next;

        Sticker s = temp->next->data;
        delete temp->next;
        temp->next = nullptr;
        length--;
        return s;
    }

    Sticker get(int index) {
        if (index < 0 || index >= length)
            throw out_of_range("Index error");

        Node* temp = head;
        for (int i = 0; i < index; i++)
            temp = temp->next;

        return temp->data;
    }

    void insert(int index, const Sticker& s) {
        if (index == 0) {
            push_front(s);
            return;
        }

        Node* temp = head;
        for (int i = 0; i < index - 1; i++)
            temp = temp->next;

        Node* newNode = new Node(s);
        newNode->next = temp->next;
        temp->next = newNode;
        length++;
    }

    Sticker remove(int index) {
        if (index == 0)
            return pop_front();

        Node* temp = head;
        for (int i = 0; i < index - 1; i++)
            temp = temp->next;

        Node* toDelete = temp->next;
        Sticker s = toDelete->data;
        temp->next = toDelete->next;
        delete toDelete;
        length--;
        return s;
    }

    int size() {
        return length;
    }

    void clear() {
        while (head)
            pop_front();
    }

    void print() {
        Node* temp = head;
        while (temp) {
            cout << temp->data.name << " "
                 << temp->data.popularity << " "
                 << temp->data.funniness << endl;
            temp = temp->next;
        }
    }
};

//////////////////////////////////////////////////////
// PERFORMANCE TEST
//////////////////////////////////////////////////////

Sticker randomSticker() {
    return Sticker("S",
        rand() % 1000,
        rand() % 1000);
}

int main() {

    srand(time(0));

    DynamicArray arr;
    LinkedList list;

    clock_t startTime, endTime;
    double seconds;

    /////////////////////////////
    // DynamicArray tests
    /////////////////////////////

    cout << "DynamicArray:\n";

    startTime = clock();
    for (int i = 0; i < 50000; i++)
        arr.push_back(randomSticker());
    endTime = clock();
    cout << "push_back 50000: "
         << (double)(endTime - startTime) / CLOCKS_PER_SEC << endl;

    startTime = clock();
    for (int i = 0; i < 10000; i++)
        arr.push_front(randomSticker());
    endTime = clock();
    cout << "push_front 10000: "
         << (double)(endTime - startTime) / CLOCKS_PER_SEC << endl;

    startTime = clock();
    for (int i = 0; i < 20000; i++)
        arr.get(rand() % arr.size());
    endTime = clock();
    cout << "get 20000 random: "
         << (double)(endTime - startTime) / CLOCKS_PER_SEC << endl;

    startTime = clock();
    for (int i = 0; i < 5000; i++)
        arr.pop_front();
    endTime = clock();
    cout << "pop_front 5000: "
         << (double)(endTime - startTime) / CLOCKS_PER_SEC << endl;

    startTime = clock();
    for (int i = 0; i < 5000; i++)
        arr.pop_back();
    endTime = clock();
    cout << "pop_back 5000: "
         << (double)(endTime - startTime) / CLOCKS_PER_SEC << endl;


    /////////////////////////////
    // LinkedList tests
    /////////////////////////////

    cout << "\nLinkedList:\n";

    startTime = clock();
    for (int i = 0; i < 50000; i++)
        list.push_back(randomSticker());
    endTime = clock();
    cout << "push_back 50000: "
         << (double)(endTime - startTime) / CLOCKS_PER_SEC << endl;

    startTime = clock();
    for (int i = 0; i < 10000; i++)
        list.push_front(randomSticker());
    endTime = clock();
    cout << "push_front 10000: "
         << (double)(endTime - startTime) / CLOCKS_PER_SEC << endl;

    startTime = clock();
    for (int i = 0; i < 20000; i++)
        list.get(rand() % list.size());
    endTime = clock();
    cout << "get 20000 random: "
         << (double)(endTime - startTime) / CLOCKS_PER_SEC << endl;

    startTime = clock();
    for (int i = 0; i < 5000; i++)
        list.pop_front();
    endTime = clock();
    cout << "pop_front 5000: "
         << (double)(endTime - startTime) / CLOCKS_PER_SEC << endl;

    startTime = clock();
    for (int i = 0; i < 5000; i++)
        list.pop_back();
    endTime = clock();
    cout << "pop_back 5000: "
         << (double)(endTime - startTime) / CLOCKS_PER_SEC << endl;

    return 0;
}