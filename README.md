# Wireless Power Distribution System — CMSC 341 Project 2

**Course:** CMSC 341 — Data Structures  
**Semester:** Spring 2024  
**Instructor:** University of Maryland, Baltimore County (UMBC)  
**Author:** Muhammad Umar  

---

## ⚡ Overview

The **Wireless Power Distribution System (WirelessPower)** simulates a customer-based wireless power management system using **binary search tree (BST)**, **AVL**, and **splay tree** data structures.  
Each **Customer** node represents a client receiving wireless power, organized by their unique ID, and stored in a tree structure depending on the selected mode.

This project demonstrates efficient data manipulation and retrieval in hierarchical tree structures while maintaining optimal performance through balancing techniques.

---

## 🧩 Project Objectives

- Implement a dynamic tree-based data system for managing wireless power customers.  
- Explore three major tree types:
  - **BST (Binary Search Tree)** – Basic unbalanced tree.
  - **AVL Tree** – Self-balancing binary search tree.
  - **Splay Tree** – Self-adjusting binary search tree with recently accessed elements moved closer to the root.
- Demonstrate tree operations: **Insertion**, **Deletion**, **Balancing**, and **Traversal**.
- Utilize recursive algorithms to manage hierarchical data efficiently.

---

## 🏗️ Class Overview

### **WirelessPower Class**

| **Member Function** | **Description** |
|----------------------|-----------------|
| `WirelessPower(TREETYPE type)` | Constructor that initializes the tree with a specified type (BST, AVL, or Splay). |
| `~WirelessPower()` | Destructor that clears the tree. |
| `clear()` | Clears the entire tree and resets root to `nullptr`. |
| `insert(const Customer &customer)` | Inserts a new customer node according to the tree type. |
| `remove(int id)` | Removes a customer node by ID (BST/AVL only). |
| `setType(TREETYPE type)` | Changes the tree type (BST → AVL) and rebalances the structure. |
| `dumpTree()` | Displays the tree structure in parenthetical form. |
| `operator=` | Overloaded assignment operator to copy tree contents. |

---

## 🌳 Tree Implementations

### 1. **Binary Search Tree (BST)**
- Simple hierarchical structure sorted by `Customer ID`.  
- No self-balancing — can degrade to linear time in the worst case.

### 2. **AVL Tree**
- Ensures logarithmic time complexity by rebalancing after insertions and deletions.  
- Balancing handled using **rotation operations**:
  - **Right Rotation (LL Case)**
  - **Left Rotation (RR Case)**
  - **Left-Right Rotation (LR Case)**
  - **Right-Left Rotation (RL Case)**

### 3. **Splay Tree**
- Self-adjusting tree that moves frequently accessed nodes closer to the root.  
- Implements:
  - **Zig-Zig** (Double rotation)
  - **Zig-Zag** (Single rotation)
- Provides **amortized O(log n)** performance for repeated access.

---

## ⚙️ Core Functions

### **Insertion**
- Recursively inserts a new `Customer` node into the tree.
- For AVL trees, applies rotations to maintain height balance.
- For Splay trees, the node is "splayed" to the root after insertion.

### **Deletion**
- Removes a `Customer` node by ID (only for BST and AVL).
- Handles all three cases:
  1. Node with no child  
  2. Node with one child  
  3. Node with two children (replaces with smallest node in the right subtree)

### **Balancing**
- Uses height differences to determine rotation cases in AVL.
- Rebalances nodes until all subtrees maintain acceptable height difference (≤ 1).

### **Splaying**
- Moves accessed or inserted nodes toward the root using rotations:
  - **Zig-Zig (Left Left / Right Right)**
  - **Zig-Zag (Left Right / Right Left)**

### **Rotations**
- `leftRotate()` and `rightRotate()` maintain tree balance.
- Each rotation updates node heights to preserve AVL constraints.

---

## 🧮 Helper Functions

| **Function** | **Description** |
|---------------|-----------------|
| `height(Customer *node)` | Returns the height of a given node. |
| `getBalanceFactor(Customer *node)` | Computes the node's balance factor (Left height - Right height). |
| `minValueNodeLeft()` / `minValueNodeRight()` | Finds the smallest value node in a subtree. |
| `rebalance()` | Rebalances an existing subtree to convert BST → AVL. |
| `convertToAVL()` | Recursively converts the entire tree to an AVL structure. |
| `clearHelper()` | Recursively deletes all nodes in the tree. |
| `postOrderAssignment()` | Copies a tree in post-order for deep assignment operations. |

---

## 🧰 Operator Overloads

### `operator=`
- Performs **deep copy** between two `WirelessPower` objects.
- Uses `postOrderAssignment()` to duplicate all nodes.
- Handles self-assignment and empty object assignment errors.

### `operator<<`
- Prints `Customer` details in the format:  
  ```
  ID (Latitude, Longitude)
  ```

---

## 🧪 Testing and Validation

### Test Operations
- Insert multiple `Customer` objects and visualize tree growth.
- Validate AVL balancing using height differences.
- Test splay operation by repeatedly accessing specific nodes.
- Perform deletions and confirm that the tree rebalances correctly.

### Example Output
Parenthetical `dumpTree()` output:
```
((1:0)(2:1)(3:0))
```
indicates each node’s ID and height.

---

## 🔍 Complexity Analysis

| **Operation** | **BST** | **AVL** | **Splay** |
|----------------|----------|----------|------------|
| Insertion | O(log n) avg / O(n) worst | O(log n) | O(log n) amortized |
| Deletion | O(log n) avg / O(n) worst | O(log n) | O(log n) amortized |
| Search | O(log n) avg / O(n) worst | O(log n) | O(log n) amortized |

---

## 📚 Lessons Learned

- Understanding **tree balancing** through rotations and height calculations.  
- Implementing **recursive memory management** safely in C++.  
- Designing polymorphic behavior for multiple tree types in one class.  
- Developing deep-copy operations with post-order traversal.

---

## 📦 File Structure

```
├── wpower.h          # Header file containing class definitions
├── wpower.cpp        # Implementation file (this project)
├── customer.h        # Customer node class
├── Makefile          # Build configuration
└── README.md         # Project documentation
```

---

## 🧠 Summary

This project demonstrates a flexible and efficient implementation of **three major search tree algorithms** under a unified class interface.  
It highlights the importance of **data structure design**, **tree balancing**, and **algorithmic efficiency** — key principles in **Data Structures (CMSC 341)**.

---
