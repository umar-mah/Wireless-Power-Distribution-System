// UMBC - CMSC 341 - Spring 2024 - Proj2
#include "wpower.h"

// Constructor
WirelessPower::WirelessPower(TREETYPE type)
{
    m_type = type;
    m_root = nullptr;
}

// Destructor
WirelessPower::~WirelessPower()
{
    clear();
}

// Main Clear Function
void WirelessPower::clear()
{
    clearHelper(m_root);
    m_root = nullptr; // root to nullptr.
}
// Clear Helper Function (Recursive)
void WirelessPower::clearHelper(Customer *node)
{
    if (node != nullptr)
    {
        clearHelper(node->getLeft());  // Recursively clear the left subtree
        clearHelper(node->getRight()); // Recursively clear the right subtree
        delete node; 
        node = nullptr;                  // Delete the current node
    }
}
// Main Insert Function
void WirelessPower::insert(const Customer &customer)
{
    m_root = insertHelper(m_root, customer);
}

// Actual recursive function to handle insertion
Customer *WirelessPower::insertHelper(Customer *node, const Customer &customer)
{
    if (m_type == 0 || m_type == 1)
    {
        if (node == nullptr)
        {
            node = new Customer(customer);
            node->setHeight(0);
            return node;
        }

        if (customer.m_id < node->m_id)
        {
            node->m_left = insertHelper(node->m_left, customer);
        }
        else if (customer.m_id > node->m_id)
        {
            node->m_right = insertHelper(node->m_right, customer);
        }
        else
        {
            return node;
        }
        node->setHeight(height(node));

        if (m_type == 1)
        {
            int bf = getBalanceFactor(node);

            // Left Left Case
            if (bf > 1 && customer.getID() < node->getLeft()->getID())
            {
                return rightRotate(node);
            }

            // Right Right Case
            if (bf < -1 && customer.getID() > node->getRight()->getID())
            {
                return leftRotate(node); // Corrected to leftRotate
            }

            // Left Right Case
            if (bf > 1 && customer.getID() > node->getLeft()->getID())
            {
                node->setLeft(leftRotate(node->getLeft()));
                return rightRotate(node);
            }

            // Right Left Case
            if (bf < -1 && customer.getID() < node->getRight()->getID())
            {
                node->setRight(rightRotate(node->getRight()));
                return leftRotate(node);
            }
        }
    }
    else
    {
        if (node == nullptr)
        {
            node = new Customer(customer);
            return node;
        }

        // Bring the closest leaf node to node
        node = splaying(node, customer.getID());

        // If key is already present, then return
        if (node->getID() == customer.getID())
            return node;

        // allocate memory for new node
        Customer *newnode = new Customer(customer);

        // If node's key is greater, node as right child
        // of newnode and left child of node to newnode
        if (node->getID() > customer.getID())
        {
            newnode->setRight(node);
            newnode->setLeft(node->getLeft());
            node->setLeft(nullptr);
        }
        // If node's key is smaller, node as left child
        // of newnode and right child of node to newnode
        else
        {
            newnode->setLeft(node);
            newnode->setRight(node->getRight());
            node->setRight(nullptr);
        }
        // newnode->getRight()->setHeight(height(newnode->getRight()));
        // newnode->getLeft()->setHeight(height(newnode->getLeft()));
        return newnode; // newnode becomes new root
    }
    return node;
}

// Main Remove Function
void WirelessPower::remove(int id)
{
    if (m_type == 0 || m_type == 1) // Only AVL and BST
    {
        m_root = removeHelper(m_root, id);
    }
}

// Actual recursive function to handle remove
Customer *WirelessPower::removeHelper(Customer *node, int id)
{
    if (node == nullptr)
    {
        return node;
    }

    if (id < node->getID())
    {
        node->setLeft(removeHelper(node->getLeft(), id));
    }
    else if (id > node->getID())
    {
        node->setRight(removeHelper(node->getRight(), id));
    }
    else
    {
        // Node with only one child or no child
        if (node->getLeft() == nullptr || node->getRight() == nullptr)
        {
            Customer *temp = node->getLeft() ? node->getLeft() : node->getRight();

            if (temp == nullptr)
            {
                temp = node;
                node = nullptr;
            }
            else
            {
                *node = *temp;
            }

            delete temp;
            temp = nullptr;
        }
        else
        {
            // Node with two children
            Customer *temp = minValueNodeLeft(node->getRight());

            node->setID(temp->getID());

            node->setRight(removeHelper(node->getRight(), temp->getID()));
        }
    }

    // If the tree had only one node then return
    if (node == nullptr)
        return node;

    // Update height of the current node
    node->setHeight(max(height(node->getLeft()), height(node->getRight())) + 1);

    // Balance the node
    int balance = getBalanceFactor(node);

    // Left Left Case
    if (balance > 1 && getBalanceFactor(node->getLeft()) >= 0)
        return rightRotate(node);

    // Left Right Case
    if (balance > 1 && getBalanceFactor(node->getLeft()) < 0)
    {
        node->setLeft(leftRotate(node->getLeft()));
        return rightRotate(node);
    }

    // Right Right Case
    if (balance < -1 && getBalanceFactor(node->getRight()) <= 0)
        return leftRotate(node);

    // Right Left Case
    if (balance < -1 && getBalanceFactor(node->getRight()) > 0)
    {
        node->setRight(rightRotate(node->getRight()));
        return leftRotate(node);
    }

    return node;
}
TREETYPE WirelessPower::getType() const
{
    return m_type;
}

// Type Change Function
void WirelessPower::setType(TREETYPE type)
{
    if (type == 1)
    {
        m_type = type;                 // Set the tree type to AVL
        m_root = convertToAVL(m_root); // Convert the existing tree to AVL
    }
    m_type = type;
}

// Helper function to convert BST to AVL (Balances Tree)
Customer *WirelessPower::rebalance(Customer *node)
{
    bool heightChanged;
    do
    {
        heightChanged = false;

        // Get the balance factor
        int balance = getBalanceFactor(node);

        // Left Left Case
        if (balance > 1 && getBalanceFactor(node->getLeft()) >= 0)
        {
            node = rightRotate(node);
            heightChanged = true;
        }
        // Left Right Case
        else if (balance > 1 && getBalanceFactor(node->getLeft()) < 0)
        {
            node->setLeft(leftRotate(node->getLeft()));
            node = rightRotate(node);
            heightChanged = true;
        }
        // Right Right Case
        else if (balance < -1 && getBalanceFactor(node->getRight()) <= 0)
        {
            node = leftRotate(node);
            heightChanged = true;
        }
        // Right Left Case
        else if (balance < -1 && getBalanceFactor(node->getRight()) > 0)
        {
            node->setRight(rightRotate(node->getRight()));
            node = leftRotate(node);
            heightChanged = true;
        }

        // Update the height of the current node if a rotation occurred
        if (heightChanged)
        {
            node->setHeight(1 + max(height(node->getLeft()), height(node->getRight())));
        }
        if (getBalanceFactor(node) > 1 || getBalanceFactor(node) < -1)
        {
            heightChanged = true;
        }
    } while (heightChanged); // Repeat if the height was changed
    return node;
}

// Helper main function to convert BST to AVL
Customer *WirelessPower::convertToAVL(Customer *node)
{
    if (node == nullptr)
    {
        return nullptr;
    }
    // Convert left and right subtrees
    node->setLeft(convertToAVL(node->getLeft()));
    node->setRight(convertToAVL(node->getRight()));

    // Rebalance the node and update the balance factor
    return rebalance(node);
}

void WirelessPower::dumpTree() const { dump(m_root); }
void WirelessPower::dump(Customer *customer) const
{
    if (customer != nullptr)
    {
        cout << "(";
        dump(customer->m_left);                              // first visit the left child
        cout << customer->m_id << ":" << customer->m_height; // second visit the node itself
        dump(customer->m_right);                             // third visit the right child
        cout << ")";
    }
}
ostream &operator<<(ostream &sout, const Customer &x)
{
    sout << x.m_id << " (" << x.getLatStr() << ", " << x.getLongStr() << ")" << endl;
    ;
    return sout;
}

// Finds Height of node
int WirelessPower::height(Customer *customer)
{
    if (customer == nullptr)
    {
        return -1;
    }
    else
    {
        int lheight = height(customer->m_left);
        int rheight = height(customer->m_right);

        return (lheight > rheight ? lheight : rheight) + 1;
    }
}

// Find lowest value on right subtree
Customer *WirelessPower::minValueNodeRight(Customer *node)
{
    Customer *current = node;
    while (current->getRight() != NULL)
    {
        current = current->getRight();
    }
    return current;
}

// Finds lowest values on left subtree
Customer *WirelessPower::minValueNodeLeft(Customer *node)
{
    Customer *current = node;
    while (current->getLeft() != NULL)
    {
        current = current->getLeft();
    }
    return current;
}

// Calculates balance factor
int WirelessPower::getBalanceFactor(Customer *node)
{
    if (node == nullptr)
    {
        return -1;
    }
    else
    {
        return (height(node->getLeft()) - height(node->getRight()));
    }
}

// Right rotate function
Customer *WirelessPower::rightRotate(Customer *y)
{
    Customer *x = y->getLeft();
    Customer *T2 = x->getRight();

    // Perform rotation
    x->setRight(y);
    y->setLeft(T2);

    // Update heights
    y->setHeight(max(height(y->getLeft()), height(y->getRight())) + 1); // Max height
    x->setHeight(max(height(x->getLeft()), height(x->getRight())) + 1);

    return x;
}

// Left rotate function
Customer *WirelessPower::leftRotate(Customer *x)
{
    Customer *y = x->getRight();
    Customer *T2 = y->getLeft();

    // Perform rotation
    y->setLeft(x);
    x->setRight(T2);
    // Update heights
    x->setHeight(max(height(x->getLeft()), height(x->getRight())) + 1);
    y->setHeight(max(height(y->getLeft()), height(y->getRight())) + 1);

    return y;
}

// Function to perform post-order traversal and copy the nodes
Customer *WirelessPower::postOrderAssignment(Customer *node, Customer *rhs)
{
    if (rhs == nullptr)
    {
        return nullptr;
    }

    // Ensuring 'node' is allocated before updating its values
    if (node == nullptr)
    {
        node = new Customer(rhs->getID(), rhs->getLatitude(), rhs->getLongitude());
    }
    else
    {
        node->setID(rhs->getID());
        node->setLatitude(rhs->getLatitude());
        node->setLongitude(rhs->getLongitude());
    }
    node->setHeight(rhs->getHeight());

    node->setLeft(postOrderAssignment(node->getLeft(), rhs->getLeft()));
    node->setRight(postOrderAssignment(node->getRight(), rhs->getRight()));

    return node;
}

// Overloaded assignment operator
const WirelessPower &WirelessPower::operator=(const WirelessPower &rhs)
{
    if (this != &rhs)
    {
        if (m_root == nullptr)
        {
            cout << "Error:: Assigning Empty Object to Empty Object"
                 << "\n";
        }
        clear(); // Clear the current tree
        m_type = rhs.getType();
        // Copy the tree starting from the root
        m_root = postOrderAssignment(nullptr, rhs.m_root);
    }
    else
    {
        cout << "Error:: Self Assignment"
             << "\n";
    }
    return *this;
}

Customer *WirelessPower::splaying(Customer *root, int key)
{
    if (root == nullptr || root->getID() == key)
        return root;

    // Key lies in left subtree
    if (root->getID() > key)
    {
        if (root->getLeft() == NULL)
        {
            if (root->getRight() != nullptr)
            {
                root->getRight()->setHeight(max(height(root->getRight()->getLeft()), height(root->getRight()->getRight())) + 1);
            }
            if (root->getLeft() != nullptr)
            {
                root->getLeft()->setHeight(max(height(root->getLeft()->getLeft()), height(root->getLeft()->getRight())) + 1);
            }
            return root;
        }
        // Zig-Zig (Left Left)
        if (root->getLeft()->getID() > key)
        {
            root->getLeft()->setLeft(splaying(root->getLeft()->getLeft(), key));
            root = rightRotate(root);
        }
        // Zig-Zag (Left Right)
        else if (root->getLeft()->getID() < key)
        {
            root->getLeft()->setRight(splaying(root->getLeft()->getRight(), key));
            if (root->getLeft()->getRight() != nullptr)
                root->setLeft(rightRotate(root->getLeft()));
        }
        if (root->getRight() != nullptr)
        {
            root->getRight()->setHeight(max(height(root->getRight()->getLeft()), height(root->getRight()->getRight())) + 1);
        }
        if (root->getLeft() != nullptr)
        {
            root->getLeft()->setHeight(max(height(root->getLeft()->getLeft()), height(root->getLeft()->getRight())) + 1);
        }
        return (root->getLeft() == nullptr) ? root : rightRotate(root);
    }
    // Key lies in right subtree
    else
    {
        if (root->getRight() == nullptr)
        {
            if (root->getLeft() != nullptr)
            {
                root->getLeft()->setHeight(max(height(root->getLeft()->getLeft()), height(root->getLeft()->getRight())) + 1);
            }
            return root;
        }
        // Zag-Zig (Right Left)
        if (root->getRight()->getID() > key)
        {
            root->getRight()->setLeft(splaying(root->getRight()->getLeft(), key));
            if (root->getRight()->getLeft() != nullptr)
            {
                root->setRight(rightRotate(root->getRight()));
            }
            if (root->getRight() != nullptr)
            {
                root->getRight()->setHeight(max(height(root->getRight()->getLeft()), height(root->getRight()->getRight())) + 1);
            }
            if (root->getLeft() != nullptr)
            {
                root->getLeft()->setHeight(max(height(root->getLeft()->getLeft()), height(root->getLeft()->getRight())) + 1);
            }
        }
        // Zag-Zag (Right Right)
        else if (root->getRight()->getID() < key)
        {
            root->getRight()->setRight(splaying(root->getRight()->getRight(), key));
            root = leftRotate(root);
        }
        if (root->getRight() != nullptr)
        {
            root->getRight()->setHeight(max(height(root->getRight()->getLeft()), height(root->getRight()->getRight())) + 1);
        }
        if (root->getLeft() != nullptr)
        {
            root->getLeft()->setHeight(max(height(root->getLeft()->getLeft()), height(root->getLeft()->getRight())) + 1);
        }
        return (root->getRight() == nullptr) ? root : leftRotate(root);
    }
}
void WirelessPower::updateHeight(Customer *node)
{
    if (node != nullptr)
    {
        int leftHeight = height(node->getLeft());
        int rightHeight = height(node->getRight());
        node->setHeight(1 + max(leftHeight, rightHeight));
    }
}
int WirelessPower::max(int a, int b)
{
    return (a > b) ? a : b;
}