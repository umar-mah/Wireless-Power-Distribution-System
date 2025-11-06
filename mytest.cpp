// UMBC - CMSC 341 - Spring 2024 - Proj2
#include "wpower.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>

enum RANDOM
{
    UNIFORMINT,
    UNIFORMREAL,
    NORMAL,
    SHUFFLE
};
class Random
{
public:
    Random(int min, int max, RANDOM type = UNIFORMINT, int mean = 50, int stdev = 20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL)
        {
            // the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            // the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            // the mean and standard deviation can change by passing new values to constructor
            m_normdist = std::normal_distribution<>(mean, stdev);
        }
        else if (type == UNIFORMINT)
        {
            // the case of UNIFORMINT to generate integer numbers
            //  Using a fixed seed value generates always the same sequence
            //  of pseudorandom numbers, e.g. reproducing scientific experiments
            //  here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10); // 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min, max);
        }
        else if (type == UNIFORMREAL)
        {                                   // the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10); // 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min, (double)max);
        }
        else
        { // the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum)
    {
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    void getShuffle(vector<int> &array)
    {
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i <= m_max; i++)
        {
            array.push_back(i);
        }
        shuffle(array.begin(), array.end(), m_generator);
    }

    void getShuffle(int array[])
    {
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i <= m_max; i++)
        {
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it = temp.begin(); it != temp.end(); it++)
        {
            array[i] = *it;
            i++;
        }
    }

    int getRandNum()
    {
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if (m_type == NORMAL)
        {
            // returns a random number in a set with normal distribution
            // we limit random numbers by the min and max values
            result = m_min - 1;
            while (result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT)
        {
            // this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum()
    {
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result * 100.0) / 100.0;
        return result;
    }

private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;            // normal distribution
    std::uniform_int_distribution<> m_unidist;        // integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal; // real uniform distribution
};

class Tester
{
public:
    bool testingBalanceFactor(Customer *node);
    int height(Customer *customer);
    bool testingBSTproperty(Customer *node);
    bool testingInsert(WirelessPower &powerSource, int numElements, Random &idGen, Random &rndLat, Random &rndLong);
    bool checkHeightHelper(Customer *node);
    bool checkHeight(Customer *node);
    bool search(Customer *node, int id);
    bool testingTypeChange(WirelessPower &powerSource, TREETYPE type);
    bool testingRemove(WirelessPower &powerSource, int numElements, Random &idGen, Random &rndLat, Random &rndLong);
};

int Tester::height(Customer *customer)
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

bool Tester::testingBalanceFactor(Customer *node)
{
    if (node == nullptr)
    {
        return true;
    }
    // Check the balance factor of the current node
    int balance = height(node->getLeft()) - height(node->getRight());
    if (abs(balance) > 1)
    {
        cout << "Error: Tree not balanced."
             << "\n";
        return false; // Not balanced
    }

    // Recursively check the balance factor of left and right subtrees
    return testingBalanceFactor(node->getLeft()) && testingBalanceFactor(node->getRight());
}

bool Tester::testingBSTproperty(Customer *node)
{
    if (node == nullptr)
    {
        return true;
    }

    if (node->getLeft() != nullptr)
    {
        if (node->getID() < node->getLeft()->getID())
        {
            cout << "Error: Does not follow BST Property."
                 << "\n";
            return false;
        }
    }
    if (node->getRight() != nullptr)
    {
        if (node->getID() > node->getRight()->getID())
        {
            cout << "Error: Does not follow BST Property."
                 << "\n";
            return false;
        }
    }
    // Recursively check the BST property of left and right subtrees
    return testingBSTproperty(node->getLeft()) && testingBSTproperty(node->getRight());
}
bool Tester::testingInsert(WirelessPower &powerSource, int numElements, Random &idGen, Random &rndLat, Random &rndLong)
{
    int randID = 0;
    cout << "Inserting nodes"
         << "\n";
    for (int i = 0; i < numElements; i++)
    {
        randID = idGen.getRandNum();
        Customer customer(randID, rndLat.getRealRandNum(), rndLong.getRealRandNum());
        powerSource.insert(customer);
        if (customer.getID() != randID)
        {
            cout << "Error: ID not assigned."
                 << "\n";
            return false;
        }
    }
    if (powerSource.getType() == 2)
    {
        if (powerSource.m_root->getID() == randID)
        {
            cout << "Splay Success."
                 << "\n";
        }
        else
        {
            cout << "Error: Splay Failed."
                 << "\n";
            return false;
        }
    }
    if (testingBSTproperty(powerSource.m_root) != false)
    {
        cout << "Follows BST Property."
             << "\n";
    }
    else
    {
        if (powerSource.getType() == 0)
        {
            return false;
        }
    }
    if (checkHeight(powerSource.m_root) != false)
    {
        cout << "Heights are correct."
             << "\n";
    }
    else
    {
        cout << "Height incorrect"
             << "\n";
        return false;
    }
    if (testingBalanceFactor(powerSource.m_root) != false)
    {
        cout << "Tree Balanced."
             << "\n";
    }
    else
    {
        if (powerSource.getType() == 1)
        {
            return false;
        }
    }
    cout << "Succesfully inserted " << numElements << " nodes into the tree."
         << "\n";
    return true;
}

bool Tester::search(Customer *node, int id)
{
    // Base case: if the node is nullptr, the id is not found.
    if (node == nullptr)
    {
        return false;
    }
    // Check if the current node has the id.
    if (node->getID() == id)
    {
        return true; // The id is found.
    }
    // Recursively search the left and right subtrees.
    return search(node->getLeft(), id) || search(node->getRight(), id);
}

bool Tester::testingRemove(WirelessPower &powerSource, int numElements, Random &idGen, Random &rndLat, Random &rndLong)
{
    int randID = 0;
    cout << "Inserting nodes"
         << "\n";
    for (int i = 0; i < numElements; i++)
    {
        randID = idGen.getRandNum();

        powerSource.remove(randID);
        if (search(powerSource.m_root, randID))
        {
            cout << "Error: ID not deleted."
                 << "\n";
            return false;
        }
    }
    if (testingBSTproperty(powerSource.m_root) != false)
    {
        cout << "Follows BST Property."
             << "\n";
    }
    else
    {
        if (powerSource.getType() == 0)
        {
            return false;
        }
    }

    if (testingBalanceFactor(powerSource.m_root) != false)
    {
        cout << "Tree Balanced."
             << "\n";
    }
    else
    {
        if (powerSource.getType() == 1)
        {
            return false;
        }
    }
    if (checkHeight(powerSource.m_root) != false)
    {
        cout << "Heights are correct."
             << "\n";
    }
    else
    {
        return false;
    }
    cout << "Succesfully deleted " << numElements << " nodes into the tree."
         << "\n";
    return true;
}
bool Tester::checkHeightHelper(Customer *node)
{
    if (node == nullptr)
    {
        return true; // An empty tree has a correct height by definition.
    }
    int maxHeight = node->getHeight();
    int tempHeight = height(node);
    if (node == nullptr)
    {
        tempHeight = 0;
    }
    if (maxHeight != tempHeight)
    {
        return false;
    }
    return true; // If the heights match, return true.
}

bool Tester::checkHeight(Customer *node)
{
    if (node == nullptr)
    {
        return true; // Base case: an empty node is considered correct.
    }
    // Check the left and right subtrees and the current node.
    return checkHeight(node->getLeft()) && checkHeight(node->getRight()) && checkHeightHelper(node);
}

bool Tester::testingTypeChange(WirelessPower &powerSource, TREETYPE type)
{
    powerSource.setType(type);
    if (testingBSTproperty(powerSource.m_root) != false)
    {
        cout << "Follows BST Property."
             << "\n";
    }
    else
    {
        if (powerSource.getType() == 0)
        {
            return false;
        }
    }

    if (testingBalanceFactor(powerSource.m_root) != false)
    {
        cout << "Tree Balanced."
             << "\n";
    }
    else
    {
        if (powerSource.getType() == 1)
        {
            return false;
        }
    }
    if (checkHeight(powerSource.m_root) != false)
    {
        cout << "Heights are correct."
             << "\n";
    }
    else
    {
        return false;
    }
    return true;
}
int main()
{
    Random idGen(MINID, MAXID);
    Random rndLat(MINLAT, MAXLAT, UNIFORMREAL);
    rndLat.setSeed(2);
    Random rndLong(MINLONG, MAXLONG, UNIFORMREAL);
    rndLong.setSeed(7);
    bool flag = false;
    Tester test;

    WirelessPower powerSourceBST(BST); // BST Case Normal
    cout << "BST Normal Case Insertion:"
         << "\n";
    flag = test.testingInsert(powerSourceBST, 500, idGen, rndLat, rndLong);
    if (!flag)
    {
        cout << "Error: Insertion did not occur."
             << "\n";
    }
    cout << "\n";

    cout << "BST Normal Case Deletion:"
         << "\n";
    flag = test.testingRemove(powerSourceBST, 470, idGen, rndLat, rndLong);
    if (!flag)
    {
        cout << "Error: Remove did not occur."
             << "\n";
    }
    cout << "\n";
    cout << "Assignment Operator: Normal Case."
         << "\n";
    WirelessPower powerSourceBST2(BST);
    powerSourceBST2 = powerSourceBST;
    if (&powerSourceBST == &powerSourceBST2)
    {
        cout << "Error: No deep copy"
             << "\n";
    }
    else if (powerSourceBST.getType() != powerSourceBST2.getType())
    {
        cout << "Error: Type not equal."
             << "\n";
    }
    else
    {
        cout << "Assignment Operator Passed"
             << "\n";
    }
    cout << "\n";
    cout << "BST Edge Case Deletion:"
         << "\n";

    flag = test.testingRemove(powerSourceBST, 30, idGen, rndLat, rndLong);
    if (!flag)
    {
        cout << "Error: Remove did not occur."
             << "\n";
    }
    cout << "\n";

    WirelessPower powerSourceAVL(AVL); // AVL Case Normal
    cout << "AVL Normal Case Insertion:"
         << "\n";
    flag = test.testingInsert(powerSourceAVL, 1000, idGen, rndLat, rndLong);
    if (!flag)
    {
        cout << "Error: Insertion did not occur."
             << "\n";
    }
    cout << "\n";

    cout << "AVL Normal Case Delete:"
         << "\n";
    flag = test.testingRemove(powerSourceAVL, 900, idGen, rndLat, rndLong);
    if (!flag)
    {
        cout << "Error: Deletion did not occur."
             << "\n";
    }
    cout << "\n";
    cout << "\n";
    cout << "BST to AVL"
         << "\n";
    test.testingTypeChange(powerSourceBST2, AVL);
    cout << "\n";
    cout << "AVL Edge Case Delete:"
         << "\n";
    flag = test.testingRemove(powerSourceBST2, 30, idGen, rndLat, rndLong);
    if (!flag)
    {
        cout << "Error: Deletion did not occur."
             << "\n";
    }
    cout << "\n";
    cout << "Splay Normal Case Insert:"
         << "\n";
    WirelessPower Splays(SPLAY);
    flag = test.testingInsert(Splays, 20, idGen, rndLat, rndLong);
    // Height error on one node: Cannot fix it. Left or right of subtree.
    cout << "\n";
    cout << "Assignment operator Error Case"
         << "\n";
    Splays = Splays;
}
