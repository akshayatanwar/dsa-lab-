#include <bits/stdc++.h>
using namespace std;

struct InventoryItem {
    int itemID;
    string itemName;
    int quantity;
    float price;
};

class InventorySystem {
private:
    // Core array-based storage
    static const int MAX_ITEMS = 1000;
    InventoryItem items[MAX_ITEMS];
    int n; // current number of items

    // PriceQuantityTable as 2D array: row-major (row = item index, col = 0:price, 1:quantity)
    float priceQuantityTable[2][MAX_ITEMS]; // row-major access: priceQuantityTable[row][col]

    // Sparse representation for rarely restocked items:
    // store only (indexInItemsArray, quantity) pairs where quantity is small
    struct SparseNode {
        int index;     // index in items[]
        int quantity;  // quantity
    };
    SparseNode sparse[MAX_ITEMS];
    int sparseCount;

    int findIndexByID(int id) {
        for (int i = 0; i < n; i++) {
            if (items[i].itemID == id)
                return i;
        }
        return -1;
    }

    int findIndexByName(const string &name) {
        for (int i = 0; i < n; i++) {
            if (items[i].itemName == name)
                return i;
        }
        return -1;
    }

    void rebuildSparse(int threshold) {
        sparseCount = 0;
        for (int i = 0; i < n; i++) {
            if (items[i].quantity <= threshold) {
                sparse[sparseCount].index = i;
                sparse[sparseCount].quantity = items[i].quantity;
                sparseCount++;
            }
        }
    }

    void refreshPriceQuantityRowMajor() {
        // row-major: priceQuantityTable[0][i] -> price, priceQuantityTable[1][i] -> quantity
        for (int i = 0; i < n; i++) {
            priceQuantityTable[0][i] = items[i].price;
            priceQuantityTable[1][i] = static_cast<float>(items[i].quantity);
        }
    }

public:
    InventorySystem() {
        n = 0;
        sparseCount = 0;
    }

    // ADT methods

    // insertItem: O(n) worst case due to duplicate check, O(1) space (excluding array)
    bool insertItem(const InventoryItem &item) {
        if (n >= MAX_ITEMS) {
            cout << "Inventory full. Cannot insert more items.\n";
            return false;
        }
        if (findIndexByID(item.itemID) != -1) {
            cout << "Duplicate ItemID. Insert failed.\n";
            return false;
        }
        items[n] = item;
        n++;
        refreshPriceQuantityRowMajor();
        return true;
    }

    // deleteItem: O(n) time due to search + shift; O(1) extra space
    bool deleteItem(int itemID) {
        int idx = findIndexByID(itemID);
        if (idx == -1) {
            cout << "Item not found.\n";
            return false;
        }
        for (int i = idx; i < n - 1; i++) {
            items[i] = items[i + 1];
        }
        n--;
        refreshPriceQuantityRowMajor();
        return true;
    }

    // searchItem by ID: O(n) time, O(1) space
    int searchItemByID(int itemID) {
        return findIndexByID(itemID);
    }

    // searchItem by name: O(n) time, O(1) space
    int searchItemByName(const string &name) {
        return findIndexByName(name);
    }

    void printItem(int idx) {
        if (idx < 0 || idx >= n) {
            cout << "Invalid index.\n";
            return;
        }
        cout << "ItemID: " << items[idx].itemID
             << ", Name: " << items[idx].itemName
             << ", Quantity: " << items[idx].quantity
             << ", Price: " << items[idx].price << "\n";
    }

    // Inventory Management System methods

    // addItemRecord: wrapper around insertItem
    void addItemRecord() {
        InventoryItem it;
        cout << "Enter ItemID: ";
        cin >> it.itemID;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter Item Name: ";
        getline(cin, it.itemName);
        cout << "Enter Quantity: ";
        cin >> it.quantity;
        cout << "Enter Price: ";
        cin >> it.price;
        if (insertItem(it)) {
            cout << "Item inserted successfully.\n";
        }
    }

    // removeItemRecord: wrapper around deleteItem
    void removeItemRecord() {
        int id;
        cout << "Enter ItemID to delete: ";
        cin >> id;
        if (deleteItem(id)) {
            cout << "Item deleted successfully.\n";
        }
    }

    void searchByItem() {
        int choice;
        cout << "Search by: 1. ItemID  2. ItemName\n";
        cin >> choice;
        if (choice == 1) {
            int id;
            cout << "Enter ItemID: ";
            cin >> id;
            int idx = searchItemByID(id);
            if (idx != -1) printItem(idx);
            else cout << "Item not found.\n";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            string name;
            cout << "Enter Item Name: ";
            getline(cin, name);
            int idx = searchItemByName(name);
            if (idx != -1) printItem(idx);
            else cout << "Item not found.\n";
        }
    }

    // Row-major vs column-major demonstration
    void managePriceQuantity() {
        if (n == 0) {
            cout << "No items to display.\n";
            return;
        }
        refreshPriceQuantityRowMajor();
        cout << "PriceQuantityTable in row-major (row=0 price, row=1 quantity):\n";
        for (int row = 0; row < 2; row++) {
            for (int col = 0; col < n; col++) {
                cout << priceQuantityTable[row][col] << " ";
            }
            cout << "\n";
        }

        cout << "Demonstrating column-major access (conceptual):\n";
        for (int col = 0; col < n; col++) {
            float price = priceQuantityTable[0][col];
            float qty = priceQuantityTable[1][col];
            cout << "Item " << col << " -> Price: " << price << ", Qty: " << qty << "\n";
        }
    }

    // Sparse representation for rarely restocked products (e.g., quantity <= threshold)
    void optimizeSparseStorage() {
        int threshold;
        cout << "Enter quantity threshold for 'rarely restocked': ";
        cin >> threshold;
        rebuildSparse(threshold);
        cout << "Sparse list (index, quantity):\n";
        for (int i = 0; i < sparseCount; i++) {
            cout << "(" << sparse[i].index << ", " << sparse[i].quantity
                 << ") -> ItemID: " << items[sparse[i].index].itemID
                 << ", Name: " << items[sparse[i].index].itemName << "\n";
        }
    }

    void displayAllItems() {
        if (n == 0) {
            cout << "Inventory empty.\n";
            return;
        }
        for (int i = 0; i < n; i++) {
            printItem(i);
        }
    }
};

int main() {

    InventorySystem system;
    int choice;

    while (true) {
        cout << "\n===== Grocery Inventory System (Array-based) =====\n";
        cout << "1. Add Item Record\n";
        cout << "2. Remove Item Record\n";
        cout << "3. Search Item\n";
        cout << "4. Display All Items\n";
        cout << "5. Manage Price & Quantity (Row/Column major)\n";
        cout << "6. Optimize Sparse Storage\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: system.addItemRecord(); break;
            case 2: system.removeItemRecord(); break;
            case 3: system.searchByItem(); break;
            case 4: system.displayAllItems(); break;
            case 5: system.managePriceQuantity(); break;
            case 6: system.optimizeSparseStorage(); break;
            case 7: cout << "Exiting...\n"; return 0;
            default: cout << "Invalid choice.\n";
        }
    }
}
