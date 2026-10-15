#include <iostream>
#include <string>

using namespace std;

const int MAX_SIZE = 5;
const int PAGES_PER_TURN = 2;

struct Job {
    string name;
    int pages;
};

struct Node {
    Job job;
    Node *next;
};

class LinkedQueue {
private:
    Node *front;
    Node *rear;
    int size;

public:
    LinkedQueue() {
        front = nullptr;
        rear = nullptr;
        size = 0;}

    ~LinkedQueue() {
        Job job;
        while (dequeue(job)) {
        }
    }

    bool isEmpty() {
        return front == nullptr;}

    bool isFull() {
        return false;}

    void enqueue(Job job) {
        Node *node = new Node{job, nullptr};
        if (isEmpty()) {
            front = node;
        } else {
            rear->next = node;}
        rear = node;
        size++;}

    bool dequeue(Job &job) {
        if (isEmpty()) {
            return false;}
        Node *old = front;
        job = old->job;
        front = front->next;
        if (front == nullptr) {
            rear = nullptr;}
        delete old;
        size--;
        return true;}

    bool peek(Job &job) {
        if (isEmpty()) {
            return false;}
        job = front->job;
        return true;}

    bool search(string name) {
        for (Node *cur = front; cur != nullptr; cur = cur->next) {
            if (cur->job.name == name) {
                return true;}
        }
        return false;}

    bool insertAt(int position, Job job) {
        if (position < 1 || position > size + 1) {
            return false;}
        if (position == size + 1) {
            enqueue(job);
            return true;}
        Node *node = new Node{job, nullptr};
        if (position == 1) {
            node->next = front;
            front = node;
        } else {
            Node *prev = front;
            for (int i = 1; i < position - 1; i++) {
                prev = prev->next;}
            node->next = prev->next;
            prev->next = node;}
        size++;
        return true;}

    bool removeByName(string name) {
        Node *prev = nullptr;
        Node *cur = front;
        while (cur != nullptr && cur->job.name != name) {
            prev = cur;
            cur = cur->next;}
        if (cur == nullptr) {
            return false;}
        if (prev == nullptr) {
            front = cur->next;
        } else {
            prev->next = cur->next;}
        if (cur == rear) {
            rear = prev;}
        delete cur;
        size--;
        return true;}

    void print() {
        cout << "  front -> ";
        for (Node *cur = front; cur != nullptr; cur = cur->next) {
            cout << cur->job.name << "(" << cur->job.pages << ") -> ";}
        cout << "null\n";}
};

class CircularQueue {
private:
    Job data[MAX_SIZE];
    int front;
    int rear;
    int count;

    int slot(int offset) {
        return (front + offset) % MAX_SIZE;}

public:
    CircularQueue() {
        front = 0;
        rear = 0;
        count = 0;}

    bool isEmpty() {
        return count == 0;}

    bool isFull() {
        return count == MAX_SIZE;}

    bool enqueue(Job job) {
        if (isFull()) {
            return false;}
        data[rear] = job;
        rear = (rear + 1) % MAX_SIZE;
        count++;
        return true;}

    bool dequeue(Job &job) {
        if (isEmpty()) {
            return false;}
        job = data[front];
        front = (front + 1) % MAX_SIZE;
        count--;
        return true;}

    bool peek(Job &job) {
        if (isEmpty()) {
            return false;}
        job = data[front];
        return true;}

    bool search(string name) {
        for (int i = 0; i < count; i++) {
            if (data[slot(i)].name == name) {
                return true;}
        }
        return false;}

    bool insertAt(int position, Job job) {
        if (isFull() || position < 1 || position > count + 1) {
            return false;}
        int offset = position - 1;
        for (int i = count; i > offset; i--) {
            data[slot(i)] = data[slot(i - 1)];}
        data[slot(offset)] = job;
        rear = (rear + 1) % MAX_SIZE;
        count++;
        return true;}

    bool removeByName(string name) {
        int offset = -1;
        for (int i = 0; i < count && offset == -1; i++) {
            if (data[slot(i)].name == name) {
                offset = i;}
        }
        if (offset == -1) {
            return false;}
        for (int i = offset; i < count - 1; i++) {
            data[slot(i)] = data[slot(i + 1)];}
        rear = (rear - 1 + MAX_SIZE) % MAX_SIZE;
        count--;
        return true;}

    void print() {
        cout << "  slots:";
        for (int i = 0; i < MAX_SIZE; i++) {
            if ((i - front + MAX_SIZE) % MAX_SIZE < count) {
                cout << " " << data[i].name << "(" << data[i].pages << ")";
            } else {
                cout << " _";}
        }
        cout << "  | front=" << front << " rear=" << rear << " count=" << count << "\n";}
};

void part1() {
    cout << "===== PART 1: LINKED-LIST QUEUE =====\n";
    LinkedQueue q;
    Job job;
    string names[3] = {"Ana", "Ben", "Chen"};
    int pages[3] = {3, 5, 2};

    cout << "isEmpty: " << q.isEmpty() << "\n";
    cout << "peek on empty queue: " << q.peek(job) << "\n";
    for (int i = 0; i < 3; i++) {
        cout << "enqueue " << names[i] << "\n";
        q.enqueue({names[i], pages[i]});
        q.print();}
    q.peek(job);
    cout << "peek: " << job.name << "\n";
    while (q.dequeue(job)) {
        cout << "dequeue: " << job.name << (q.isEmpty() ? "  (last node removed)" : "") << "\n";
        q.print();}
    cout << "isEmpty: " << q.isEmpty() << "\n";
    cout << "dequeue on empty queue: " << q.dequeue(job) << "\n";}

void part2() {
    cout << "\n===== PART 2: CIRCULAR ARRAY QUEUE (size " << MAX_SIZE << ") =====\n";
    CircularQueue q;
    Job job;
    string names[6] = {"Ana", "Ben", "Chen", "Dee", "Eli", "Fay"};

    cout << "start  isEmpty: " << q.isEmpty() << "\n";
    q.print();
    for (int i = 0; i < 6; i++) {
        bool ok = q.enqueue({names[i], i + 1});
        cout << "enqueue " << names[i] << ": " << (ok ? "OK" : "FULL, rejected") << "\n";
        q.print();}
    cout << "isFull: " << q.isFull() << "\n";
    q.peek(job);
    cout << "peek: " << job.name << "\n";

    for (int i = 0; i < 2; i++) {
        q.dequeue(job);
        cout << "dequeue: " << job.name << "\n";
        q.print();}

    cout << "enqueue Gus: stored at slot 0, then rear = (0 + 1) % 5 = 1\n";
    q.enqueue({"Gus", 2});
    q.print();
    cout << "enqueue Hal: stored at slot 1, then rear = (1 + 1) % 5 = 2\n";
    q.enqueue({"Hal", 4});
    q.print();

    while (q.dequeue(job)) {
        cout << "dequeue: " << job.name << "\n";
        q.print();}
    cout << "isEmpty: " << q.isEmpty() << "\n";}

void part3() {
    cout << "\n===== PART 3: ROUND-ROBIN PRINTER (" << PAGES_PER_TURN << " pages per turn) =====\n";
    CircularQueue q;
    Job job;
    string names[4] = {"Ana", "Ben", "Chen", "Dee"};
    int pages[4] = {3, 5, 2, 4};

    for (int i = 0; i < 4; i++) {
        q.enqueue({names[i], pages[i]});}
    cout << "start\n";
    q.print();

    int turn = 1;
    int finished = 1;
    while (q.dequeue(job)) {
        int printed = job.pages < PAGES_PER_TURN ? job.pages : PAGES_PER_TURN;
        job.pages -= printed;
        cout << "Turn " << turn << ": " << job.name << " prints " << printed
             << " page(s), remaining " << job.pages;
        if (job.pages > 0) {
            q.enqueue(job);
            cout << " -> goes back to the rear\n";
        } else {
            cout << " -> FINISHED (#" << finished << ")\n";
            finished++;}
        q.print();
        turn++;}
}

void part4() {
    cout << "\n===== PART 4: TASKS 1-6 ON BOTH QUEUES =====\n";
    LinkedQueue lq;
    CircularQueue cq;
    Job job;
    string names[4] = {"Ana", "Ben", "Chen", "Dee"};

    for (int i = 0; i < 4; i++) {
        lq.enqueue({names[i], i + 1});
        cq.enqueue({names[i], i + 1});}
    cout << "start\n";
    lq.print();
    cq.print();

    cout << "\nTask 1: add Eli\n";
    lq.enqueue({"Eli", 2});
    cq.enqueue({"Eli", 2});
    lq.print();
    cq.print();

    cout << "\nTask 2: delete the job at the front\n";
    lq.dequeue(job);
    cq.dequeue(job);
    lq.print();
    cq.print();

    cout << "\nTask 3: is Chen's job still waiting?\n";
    cout << "  linked: " << lq.search("Chen") << "  circular: " << cq.search("Chen") << "\n";

    cout << "\nTask 4: insert Prof. Lee at position 2\n";
    lq.insertAt(2, {"Prof. Lee", 1});
    cq.insertAt(2, {"Prof. Lee", 1});
    lq.print();
    cq.print();

    cout << "\nTask 5: delete cancelled job Dee from the middle\n";
    lq.removeByName("Dee");
    cq.removeByName("Dee");
    lq.print();
    cq.print();
    cout << "  search Dee again -> linked: " << lq.search("Dee") << "  circular: " << cq.search("Dee") << "\n";

    cout << "\nTask 6: isEmpty() and isFull()\n";
    cout << "  linked: isEmpty=" << lq.isEmpty() << " isFull=" << lq.isFull() << "\n";
    cout << "  circular: isEmpty=" << cq.isEmpty() << " isFull=" << cq.isFull() << "\n";}

int main() {
    cout << boolalpha;
    part1();
    part2();
    part3();
    part4();
    return 0;}