#include <bits/stdc++.h>
using namespace std;

// ---------------- Ticket ADT (Singly Linked List Node) ----------------

struct Ticket {
    int ticketID;
    string customerName;
    string issueDescription;
    int priority; // 1 = urgent, higher numbers = lower priority
    Ticket *next;
    Ticket(int id, const string &name, const string &issue, int pr)
        : ticketID(id), customerName(name), issueDescription(issue),
          priority(pr), next(nullptr) {}
};

// ---------------- Polynomial Node for Billing History ----------------

struct PolyNode {
    int coeff;   // amount or some billing metric
    int exp;     // time period / bill index
    PolyNode *next;
    PolyNode(int c, int e) : coeff(c), exp(e), next(nullptr) {}
};

// ---------------- Stack for Undo Operations ----------------

struct UndoOperation {
    string opType; // "INSERT" or "DELETE"
    Ticket data;
    UndoOperation(const string &t, const Ticket &d) : opType(t), data(d) {}
};

class UndoStack {
    vector<UndoOperation> st;
public:
    void push(const UndoOperation &u) { st.push_back(u); }    // O(1)
    bool empty() const { return st.empty(); }                 // O(1)
    UndoOperation top() { return st.back(); }                 // O(1)
    void pop() { st.pop_back(); }                            // O(1)
};

// ---------------- Priority Queue (min-heap on priority, then TicketID) ----------------

struct PQNode {
    int priority;
    int ticketID;
    Ticket *ref;
    bool operator<(const PQNode &other) const {
        if (priority == other.priority) return ticketID > other.ticketID;
        return priority > other.priority; // smaller priority value = higher priority
    }
};

// ---------------- Circular Queue for Round Robin ----------------

class CircularQueue {
    static const int MAXQ = 1000;
    Ticket *arr[MAXQ];
    int front, rear, count;
public:
    CircularQueue() : front(0), rear(-1), count(0) {}
    bool isFull() const { return count == MAXQ; }
    bool isEmpty() const { return count == 0; }
    bool enqueue(Ticket *t) {
        if (isFull()) return false;
        rear = (rear + 1) % MAXQ;
        arr[rear] = t;
        count++;
        return true;
    }
    Ticket* dequeue() {
        if (isEmpty()) return nullptr;
        Ticket *t = arr[front];
        front = (front + 1) % MAXQ;
        count--;
        return t;
    }
};

// ---------------- Polynomial Linked List for Billing History ----------------

class Polynomial {
    PolyNode *head;
public:
    Polynomial() : head(nullptr) {}

    void insertTerm(int coeff, int exp) {
        if (coeff == 0) return;
        PolyNode *node = new PolyNode(coeff, exp);
        if (!head || exp > head->exp) {
            node->next = head;
            head = node;
            return;
        }
        PolyNode *curr = head, *prev = nullptr;
        while (curr && curr->exp > exp) {
            prev = curr;
            curr = curr->next;
        }
        if (curr && curr->exp == exp) {
            curr->coeff += coeff;
            if (curr->coeff == 0) {
                if (prev) prev->next = curr->next;
                else head = curr->next;
                delete curr;
            }
            delete node;
        } else {
            node->next = curr;
            if (prev) prev->next = node;
            else head = node;
        }
    }

    static Polynomial add(const Polynomial &p1, const Polynomial &p2) {
        Polynomial res;
        PolyNode *a = p1.head, *b = p2.head;
        while (a && b) {
            if (a->exp == b->exp) {
                res.insertTerm(a->coeff + b->coeff, a->exp);
                a = a->next; b = b->next;
            } else if (a->exp > b->exp) {
                res.insertTerm(a->coeff, a->exp);
                a = a->next;
            } else {
                res.insertTerm(b->coeff, b->exp);
                b = b->next;
            }
        }
        while (a) { res.insertTerm(a->coeff, a->exp); a = a->next; }
        while (b) { res.insertTerm(b->coeff, b->exp); b = b->next; }
        return res;
    }

    static Polynomial subtract(const Polynomial &p1, const Polynomial &p2) {
        Polynomial res;
        PolyNode *a = p1.head, *b = p2.head;
        while (a && b) {
            if (a->exp == b->exp) {
                res.insertTerm(a->coeff - b->coeff, a->exp);
                a = a->next; b = b->next;
            } else if (a->exp > b->exp) {
                res.insertTerm(a->coeff, a->exp);
                a = a->next;
            } else {
                res.insertTerm(-b->coeff, b->exp);
                b = b->next;
            }
        }
        while (a) { res.insertTerm(a->coeff, a->exp); a = a->next; }
        while (b) { res.insertTerm(-b->coeff, b->exp); b = b->next; }
        return res;
    }

    void print(const string &label) const {
        cout << label;
        if (!head) { cout << "0\n"; return; }
        PolyNode *curr = head;
        bool first = true;
        while (curr) {
            if (!first && curr->coeff > 0) cout << " + ";
            if (curr->coeff < 0) cout << " - " << abs(curr->coeff);
            else cout << (first ? "" : "") << curr->coeff;
            cout << "x^" << curr->exp;
            first = false;
            curr = curr->next;
        }
        cout << "\n";
    }
};

// ---------------- Ticket List (Singly Linked List) ----------------

class TicketList {
    Ticket *head;
public:
    TicketList() : head(nullptr) {}

    Ticket* find(int id) {
        Ticket *curr = head;
        while (curr) {
            if (curr->ticketID == id) return curr;
            curr = curr->next;
        }
        return nullptr;
    }

    bool insertTicket(const Ticket &t) {
        if (find(t.ticketID)) return false;
        Ticket *node = new Ticket(t.ticketID, t.customerName, t.issueDescription, t.priority);
        node->next = head;
        head = node;
        return true;
    }

    bool deleteTicket(int id, Ticket &deletedCopy) {
        Ticket *curr = head, *prev = nullptr;
        while (curr && curr->ticketID != id) {
            prev = curr;
            curr = curr->next;
        }
        if (!curr) return false;
        deletedCopy = *curr;
        if (prev) prev->next = curr->next;
        else head = curr->next;
        delete curr;
        return true;
    }

    Ticket* retrieveTicket(int id) {
        return find(id);
    }

    Ticket* getHead() { return head; }

    void printAll() {
        Ticket *curr = head;
        if (!curr) {
            cout << "No tickets in the system.\n";
            return;
        }
        while (curr) {
            cout << "TicketID: " << curr->ticketID
                 << ", Customer: " << curr->customerName
                 << ", Priority: " << curr->priority
                 << ", Issue: " << curr->issueDescription << "\n";
            curr = curr->next;
        }
    }
};

// ---------------- Customer Support Ticket System ----------------

class CustomerSupportSystem {
    TicketList ticketList;
    UndoStack undoStack;
    priority_queue<PQNode> pq;
    CircularQueue cq;
    Polynomial billing1, billing2; // sample billing histories

public:
    void addTicketRecord() {
        int id, pr;
        string name, issue;
        cout << "Enter TicketID: ";
        cin >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter Customer Name: ";
        getline(cin, name);
        cout << "Enter Issue Description: ";
        getline(cin, issue);
        cout << "Enter Priority (1 = urgent): ";
        cin >> pr;

        Ticket t(id, name, issue, pr);
        if (ticketList.insertTicket(t)) {
            undoStack.push(UndoOperation("INSERT", t));
            cout << "Ticket inserted.\n";
        } else {
            cout << "TicketID already exists. Insert failed.\n";
        }
    }

    void undoTicketOperation() {
        if (undoStack.empty()) {
            cout << "No operations to undo.\n";
            return;
        }
        UndoOperation u = undoStack.top();
        undoStack.pop();

        if (u.opType == "INSERT") {
            Ticket dummy(0,"","",0);
            if (ticketList.deleteTicket(u.data.ticketID, dummy)) {
                cout << "Undo: Insert reverted, ticket removed.\n";
            } else {
                cout << "Undo failed: ticket not found.\n";
            }
        } else if (u.opType == "DELETE") {
            if (ticketList.insertTicket(u.data)) {
                cout << "Undo: Delete reverted, ticket reinserted.\n";
            } else {
                cout << "Undo failed: duplicate TicketID.\n";
            }
        }
    }

    void processPriorityTickets() {
        pq = priority_queue<PQNode>();
        Ticket *curr = ticketList.getHead();
        while (curr) {
            pq.push(PQNode{curr->priority, curr->ticketID, curr});
            curr = curr->next;
        }
        if (pq.empty()) {
            cout << "No tickets to process.\n";
            return;
        }
        cout << "Processing tickets by priority:\n";
        while (!pq.empty()) {
            PQNode top = pq.top();
            pq.pop();
            cout << "Processing TicketID: " << top.ticketID
                 << ", Priority: " << top.priority
                 << ", Customer: " << top.ref->customerName << "\n";
        }
    }

    void processRoundRobinTickets() {
        cq = CircularQueue();
        Ticket *curr = ticketList.getHead();
        while (curr) {
            cq.enqueue(curr);
            curr = curr->next;
        }
        if (cq.isEmpty()) {
            cout << "No tickets for round-robin.\n";
            return;
        }
        int quantum;
        cout << "Enter number of cycles to simulate: ";
        cin >> quantum;
        cout << "Round-robin order:\n";
        for (int i = 0; i < quantum && !cq.isEmpty(); i++) {
            Ticket *t = cq.dequeue();
            cout << "Cycle " << (i + 1) << " -> TicketID: " << t->ticketID
                 << ", Customer: " << t->customerName << "\n";
            cq.enqueue(t);
        }
    }

    void setupSampleBilling() {
        billing1 = Polynomial();
        billing2 = Polynomial();
        billing1.insertTerm(500, 3);
        billing1.insertTerm(300, 2);
        billing1.insertTerm(200, 1);

        billing2.insertTerm(450, 3);
        billing2.insertTerm(320, 2);
        billing2.insertTerm(180, 1);
    }

    void compareBillingHistory() {
        setupSampleBilling();
        cout << "Billing history 1:\n";
        billing1.print("B1(x) = ");
        cout << "Billing history 2:\n";
        billing2.print("B2(x) = ");

        Polynomial diff = Polynomial::subtract(billing1, billing2);
        cout << "Difference (B1 - B2):\n";
        diff.print("D(x) = ");
    }

    void searchTicket() {
        int id;
        cout << "Enter TicketID to retrieve: ";
        cin >> id;
        Ticket *t = ticketList.retrieveTicket(id);
        if (!t) cout << "Ticket not found.\n";
        else {
            cout << "TicketID: " << t->ticketID
                 << ", Customer: " << t->customerName
                 << ", Priority: " << t->priority
                 << ", Issue: " << t->issueDescription << "\n";
        }
    }

    void deleteTicketByID() {
        int id;
        cout << "Enter TicketID to delete: ";
        cin >> id;
        Ticket deletedCopy(0,"","",0);
        if (ticketList.deleteTicket(id, deletedCopy)) {
            undoStack.push(UndoOperation("DELETE", deletedCopy));
            cout << "Ticket deleted.\n";
        } else {
            cout << "Ticket not found.\n";
        }
    }

    void displayAllTickets() {
        ticketList.printAll();
    }
};

// ---------------- Main ----------------

int main() {

    CustomerSupportSystem system;
    int choice;

    while (true) {
        cout << "\n===== Customer Support Ticket System =====\n";
        cout << "1. Add Ticket Record\n";
        cout << "2. Delete Ticket by ID\n";
        cout << "3. Search Ticket by ID\n";
        cout << "4. Display All Tickets\n";
        cout << "5. Undo Last Operation\n";
        cout << "6. Process Priority Tickets\n";
        cout << "7. Process Round-Robin Tickets\n";
        cout << "8. Compare Billing History (Polynomial)\n";
        cout << "9. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: system.addTicketRecord(); break;
            case 2: system.deleteTicketByID(); break;
            case 3: system.searchTicket(); break;
            case 4: system.displayAllTickets(); break;
            case 5: system.undoTicketOperation(); break;
            case 6: system.processPriorityTickets(); break;
            case 7: system.processRoundRobinTickets(); break;
            case 8: system.compareBillingHistory(); break;
            case 9: cout << "Exiting...\n"; return 0;
            default: cout << "Invalid choice.\n";
        }
    }
}
