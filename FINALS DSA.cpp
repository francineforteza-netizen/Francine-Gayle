
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;

// 1. PRODUCT STRUCTURE
struct Product {
    int id;
    string name;
    string category;
    string brand;
    double price;
    int stockQuantity;
};

// 2. ORDER STRUCTURE
struct Order {
    int orderID;
    int productID;
    int quantity;
    double total;
};

// 3. BINARY SEARCH TREE
struct TreeNode {
    Product data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(Product p) {
        data = p;
        left = right = nullptr;
    }
};

TreeNode* insertBST(TreeNode* node, Product p) {
    if (node == nullptr)
        return new TreeNode(p);

    if (p.id < node->data.id)
        node->left = insertBST(node->left, p);
    else if (p.id > node->data.id)
        node->right = insertBST(node->right, p);

    return node;
}

TreeNode* searchBST(TreeNode* node, int id) {
    if (node == nullptr || node->data.id == id)
        return node;

    if (id < node->data.id)
        return searchBST(node->left, id);

    return searchBST(node->right, id);
}

void deleteBST(TreeNode* node) {
    if (node == nullptr)
        return;

    deleteBST(node->left);
    deleteBST(node->right);
    delete node;
}

// 4. GLOBAL DATA STRUCTURES
vector<Product> products;
queue<Order> checkoutQueue;
stack<string> activityStack;
TreeNode* root = nullptr;
int nextOrderID = 1001;

void rebuildBST();

// 5. INITIAL FOOD AND BEVERAGE PRODUCTS
void initializeProducts() {
    products.push_back({101, "Bottled Water", "Beverages", "Nature Spring", 15.00, 100});
    products.push_back({102, "Cola Soft Drink", "Beverages", "Coca-Cola", 25.00, 80});
    products.push_back({103, "Orange Juice", "Beverages", "Minute Maid", 35.00, 60});
    products.push_back({104, "Instant Noodles", "Food", "Lucky Me", 18.00, 120});
    products.push_back({105, "Potato Chips", "Snacks", "Piattos", 20.00, 75});
    products.push_back({106, "Chocolate Bar", "Snacks", "KitKat", 30.00, 50});

    rebuildBST();
}

void rebuildBST() {
    deleteBST(root);
    root = nullptr;

    for (const Product& p : products)
        root = insertBST(root, p);
}

// 6. DISPLAY PRODUCTS
void displayProducts() {
    cout << "\n========================= FOOD AND BEVERAGE INVENTORY =========================\n";

    cout << left
         << setw(8) << "ID"
         << setw(22) << "NAME"
         << setw(16) << "CATEGORY"
         << setw(18) << "BRAND"
         << setw(12) << "PRICE"
         << "STOCK\n";

    cout << string(88, '-') << "\n";

    for (const Product& p : products) {
        cout << left
             << setw(8) << p.id
             << setw(22) << p.name
             << setw(16) << p.category
             << setw(18) << p.brand
             << "P" << setw(11) << fixed << setprecision(2) << p.price
             << p.stockQuantity << "\n";
    }
}

// 7. ADD NEW PRODUCT
void addProduct() {
    Product p;

    cout << "\n========== ADD NEW FOOD OR BEVERAGE PRODUCT ==========\n";
    cout << "Enter Product ID: ";

    if (!(cin >> p.id)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid product ID!\n";
        return;
    }

    for (const Product& item : products) {
        if (item.id == p.id) {
            cout << "Product ID already exists!\n";
            return;
        }
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter Product Name: ";
    getline(cin, p.name);

    cout << "Enter Category (Food/Snacks/Beverages): ";
    getline(cin, p.category);

    cout << "Enter Brand: ";
    getline(cin, p.brand);

    cout << "Enter Price: ";

    if (!(cin >> p.price)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid price!\n";
        return;
    }

    cout << "Enter Stock Quantity: ";

    if (!(cin >> p.stockQuantity)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid stock quantity!\n";
        return;
    }

    if (p.price < 0 || p.stockQuantity < 0) {
        cout << "Invalid price or stock quantity!\n";
        return;
    }

    products.push_back(p);
    rebuildBST();
    activityStack.push("Added product: " + p.name);

    cout << "\nProduct successfully added!\n";
}

// 8. SEARCH PRODUCT - LINEAR SEARCH
void searchProduct() {
    string searchName;
    bool found = false;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\nEnter product name: ";
    getline(cin, searchName);

    for (const Product& p : products) {
        if (p.name.find(searchName) != string::npos) {
            cout << "\nProduct Found!\n";
            cout << "ID: " << p.id << "\n";
            cout << "Name: " << p.name << "\n";
            cout << "Category: " << p.category << "\n";
            cout << "Brand: " << p.brand << "\n";
            cout << "Price: P" << fixed << setprecision(2) << p.price << "\n";
            cout << "Stock: " << p.stockQuantity << "\n";

            found = true;
        }
    }

    if (!found)
        cout << "\nProduct not found!\n";
}

// 9. FILTER BY BRAND
void filterByBrand() {
    string brand;
    bool found = false;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\nEnter Brand: ";
    getline(cin, brand);

    cout << "\n========== BRAND RESULTS ==========\n";

    for (const Product& p : products) {
        if (p.brand == brand) {
            cout << "\nID: " << p.id;
            cout << "\nName: " << p.name;
            cout << "\nCategory: " << p.category;
            cout << "\nBrand: " << p.brand;
            cout << "\nPrice: P" << fixed << setprecision(2) << p.price;
            cout << "\nStock: " << p.stockQuantity << "\n";

            found = true;
        }
    }

    if (!found)
        cout << "\nNo products found for this brand.\n";
}

// 10. SORT PRODUCTS - INSERTION SORT
void sortProducts() {
    for (int i = 1; i < static_cast<int>(products.size()); i++) {
        Product key = products[i];
        int j = i - 1;

        while (j >= 0 && products[j].price > key.price) {
            products[j + 1] = products[j];
            j--;
        }

        products[j + 1] = key;
    }

    cout << "\nProducts sorted by price (Lowest to Highest)!\n";

    displayProducts();

    activityStack.push("Sorted products by price");
}

// 11. BINARY SEARCH TREE SEARCH
void searchProductByID() {
    int id;

    cout << "\nEnter Product ID: ";

    if (!(cin >> id)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid product ID!\n";
        return;
    }

    TreeNode* result = searchBST(root, id);

    if (result != nullptr) {
        cout << "\nProduct Found Using BST!\n";
        cout << "ID: " << result->data.id << "\n";
        cout << "Name: " << result->data.name << "\n";
        cout << "Category: " << result->data.category << "\n";
        cout << "Brand: " << result->data.brand << "\n";
        cout << "Price: P" << fixed << setprecision(2) << result->data.price << "\n";
        cout << "Stock: " << result->data.stockQuantity << "\n";
    } else {
        cout << "\nProduct ID not found!\n";
    }
}

// 12. CUSTOMER CHECKOUT QUEUE
void addToCheckoutQueue() {
    int id, quantity;

    cout << "\n========== CUSTOMER CHECKOUT ==========\n";
    cout << "Enter Product ID: ";

    if (!(cin >> id)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid product ID!\n";
        return;
    }

    cout << "Enter Quantity: ";

    if (!(cin >> quantity)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid quantity!\n";
        return;
    }

    if (quantity <= 0) {
        cout << "Invalid quantity!\n";
        return;
    }

    for (const Product& p : products) {
        if (p.id == id) {
            int reservedQuantity = 0;
            queue<Order> temp = checkoutQueue;

            while (!temp.empty()) {
                if (temp.front().productID == id)
                    reservedQuantity += temp.front().quantity;

                temp.pop();
            }

            if (p.stockQuantity - reservedQuantity < quantity) {
                cout << "\nNot enough available stock!\n";
                return;
            }

            Order order;
            order.orderID = nextOrderID++;
            order.productID = id;
            order.quantity = quantity;
            order.total = p.price * quantity;

            checkoutQueue.push(order);

            cout << "\nOrder added to checkout queue!\n";
            cout << "Order ID: " << order.orderID << "\n";
            cout << "Product: " << p.name << "\n";
            cout << "Total: P" << fixed << setprecision(2) << order.total << "\n";

            activityStack.push("New checkout order");

            return;
        }
    }

    cout << "\nProduct not found!\n";
}

// 13. PROCESS CHECKOUT QUEUE
void processCheckout() {
    if (checkoutQueue.empty()) {
        cout << "\nNo pending orders!\n";
        return;
    }

    Order order = checkoutQueue.front();

    for (Product& p : products) {
        if (p.id == order.productID) {
            if (p.stockQuantity >= order.quantity) {
                p.stockQuantity -= order.quantity;

                cout << "\n========== ORDER PROCESSED ==========\n";
                cout << "Order ID: " << order.orderID << "\n";
                cout << "Product: " << p.name << "\n";
                cout << "Quantity: " << order.quantity << "\n";
                cout << "Total Payment: P" << fixed << setprecision(2) << order.total << "\n";
                cout << "Remaining Stock: " << p.stockQuantity << "\n";

                checkoutQueue.pop();

                activityStack.push("Processed order #" + to_string(order.orderID));

                return;
            } else {
                cout << "\nInsufficient stock. Order remains pending.\n";
                return;
            }
        }
    }

    cout << "\nProduct no longer exists. Order cancelled.\n";
    checkoutQueue.pop();
}

// 14. DISPLAY CHECKOUT QUEUE
void displayCheckoutQueue() {
    if (checkoutQueue.empty()) {
        cout << "\nCheckout queue is empty!\n";
        return;
    }

    queue<Order> temp = checkoutQueue;

    cout << "\n========== PENDING ORDERS ==========\n";

    while (!temp.empty()) {
        Order order = temp.front();

        cout << "Order ID: " << order.orderID
             << " | Product ID: " << order.productID
             << " | Quantity: " << order.quantity
             << " | Total: P" << fixed << setprecision(2) << order.total << "\n";

        temp.pop();
    }
}

// 15. STACK - RECENT ACTIVITIES
void displayActivities() {
    if (activityStack.empty()) {
        cout << "\nNo recent activities!\n";
        return;
    }

    stack<string> temp = activityStack;

    cout << "\n========== RECENT ACTIVITIES ==========\n";

    while (!temp.empty()) {
        cout << "- " << temp.top() << "\n";
        temp.pop();
    }
}

// 16. DELETE PRODUCT
void deleteProduct() {
    int id;

    cout << "\nEnter Product ID to delete: ";

    if (!(cin >> id)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid product ID!\n";
        return;
    }

    for (auto it = products.begin(); it != products.end(); ++it) {
        if (it->id == id) {
            string deletedName = it->name;

            products.erase(it);
            rebuildBST();

            activityStack.push("Deleted product: " + deletedName);

            cout << "\nProduct successfully deleted!\n";

            return;
        }
    }

    cout << "\nProduct not found!\n";
}

// 17. MAIN MENU
int main() {
    initializeProducts();

    int choice;

    do {
        cout << "\n\n";
        cout << "====================================================\n";
        cout << "       FOOD AND BEVERAGE INVENTORY SYSTEM\n";
        cout << "====================================================\n";
        cout << "1. Display All Products\n";
        cout << "2. Add New Product\n";
        cout << "3. Search Product (Linear Search)\n";
        cout << "4. Filter Products by Brand\n";
        cout << "5. Sort Products by Price\n";
        cout << "6. Search Product by ID (BST)\n";
        cout << "7. Add Customer Order to Queue\n";
        cout << "8. Process Checkout\n";
        cout << "9. Display Pending Orders\n";
        cout << "10. View Recent Activities (Stack)\n";
        cout << "11. Delete Product\n";
        cout << "0. Exit System\n";
        cout << "====================================================\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input!\n";
            continue;
        }

        switch (choice) {
            case 1:
                displayProducts();
                break;

            case 2:
                addProduct();
                break;

            case 3:
                searchProduct();
                break;

            case 4:
                filterByBrand();
                break;

            case 5:
                sortProducts();
                break;

            case 6:
                searchProductByID();
                break;

            case 7:
                addToCheckoutQueue();
                break;

            case 8:
                processCheckout();
                break;

            case 9:
                displayCheckoutQueue();
                break;

            case 10:
                displayActivities();
                break;

            case 11:
                deleteProduct();
                break;

            case 0:
                cout << "\nThank you for using the system!\n";
                break;

            default:
                cout << "\nInvalid choice! Try again.\n";
        }

    } while (choice != 0);

    deleteBST(root);
    root = nullptr;

    return 0;
}
