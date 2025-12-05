#include <iostream>
#include <fstream>   // For file handling (CSV)
#include <string>
#include <vector>
#include <algorithm> // For max()
#include <sstream>   // For parsing CSV lines

using namespace std;

// 1. DATA MODELS (With Comparison Logic)

// 1. Individual Node
struct Individual {
    int userID;
    string name;
    double income;
    int familyID;

    // Overload < operator for AVL sorting
    bool operator<(const Individual& other) const {
        return userID < other.userID;
    }
    bool operator>(const Individual& other) const {
        return userID > other.userID;
    }
    bool operator==(const Individual& other) const {
        return userID == other.userID;
    }
};

// 2. Family Node
struct Family {
    int familyID;
    string familyName;
    vector<int> memberIDs;
    double totalIncome;
    double monthlyExpense;

    // Overload < operator for AVL sorting
    bool operator<(const Family& other) const {
        return familyID < other.familyID;
    }
    bool operator>(const Family& other) const {
        return familyID > other.familyID;
    }
    bool operator==(const Family& other) const {
        return familyID == other.familyID;
    }
};

// 3. Expense Node
struct Expense {
    int expenseID;
    int userID;
    string category;
    double amount;
    string date;

    // Overload < operator for AVL sorting
    bool operator<(const Expense& other) const {
        return expenseID < other.expenseID;
    }
    bool operator>(const Expense& other) const {
        return expenseID > other.expenseID;
    }
    bool operator==(const Expense& other) const {
        return expenseID == other.expenseID;
    }
};


// 2. TEMPLATE AVL TREE IMPLEMENTATION
// Generic Template Node
template <typename T>
struct Node {
    T data;
    Node *left;
    Node *right;
    int height;
};

// --- Helper Functions ---

template <typename T>
int getHeight(Node<T>* N) {
    if (N == nullptr) return 0;
    return N->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

template <typename T>
int getBalance(Node<T>* N) {
    if (N == nullptr) return 0;
    return getHeight(N->left) - getHeight(N->right);
}

// --- Rotations ---

template <typename T>
Node<T>* rightRotate(Node<T>* y) {
    Node<T>* x = y->left;
    Node<T>* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    // Return new root
    return x;
}

template <typename T>
Node<T>* leftRotate(Node<T>* x) {
    Node<T>* y = x->right;
    Node<T>* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    // Return new root
    return y;
}

// --- Generic Insertion ---

template <typename T>
Node<T>* insert(Node<T>* node, T data) {
    // 1. Normal BST insertion
    if (node == nullptr) {
        Node<T>* newNode = new Node<T>();
        newNode->data = data;
        newNode->left = nullptr;
        newNode->right = nullptr;
        newNode->height = 1;
        return newNode;
    }

    // Overloaded operators (<, >) 
    if (data < node->data)
        node->left = insert(node->left, data);
    else if (data > node->data)
        node->right = insert(node->right, data);
    else
        return node; // No duplicates allowed

    // 2. Update height
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    // 3. Get balance factor
    int balance = getBalance(node);

    // 4. Balance the tree (4 Cases)

    // Left Left Case
    if (balance > 1 && data < node->left->data)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && data > node->right->data)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && data > node->left->data) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && data < node->right->data) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// 3. GLOBAL ROOTS
Node<Individual>* rootUsers = nullptr;
Node<Family>* rootFamilies = nullptr;
Node<Expense>* rootExpenses = nullptr;

// 4. FILE HANDLING UTILITIES

// Split a string by a comma
vector<string> split(const string &s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// --- Load Users ---
void loadUsers() {
    ifstream file("users.csv");
    string line;
    // Format: UserID, Name, Income, FamilyID
    while (getline(file, line)) {
        if (line.empty()) continue;
        vector<string> data = split(line, ',');
        
        if (data.size() >= 4) {
            Individual ind;
            ind.userID = stoi(data[0]);
            ind.name = data[1];
            ind.income = stod(data[2]);
            ind.familyID = stoi(data[3]);
            
            // Insert into AVL Tree
            rootUsers = insert(rootUsers, ind);
        }
    }
    file.close();
    cout << "Users loaded." << endl;
}

// --- Load Expenses ---
void loadExpenses() {
    ifstream file("expenses.csv");
    string line;
    // Format: ExpenseID, UserID, Category, Amount, Date
    while (getline(file, line)) {
        if (line.empty()) continue;
        vector<string> data = split(line, ',');
        
        if (data.size() >= 5) {
            Expense exp;
            exp.expenseID = stoi(data[0]);
            exp.userID = stoi(data[1]);
            exp.category = data[2];
            exp.amount = stod(data[3]);
            exp.date = data[4];
            
            // Insert into AVL Tree
            rootExpenses = insert(rootExpenses, exp);
        }
    }
    file.close();
    cout << "Expenses loaded." << endl;
}

// --- Load Families ---
void loadFamilies() {
    ifstream file("families.csv");
    string line;
    // Format: FamilyID, FamilyName, MemberIDs(1;2;3), TotalIncome, MonthlyExpense
    while (getline(file, line)) {
        if (line.empty()) continue;
        vector<string> data = split(line, ',');
        
        if (data.size() >= 5) {
            Family fam;
            fam.familyID = stoi(data[0]);
            fam.familyName = data[1];
            
            // Parse Member IDs (separated by semicolon)
            vector<string> members = split(data[2], ';');
            for (const string& m : members) {
                if(!m.empty()) fam.memberIDs.push_back(stoi(m));
            }
            
            fam.totalIncome = stod(data[3]);
            fam.monthlyExpense = stod(data[4]);
            
            // Insert into AVL Tree
            rootFamilies = insert(rootFamilies, fam);
        }
    }
    file.close();
    cout << "Families loaded." << endl;
}


// 5. SAVE DATA UTILITIES

// Save Users Recursively
void saveUserNode(Node<Individual>* root, ofstream& file) {
    if (root == nullptr) return;
    
    // 1. Go Left
    saveUserNode(root->left, file);
    
    // 2. Write Data
    file << root->data.userID << ","
         << root->data.name << ","
         << root->data.income << ","
         << root->data.familyID << "\n";
         
    // 3. Go Right
    saveUserNode(root->right, file);
}

void saveUsers() {
    ofstream file("users.csv"); // Opens in truncate mode (overwrites file)
    if (file.is_open()) {
        saveUserNode(rootUsers, file);
        file.close();
        cout << "Users saved." << endl;
    } else {
        cout << "Error opening users.csv for writing!" << endl;
    }
}

// Save Families Recursively 
void saveFamilyNode(Node<Family>* root, ofstream& file) {
    if (root == nullptr) return;
    
    saveFamilyNode(root->left, file);
    
    // Write Data
    file << root->data.familyID << ","
         << root->data.familyName << ",";
         
    // Join MemberIDs with ';'
    for (size_t i = 0; i < root->data.memberIDs.size(); i++) {
        file << root->data.memberIDs[i];
        if (i != root->data.memberIDs.size() - 1) file << ";";
    }
    
    file << "," << root->data.totalIncome << ","
         << root->data.monthlyExpense << "\n";
         
    saveFamilyNode(root->right, file);
}

void saveFamilies() {
    ofstream file("families.csv");
    if (file.is_open()) {
        saveFamilyNode(rootFamilies, file);
        file.close();
        cout << "Families saved." << endl;
    } else {
        cout << "Error opening families.csv for writing!" << endl;
    }
}

// Save Expenses Recursively 
void saveExpenseNode(Node<Expense>* root, ofstream& file) {
    if (root == nullptr) return;
    
    saveExpenseNode(root->left, file);
    
    // Write Data
    file << root->data.expenseID << ","
         << root->data.userID << ","
         << root->data.category << ","
         << root->data.amount << ","
         << root->data.date << "\n";
         
    saveExpenseNode(root->right, file);
}

void saveExpenses() {
    ofstream file("expenses.csv");
    if (file.is_open()) {
        saveExpenseNode(rootExpenses, file);
        file.close();
        cout << "Expenses saved." << endl;
    } else {
        cout << "Error opening expenses.csv for writing!" << endl;
    }
}

// Master Save Function 
void saveAllData() {
    saveUsers();
    saveFamilies();
    saveExpenses();
}

// 6. CORE LOGIC & HELPERS

// Find Maximum ID in Tree (Right-most node)
template <typename T>
int getMaxID(Node<T>* root) {
    if (root == nullptr) return 0;
    while (root->right != nullptr) {
        root = root->right;
    }
    return 0;
}

// Specializations for Max ID
int getMaxUserID(Node<Individual>* root) {
    if (root == nullptr) return 0;
    while (root->right != nullptr) root = root->right;
    return root->data.userID;
}

int getMaxFamilyID(Node<Family>* root) {
    if (root == nullptr) return 0;
    while (root->right != nullptr) root = root->right;
    return root->data.familyID;
}

int getMaxExpenseID(Node<Expense>* root) {
    if (root == nullptr) return 0;
    while (root->right != nullptr) root = root->right;
    return root->data.expenseID;
}

// Search Function 
// Returns pointer to Node if found, nullptr otherwise
template <typename T>
Node<T>* search(Node<T>* root, int id) {
    if (root == nullptr) return nullptr;
    
    // Create a dummy object with just the ID to compare
    T dummy;
    return nullptr; 
}

Node<Individual>* searchUser(Node<Individual>* root, int id) {
    if (root == nullptr || root->data.userID == id) return root;
    if (id < root->data.userID) return searchUser(root->left, id);
    return searchUser(root->right, id);
}

Node<Family>* searchFamily(Node<Family>* root, int id) {
    if (root == nullptr || root->data.familyID == id) return root;
    if (id < root->data.familyID) return searchFamily(root->left, id);
    return searchFamily(root->right, id);
}


// 7. MAIN OPERATIONS (Add/Create)
// 1. Add User 
void addUser() {
    cout << "\n--- Add New User ---" << endl;
    Individual newUser;
    
    // Auto-increment ID
    newUser.userID = getMaxUserID(rootUsers) + 1;
    
    cout << "Enter Name: ";
    cin.ignore(); 
    getline(cin, newUser.name);
    
    cout << "Enter Income: ";
    cin >> newUser.income;
    
    cout << "Enter Family ID (0 if new family needed later): ";
    cin >> newUser.familyID;
    
    // Insert and Save
    rootUsers = insert(rootUsers, newUser);
    saveUsers(); 
    
    cout << "User Added Successfully! New User ID: " << newUser.userID << endl;
}

// 2. Create Family
void createFamily() {
    cout << "\n--- Create New Family ---" << endl;
    Family newFam;
    
    newFam.familyID = getMaxFamilyID(rootFamilies) + 1;
    
    cout << "Enter Family Name: ";
    cin.ignore();
    getline(cin, newFam.familyName);
    
    newFam.totalIncome = 0;
    newFam.monthlyExpense = 0;
    
    cout << "How many members (1-4)? ";
    int count;
    cin >> count;
    
    if (count < 1 || count > 4) {
        cout << "Invalid number of members! Creation aborted." << endl;
        return;
    }
    
    for (int i = 0; i < count; i++) {
        int uid;
        cout << "Enter User ID for member " << (i+1) << ": ";
        cin >> uid;
        
        // Validate User exists
        Node<Individual>* uNode = searchUser(rootUsers, uid);
        if (uNode != nullptr) {
            newFam.memberIDs.push_back(uid);
            // Update User's family ID link
            uNode->data.familyID = newFam.familyID;
            // Add to family totals
            newFam.totalIncome += uNode->data.income;
        } else {
            cout << "User ID " << uid << " not found! Skipping." << endl;
        }
    }
    
    // Insert and Save
    rootFamilies = insert(rootFamilies, newFam);
    saveFamilies(); // Save new family
    saveUsers();    // Save users (because we updated their familyID)
    
    cout << "Family Created Successfully! Family ID: " << newFam.familyID << endl;
}

// 3. Add Expense
void addExpense() {
    cout << "\n--- Add New Expense ---" << endl;
    Expense newExp;
    
    newExp.expenseID = getMaxExpenseID(rootExpenses) + 1;
    
    cout << "Enter User ID who spent: ";
    cin >> newExp.userID;
    
    // Validate User
    Node<Individual>* uNode = searchUser(rootUsers, newExp.userID);
    if (uNode == nullptr) {
        cout << "User not found! Aborting." << endl;
        return;
    }
    
    cout << "Enter Category (Rent/Utility/Grocery/Stationary/Leisure): ";
    cin >> newExp.category;
    
    cout << "Enter Amount: ";
    cin >> newExp.amount;
    
    cout << "Enter Date (DD-MM-YYYY): ";
    cin >> newExp.date;
    
    // Update Family Expense Totals
    int famID = uNode->data.familyID;
    Node<Family>* fNode = searchFamily(rootFamilies, famID);
    if (fNode != nullptr) {
        fNode->data.monthlyExpense += newExp.amount;
        if (fNode->data.monthlyExpense > fNode->data.totalIncome) {
            cout << "WARNING: Family expenses have exceeded total income!" << endl;
        }
    }
    
    // Insert and Save
    rootExpenses = insert(rootExpenses, newExp);
    saveExpenses();
    saveFamilies(); // Save updated totals
    
    cout << "Expense Added Successfully! ID: " << newExp.expenseID << endl;
}

// 8. ANALYTICS & REPORTING
// Date Comparison
// Returns -1 if d1 < d2, 0 if equal, 1 if d1 > d2
int compareDates(string d1, string d2) {
    // Format: DD-MM-YYYY
    if (d1 == d2) return 0;
    
    // Parse strings
    int day1 = stoi(d1.substr(0, 2));
    int mon1 = stoi(d1.substr(3, 2));
    int year1 = stoi(d1.substr(6, 4));
    
    int day2 = stoi(d2.substr(0, 2));
    int mon2 = stoi(d2.substr(3, 2));
    int year2 = stoi(d2.substr(6, 4));
    
    if (year1 != year2) return (year1 < year2) ? -1 : 1;
    if (mon1 != mon2) return (mon1 < mon2) ? -1 : 1;
    return (day1 < day2) ? -1 : 1;
}

// Check if User belongs to Family 
bool isUserInFamily(int userID, int familyID) {
    Node<Individual>* u = searchUser(rootUsers, userID);
    if (u != nullptr && u->data.familyID == familyID) return true;
    return false;
}

// 4. Get Total Family Expenses
void getTotalExpense() {
    int famID;
    cout << "\nEnter Family ID to check: ";
    cin >> famID;
    
    Node<Family>* fNode = searchFamily(rootFamilies, famID);
    if (fNode == nullptr) {
        cout << "Family not found!" << endl;
        return;
    }
    
    cout << "\n--- Family Status: " << fNode->data.familyName << " ---" << endl;
    cout << "Total Income: " << fNode->data.totalIncome << endl;
    cout << "Total Expense: " << fNode->data.monthlyExpense << endl;
    
    if (fNode->data.monthlyExpense <= fNode->data.totalIncome) {
        cout << "Status: Within Budget (Savings: " 
             << (fNode->data.totalIncome - fNode->data.monthlyExpense) << ")" << endl;
    } else {
        cout << "Status: OVER BUDGET! (Deficit: " 
             << (fNode->data.monthlyExpense - fNode->data.totalIncome) << ")" << endl;
    }
}

// Traverse and Accumulate Category Expenses 
struct UserExpense {
    string userName;
    double amount;
    
    // For sorting
    bool operator<(const UserExpense& other) const {
        return amount > other.amount; // Descending order
    }
};

void collectCategoryExpenses(Node<Expense>* root, int familyID, string category, 
                             vector<UserExpense>& results, double& totalCatExpense) {
    if (root == nullptr) return;
    
    collectCategoryExpenses(root->left, familyID, category, results, totalCatExpense);
    
    // Check if expense matches criteria
    if (root->data.category == category) {
        if (isUserInFamily(root->data.userID, familyID)) {
            totalCatExpense += root->data.amount;
            
            // Find User Name
            Node<Individual>* u = searchUser(rootUsers, root->data.userID);
            string uName = (u != nullptr) ? u->data.name : "Unknown";
            
            // Add to results (combine if user already in list)
            bool found = false;
            for (auto& ue : results) {
                if (ue.userName == uName) {
                    ue.amount += root->data.amount;
                    found = true;
                    break;
                }
            }
            if (!found) {
                results.push_back({uName, root->data.amount});
            }
        }
    }
    
    collectCategoryExpenses(root->right, familyID, category, results, totalCatExpense);
}

// 5. Get Categorical Expense
void getCategoricalExpense() {
    int famID;
    string cat;
    cout << "\nEnter Family ID: ";
    cin >> famID;
    cout << "Enter Category (Rent/Utility/Grocery/Stationary/Leisure): ";
    cin >> cat;
    
    vector<UserExpense> results;
    double total = 0;
    
    collectCategoryExpenses(rootExpenses, famID, cat, results, total);
    
    sort(results.begin(), results.end()); // Sort descending
    
    cout << "\n--- " << cat << " Expenses for Family " << famID << " ---" << endl;
    cout << "Total Family Spend: " << total << endl;
    cout << "Individual Contributions:" << endl;
    for (const auto& r : results) {
        cout << " - " << r.userName << ": " << r.amount << endl;
    }
}

// 6. Get Individual Expense (Sorted by Category) 
struct CatExpense {
    string category;
    double amount;
    bool operator<(const CatExpense& other) const {
        return amount > other.amount; // Descending
    }
};

void collectUserExpenses(Node<Expense>* root, int userID, 
                         vector<CatExpense>& results, double& totalUserExpense) {
    if (root == nullptr) return;
    
    collectUserExpenses(root->left, userID, results, totalUserExpense);
    
    if (root->data.userID == userID) {
        totalUserExpense += root->data.amount;
        
        // Add/Merge category
        bool found = false;
        for (auto& ce : results) {
            if (ce.category == root->data.category) {
                ce.amount += root->data.amount;
                found = true;
                break;
            }
        }
        if (!found) results.push_back({root->data.category, root->data.amount});
    }
    
    collectUserExpenses(root->right, userID, results, totalUserExpense);
}

void getIndividualExpense() {
    int uid;
    cout << "\nEnter User ID: ";
    cin >> uid;
    
    vector<CatExpense> results;
    double total = 0;
    
    collectUserExpenses(rootExpenses, uid, results, total);
    sort(results.begin(), results.end());
    
    cout << "\n--- Expenses for User " << uid << " ---" << endl;
    cout << "Total Monthly Expense: " << total << endl;
    cout << "Breakdown:" << endl;
    for (const auto& r : results) {
        cout << " - " << r.category << ": " << r.amount << endl;
    }
}

// 7. Get Expenses in Date Range 
void printRangeExpenses(Node<Expense>* root, string d1, string d2) {
    if (root == nullptr) return;
    
    printRangeExpenses(root->left, d1, d2);
    
    // Check range
    if (compareDates(root->data.date, d1) >= 0 && compareDates(root->data.date, d2) <= 0) {
        cout << "ID: " << root->data.expenseID 
             << " | Cat: " << root->data.category 
             << " | Amt: " << root->data.amount 
             << " | Date: " << root->data.date << endl;
    }
    
    printRangeExpenses(root->right, d1, d2);
}

void getExpensesInPeriod() {
    string d1, d2;
    cout << "\nEnter Start Date (DD-MM-YYYY): ";
    cin >> d1;
    cout << "Enter End Date (DD-MM-YYYY): ";
    cin >> d2;
    
    cout << "\n--- Expenses from " << d1 << " to " << d2 << " ---" << endl;
    printRangeExpenses(rootExpenses, d1, d2);
}

// 8. Highest Expense Day 
struct DateSum {
    string date;
    double amount;
};

void collectDailyExpenses(Node<Expense>* root, vector<DateSum>& daily, int familyID) {
    if (root == nullptr) return;
    collectDailyExpenses(root->left, daily, familyID);
    
    if (isUserInFamily(root->data.userID, familyID)) {
        bool found = false;
        for (auto& d : daily) {
            if (d.date == root->data.date) {
                d.amount += root->data.amount;
                found = true;
                break;
            }
        }
        if (!found) daily.push_back({root->data.date, root->data.amount});
    }
    
    collectDailyExpenses(root->right, daily, familyID);
}

void getHighestExpenseDay() {
    int famID;
    cout << "\nEnter Family ID: ";
    cin >> famID;
    
    vector<DateSum> daily;
    collectDailyExpenses(rootExpenses, daily, famID);
    
    if (daily.empty()) {
        cout << "No expenses found for this family." << endl;
        return;
    }
    
    // Find max
    string maxDate = "";
    double maxAmt = -1;
    
    for (const auto& d : daily) {
        if (d.amount > maxAmt) {
            maxAmt = d.amount;
            maxDate = d.date;
        }
    }
    
    cout << "\nHighest Expense Day: " << maxDate << " (Total: " << maxAmt << ")" << endl;
}

// 9. MAIN MENU

void showMenu() {
    cout << "\n=========================================" << endl;
    cout << "   FAMILY EXPENSE TRACKING SYSTEM (AVL)   " << endl;
    cout << "=========================================" << endl;
    cout << "1. Add User" << endl;
    cout << "2. Create Family" << endl;
    cout << "3. Add Expense" << endl;
    cout << "4. Get Total Family Expense & Budget Status" << endl;
    cout << "5. Get Categorical Expense (Sorted)" << endl;
    cout << "6. Get Individual Expense (Sorted)" << endl;
    cout << "7. Get Expenses in Date Range" << endl;
    cout << "8. Get Highest Expense Day" << endl;
    cout << "0. Exit" << endl;
    cout << "=========================================" << endl;
    cout << "Enter Choice: ";
}

//MAIN FUNCTION
int main() {
    // 1. Load Initial Data
    loadUsers();
    loadFamilies();
    loadExpenses();
    
    int choice;
    while (true) {
        showMenu();
        if (!(cin >> choice)) {
            // Handle non-integer input
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        
        switch (choice) {
            case 1: addUser(); break;
            case 2: createFamily(); break;
            case 3: addExpense(); break;
            case 4: getTotalExpense(); break;
            case 5: getCategoricalExpense(); break;
            case 6: getIndividualExpense(); break;
            case 7: getExpensesInPeriod(); break;
            case 8: getHighestExpenseDay(); break;
            case 0: 
                cout << "Exiting... Data saved." << endl;
                return 0;
            default: cout << "Invalid choice!" << endl;
        }
    }
    return 0;
}