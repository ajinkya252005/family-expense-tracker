# Family Expense Tracking System (DSA Project)

A high-performance, terminal-based application designed to track and analyze family finances efficiently. Built using **C++** and **AVL Trees (Self-Balancing Binary Search Trees)**, this system ensures optimal $O(\log n)$ time complexity for data retrieval and storage, even as the dataset grows.

## 🚀 Key Features

* **Hierarchical Data Management:**
    * **Users:** Stores individual details (Name, Income) with unique IDs.
    * **Families:** Dynamically aggregates multiple users, calculating total family income and tracking collective monthly expenses automatically.
    * **Expenses:** Links spending to specific users and categories (Rent, Grocery, Utility, etc.).

* **Advanced Data Structures (AVL Trees):**
    * Implements **three separate AVL Trees** to manage Users, Families, and Expenses.
    * Guarantees balanced height for efficient searching, insertion, and deletion.
    * Custom rotation logic (Left/Right) ensures the tree remains balanced after every operation.

* **Financial Analytics:**
    * **Budget Tracking:** Automatically compares total family expenses against income to flag deficits or savings.
    * **Categorical Reports:** Generates sorted breakdowns of spending by category (e.g., "How much did we spend on Rent?").
    * **Individual Contributions:** Detailed reports on how much each family member contributed to specific expenses.
    * **Date-Range Queries:** Retrieve all transactions within a specific start and end date.

* **Persistent Storage:**
    * Uses a custom File I/O engine to save and load data from CSV files (`users.csv`, `families.csv`, `expenses.csv`).
    * Ensures data persistence across program restarts.

## 🛠️ Tech Stack

* **Language:** C++ (Standard Template Library)
* **Data Structures:** AVL Trees, Linked Nodes
* **Storage:** CSV File Handling (fstream)
* **Concepts:** Object-Oriented Programming (OOP), Recursion, Pointers

## 📂 Project Structure

```text
├── main.cpp          # Source code containing AVL implementation and Menu logic
├── users.csv         # Database file for User records
├── families.csv      # Database file for Family aggregations
├── expenses.csv      # Database file for Expense transaction logs
└── README.md         # Project Documentation

## ⚙️ How to Run (Cloning & Setup)

1.  **Clone the Repository:**
    ```bash
    git clone [https://github.com/ajinkya252005/family-expense-tracker.git](https://github.com/ajinkya252005/family-expense-tracker.git)
    cd family-expense-tracker
    ```

2.  **Compile the Code:**
    You need a C++ compiler (like g++).
    ```bash
    g++ main.cpp -o tracker
    ```

3.  **Run the Application:**
    * **Windows:**
        ```bash
        tracker.exe
        ```
    * **Mac/Linux:**
        ```bash
        ./tracker
        ```

## 📊 Menu Options

Once the application is running, you can perform the following operations via the terminal menu:

1.  **Add User:** Register a new individual with income details.
2.  **Create Family:** Group individuals into a family unit.
3.  **Add Expense:** Log a new transaction for a user.
4.  **Get Family Status:** View total income vs. total expense (Budget Check).
5.  **Get Categorical Expense:** See spending sorted by category.
6.  **Get Individual Expense:** View spending habits of a specific user.
7.  **Date Range Report:** List expenses between two dates.
8.  **Highest Expense Day:** Find the date with the peak spending.
9.  **Exit:** Save data to CSVs and close the program.

## 🧠 Complexity Analysis

| Operation | Data Structure | Time Complexity | Description |
| :--- | :--- | :--- | :--- |
| **Search (User/Family)** | AVL Tree | $O(\log n)$ | Binary Search Logic on balanced tree. |
| **Insertion** | AVL Tree | $O(\log n)$ | Includes re-balancing (rotations). |
| **Range Queries** | AVL Tree | $O(k + \log n)$ | Where $k$ is the number of elements in range. |
| **Data Loading** | File I/O | $O(n)$ | Linear scan of CSV files on startup. |

## 👤 Author & Contact

**Ajinkya Ashish Moharir**
* **Course:** TE AI & DS, PVG's COET, Pune
* **Email:** ajinkyamoharir2005@gmail.com
* **Project Type:** DSA / Low-Level Design