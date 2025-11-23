#include <bits/stdc++.h>
using namespace std;

// ---------------- Student Record ADT ----------------

struct Student {
    int studentID;
    string studentName;
    float grade;
    string courseDetails;
};

// ---------------- Hash Table with Linear Probing ----------------

class HashTable {
    static const int TABLE_SIZE = 101; // prime
    Student table[TABLE_SIZE];
    bool occupied[TABLE_SIZE];
    bool deletedFlag[TABLE_SIZE];

    int hashFunc(int key) const {
        return key % TABLE_SIZE;
    }

public:
    HashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            occupied[i] = false;
            deletedFlag[i] = false;
        }
    }

    bool insertStudent(const Student &s) {
        int idx = hashFunc(s.studentID);
        int start = idx;
        do {
            if (!occupied[idx] || deletedFlag[idx]) {
                table[idx] = s;
                occupied[idx] = true;
                deletedFlag[idx] = false;
                return true;
            }
            if (occupied[idx] && table[idx].studentID == s.studentID) {
                return false; // duplicate ID
            }
            idx = (idx + 1) % TABLE_SIZE;
        } while (idx != start);
        return false;
    }

    bool searchByID(int id, Student &result) const {
        int idx = hashFunc(id);
        int start = idx;
        do {
            if (!occupied[idx] && !deletedFlag[idx]) {
                return false;
            }
            if (occupied[idx] && table[idx].studentID == id) {
                result = table[idx];
                return true;
            }
            idx = (idx + 1) % TABLE_SIZE;
        } while (idx != start);
        return false;
    }

    void getAllStudents(vector<Student> &out) const {
        out.clear();
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (occupied[i] && !deletedFlag[i]) {
                out.push_back(table[i]);
            }
        }
    }
};

// ---------------- Searching Algorithms ----------------

int sequentialSearchByID(const vector<Student> &arr, int id) {
    for (int i = 0; i < (int)arr.size(); i++) {
        if (arr[i].studentID == id) return i;
    }
    return -1;
}

int sequentialSearchByName(const vector<Student> &arr, const string &name) {
    for (int i = 0; i < (int)arr.size(); i++) {
        if (arr[i].studentName == name) return i;
    }
    return -1;
}

int binarySearchByID(const vector<Student> &arr, int id) {
    int low = 0, high = (int)arr.size() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid].studentID == id) return mid;
        else if (arr[mid].studentID < id) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

// ---------------- Sorting Algorithms (by grade) ----------------

void bubbleSort(vector<Student> &arr) {
    int n = (int)arr.size();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; j++) {
            if (arr[j].grade > arr[j + 1].grade) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

void insertionSort(vector<Student> &arr) {
    int n = (int)arr.size();
    for (int i = 1; i < n; i++) {
        Student key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].grade > key.grade) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void merge(vector<Student> &arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<Student> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i].grade <= R[j].grade) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(vector<Student> &arr, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
}

int partitionQuick(vector<Student> &arr, int low, int high) {
    float pivot = arr[high].grade;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j].grade <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<Student> &arr, int low, int high) {
    if (low < high) {
        int pi = partitionQuick(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// ---------------- Heap Sort for Ranking (by grade descending) ----------------

void heapify(vector<Student> &arr, int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < n && arr[l].grade > arr[largest].grade) largest = l;
    if (r < n && arr[r].grade > arr[largest].grade) largest = r;
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<Student> &arr) {
    int n = (int)arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
    reverse(arr.begin(), arr.end());
}

// ---------------- Student Performance Tracker System ----------------

class StudentPerformanceTracker {
    HashTable hashTable;
    vector<Student> studentArray;

public:
    void syncFromHash() {
        studentArray.clear();
        hashTable.getAllStudents(studentArray);
    }

    void addStudentRecord() {
        Student s;
        cout << "Enter StudentID: ";
        cin >> s.studentID;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter Student Name: ";
        getline(cin, s.studentName);
        cout << "Enter Grade: ";
        cin >> s.grade;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter Course Details: ";
        getline(cin, s.courseDetails);

        if (hashTable.insertStudent(s)) {
            cout << "Student inserted into hash table.\n";
        } else {
            cout << "Insert failed (duplicate or table full).\n";
        }
    }

    void searchByStudent() {
        int choice;
        cout << "Search by: 1. ID (hash)  2. ID (sequential)  3. Name (sequential)  4. ID (binary)\n";
        cin >> choice;
        if (choice == 1) {
            int id;
            cout << "Enter StudentID: ";
            cin >> id;
            Student s;
            if (hashTable.searchByID(id, s)) {
                printStudent(s);
            } else {
                cout << "Student not found in hash table.\n";
            }
        } else if (choice == 2) {
            syncFromHash();
            int id;
            cout << "Enter StudentID: ";
            cin >> id;
            int idx = sequentialSearchByID(studentArray, id);
            if (idx != -1) printStudent(studentArray[idx]);
            else cout << "Student not found (sequential).\n";
        } else if (choice == 3) {
            syncFromHash();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            string name;
            cout << "Enter Student Name: ";
            getline(cin, name);
            int idx = sequentialSearchByName(studentArray, name);
            if (idx != -1) printStudent(studentArray[idx]);
            else cout << "Student not found (sequential by name).\n";
        } else if (choice == 4) {
            syncFromHash();
            sort(studentArray.begin(), studentArray.end(),
                 [](const Student &a, const Student &b) {
                     return a.studentID < b.studentID;
                 });
            int id;
            cout << "Enter StudentID: ";
            cin >> id;
            int idx = binarySearchByID(studentArray, id);
            if (idx != -1) printStudent(studentArray[idx]);
            else cout << "Student not found (binary search).\n";
        } else {
            cout << "Invalid choice.\n";
        }
    }

    void sortByGrades() {
        syncFromHash();
        if (studentArray.empty()) {
            cout << "No records to sort.\n";
            return;
        }
        int choice;
        cout << "Sort by grades using: 1.Bubble  2.Insertion  3.Merge  4.Quick\n";
        cin >> choice;
        vector<Student> temp = studentArray;
        switch (choice) {
            case 1: bubbleSort(temp); break;
            case 2: insertionSort(temp); break;
            case 3: mergeSort(temp, 0, (int)temp.size() - 1); break;
            case 4: quickSort(temp, 0, (int)temp.size() - 1); break;
            default: cout << "Invalid choice.\n"; return;
        }
        cout << "Sorted by grades (ascending):\n";
        for (const auto &s : temp) printStudent(s);
    }

    void rankByPerformance() {
        syncFromHash();
        if (studentArray.empty()) {
            cout << "No records to rank.\n";
            return;
        }
        vector<Student> temp = studentArray;
        heapSort(temp);
        cout << "Ranking by performance (highest grade first):\n";
        int rank = 1;
        for (const auto &s : temp) {
            cout << "Rank " << rank++ << ": ";
            printStudent(s);
        }
    }

    void compareSortingComplexity() {
        cout << "Theoretical time complexity of sorting algorithms (n = number of students):\n";
        cout << "Bubble Sort: Best O(n), Average/Worst O(n^2)\n";
        cout << "Insertion Sort: Best O(n), Average/Worst O(n^2)\n";
        cout << "Merge Sort: Best/Average/Worst O(n log n)\n";
        cout << "Quick Sort: Best/Average O(n log n), Worst O(n^2)\n";
        cout << "Heap Sort: Best/Average/Worst O(n log n)\n";
    }

    void displayAllStudents() {
        syncFromHash();
        if (studentArray.empty()) {
            cout << "No students.\n";
            return;
        }
        for (const auto &s : studentArray) {
            printStudent(s);
        }
    }

private:
    void printStudent(const Student &s) const {
        cout << "ID: " << s.studentID
             << ", Name: " << s.studentName
             << ", Grade: " << s.grade
             << ", Course: " << s.courseDetails << "\n";
    }
};

// ---------------- Main ----------------

int main() {

    StudentPerformanceTracker tracker;
    int choice;

    while (true) {
        cout << "\n===== Student Performance Tracker =====\n";
        cout << "1. Add Student Record\n";
        cout << "2. Search Student\n";
        cout << "3. Sort by Grades\n";
        cout << "4. Rank by Performance (Heap Sort)\n";
        cout << "5. Compare Sorting Complexity\n";
        cout << "6. Display All Students\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: tracker.addStudentRecord(); break;
            case 2: tracker.searchByStudent(); break;
            case 3: tracker.sortByGrades(); break;
            case 4: tracker.rankByPerformance(); break;
            case 5: tracker.compareSortingComplexity(); break;
            case 6: tracker.displayAllStudents(); break;
            case 7: cout << "Exiting...\n"; return 0;
            default: cout << "Invalid choice.\n";
        }
    }
}
