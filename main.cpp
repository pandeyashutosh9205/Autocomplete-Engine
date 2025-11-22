#include <bits/stdc++.h>
#include <conio.h>
#include <chrono>

using namespace std;

vector<string> dataset;
int currentengine = 1;

void loadData()
{
    cout << "Loading data from file data.txt" << endl;

    ifstream file("data.txt");
    string line;

    if (!file.is_open())
    {
        cout << "Could not open file data.txt" << endl;
        return;
    }

    while (getline(file, line))
    {

        dataset.push_back(line);
    }
    file.close();

    cout << "Successfully loaded " << dataset.size() << " entries." << endl;
}

class BSTNode
{
public:
    string data;
    BSTNode *left;
    BSTNode *right;

    BSTNode(string value)
    {
        data = value;
        left = nullptr;
        right = nullptr;
    }
};

class AVLNode
{
public:
    string data;
    AVLNode *left;
    AVLNode *right;
    int height;

    AVLNode(string value)
    {
        data = value;
        left = right = nullptr;
        height = 1;
    }
};

enum Color
{
    RED,
    BLACK
};

class RBNode
{
public:
    string data;
    bool color;
    RBNode *left, *right, *parent;

    RBNode(string value)
    {
        data = value;
        left = right = parent = nullptr;
        color = RED;
    }
};

class HistoryNode
{
public:
    string data;
    HistoryNode *next;

    HistoryNode(string value)
    {
        data = value;
        next = nullptr;
    }
};

HistoryNode *historyHead = nullptr;

void addToHistory(string searchQuery)
{
    HistoryNode *newNode = new HistoryNode(searchQuery);

    newNode->next = historyHead;

    historyHead = newNode;

    cout << " Saved to history." << endl;
}

void displayHistory()
{
    system("cls");
    cout << "==========================================" << endl;
    cout << "           RECENT SEARCH HISTORY          " << endl;
    cout << "==========================================" << endl;

    if (historyHead == nullptr)
    {
        cout << " History is empty." << endl;
    }
    else
    {
        HistoryNode *temp = historyHead;
        int count = 1;
        while (temp != nullptr)
        {
            cout << "  " << count++ << ". " << temp->data << endl;
            temp = temp->next;
        }
    }

    cout << " Press any key to return to the menu." << endl;
    _getch();
}

void loadHistory()
{
    ifstream file("log.txt");
    if (!file.is_open())
    {
        cout << " No previous history file found." << endl;
        return;
    }

    string line;

    stack<string> historyStack;
    while (getline(file, line))
    {
        if (!line.empty())
        {
            historyStack.push(line);
        }
    }

    file.close();

    while (!historyStack.empty())
    {
        addToHistory(historyStack.top());
        historyStack.pop();
    }
    cout << " History loaded." << endl;
}

void saveHistory()
{
    ofstream file("log.txt");
    if (!file.is_open())
    {
        cout << "Error: Could not save history to log.txt" << endl;
        return;
    }

    HistoryNode *temp = historyHead;

    stack<string> historyStack;
    while (temp != nullptr)
    {
        historyStack.push(temp->data);
        temp = temp->next;
    }

    while (!historyStack.empty())
    {
        file << historyStack.top() << endl;
        historyStack.pop();
    }
    file.close();
}

class DirectoryRB
{
private:
    RBNode *root;

    void rotateLeft(RBNode *&root, RBNode *&pt)
    {
        RBNode *pt_right = pt->right;
        pt->right = pt_right->left;
        if (pt->right != nullptr)
            pt->right->parent = pt;
        pt_right->parent = pt->parent;
        if (pt->parent == nullptr)
            root = pt_right;
        else if (pt == pt->parent->left)
            pt->parent->left = pt_right;
        else
            pt->parent->right = pt_right;
        pt_right->left = pt;
        pt->parent = pt_right;
    }

    void rotateRight(RBNode *&root, RBNode *&pt)
    {
        RBNode *pt_left = pt->left;
        pt->left = pt_left->right;
        if (pt->left != nullptr)
            pt->left->parent = pt;
        pt_left->parent = pt->parent;
        if (pt->parent == nullptr)
            root = pt_left;
        else if (pt == pt->parent->left)
            pt->parent->left = pt_left;
        else
            pt->parent->right = pt_left;
        pt_left->right = pt;
        pt->parent = pt_left;
    }

    void fixViolation(RBNode *&root, RBNode *&pt)
    {
        RBNode *parent_pt = nullptr;
        RBNode *grand_parent_pt = nullptr;

        while ((pt != root) && (pt->color != BLACK) && (pt->parent != nullptr) && (pt->parent->color == RED))
        {
            parent_pt = pt->parent;
            grand_parent_pt = pt->parent->parent;

            if (parent_pt == grand_parent_pt->left)
            {
                RBNode *uncle_pt = grand_parent_pt->right;
                if (uncle_pt != nullptr && uncle_pt->color == RED)
                {
                    grand_parent_pt->color = RED;
                    parent_pt->color = BLACK;
                    uncle_pt->color = BLACK;
                    pt = grand_parent_pt;
                }
                else
                {
                    if (pt == parent_pt->right)
                    {
                        rotateLeft(root, parent_pt);
                        pt = parent_pt;
                        parent_pt = pt->parent;
                    }
                    rotateRight(root, grand_parent_pt);
                    swap(parent_pt->color, grand_parent_pt->color);
                    pt = parent_pt;
                }
            }
            else
            {
                RBNode *uncle_pt = grand_parent_pt->left;
                if ((uncle_pt != nullptr) && (uncle_pt->color == RED))
                {
                    grand_parent_pt->color = RED;
                    parent_pt->color = BLACK;
                    uncle_pt->color = BLACK;
                    pt = grand_parent_pt;
                }
                else
                {
                    if (pt == parent_pt->left)
                    {
                        rotateRight(root, parent_pt);
                        pt = parent_pt;
                        parent_pt = pt->parent;
                    }
                    rotateLeft(root, grand_parent_pt);
                    swap(parent_pt->color, grand_parent_pt->color);
                    pt = parent_pt;
                }
            }
        }
        root->color = BLACK;
    }

    RBNode *BSTInsert(RBNode *root, RBNode *node)
    {
        if (root == nullptr)
            return node;
        if (node->data < root->data)
        {
            root->left = BSTInsert(root->left, node);
            root->left->parent = root;
        }
        else if (node->data > root->data)
        {
            root->right = BSTInsert(root->right, node);
            root->right->parent = root;
        }
        return root;
    }

    void autocompleteHelper(RBNode *node, string prefix, vector<string> &results, int &count)
    {
        if (!node || count == 0)
        {
            return;
        }

        string s = node->data;
        string p = prefix;
      
        for (char &c : s) {
        c = tolower(c);
        }
        for (char &c : p) {
        c = tolower(c);
        }

        if (s.find(p) == 0)
        {
            results.push_back(node->data);
            count--;
            autocompleteHelper(node->left, prefix, results, count);
            autocompleteHelper(node->right, prefix, results, count);
        }
        else if (p < s)
        {
            autocompleteHelper(node->left, prefix, results, count);
        }
        else
        {
            autocompleteHelper(node->right, prefix, results, count);
        }
    }

    bool exists(RBNode *node, string val)
    {
        if (node == nullptr)
            return false;
        if (val == node->data)
            return true;
        if (val < node->data)
            return exists(node->left, val);
        return exists(node->right, val);
    }

public:
    DirectoryRB() { 
        root = nullptr; 
    }

    void insert(string data)
    {
        if (exists(root, data))
            return;
        RBNode *node = new RBNode(data);
        root = BSTInsert(root, node);
        fixViolation(root, node);
    }

    void autocomplete(string prefix)
    {
        int count = 5;
        vector<string> results;
        autocompleteHelper(root, prefix, results, count);

        if (results.empty())
        {
            cout << "No matches found." << endl;
        }
        else
        {
            for (const string &s : results)
                cout << "  -> " << s << endl;
        }
    }

    void search(string prefix)
    {
        int count = 5;
        vector<string> results;
        autocompleteHelper(root, prefix, results, count);
    }
};

DirectoryRB rb;

class DirectoryAVL
{
private:
    AVLNode *root;

    int height(AVLNode *N)
    {
        if (N == nullptr)
            return 0;
        return N->height;
    }

    int getBalance(AVLNode *N)
    {
        if (N == nullptr)
            return 0;
        return height(N->left) - height(N->right);
    }

    AVLNode *rightRotate(AVLNode *y)
    {
        AVLNode *x = y->left;
        AVLNode *T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    AVLNode *leftRotate(AVLNode *x)
    {
        AVLNode *y = x->right;
        AVLNode *T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    AVLNode *insertHelper(AVLNode *node, string value)
    {
        if (node == nullptr)
            return new AVLNode(value);

        if (value < node->data)
            node->left = insertHelper(node->left, value);
        else if (value > node->data)
            node->right = insertHelper(node->right, value);
        else
            return node;

        node->height = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);

        if (balance > 1 && value < node->left->data)
            return rightRotate(node);
        if (balance < -1 && value > node->right->data)
            return leftRotate(node);
        if (balance > 1 && value > node->left->data)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && value < node->right->data)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        return node;
    }

    bool isPrefix(string data, string prefix)
    {
        if (data.length() < prefix.length())
            return false;
        string d = data.substr(0, prefix.length());
        for (auto &c : d)
            c = tolower(c);
        for (auto &c : prefix)
            c = tolower(c);
        return d == prefix;
    }

    bool isLess(string a, string b)
    {
        for (auto &c : a)
            c = tolower(c);
        for (auto &c : b)
            c = tolower(c);
        return a < b;
    }

    void autocompleteHelper(AVLNode *node, string prefix, vector<string> &results, int &count)
    {
        if (!node || count == 0)
            return;

        if (isPrefix(node->data, prefix))
        {
            results.push_back(node->data);
            count--;
            autocompleteHelper(node->left, prefix, results, count);
            autocompleteHelper(node->right, prefix, results, count);
        }
        else if (isLess(prefix, node->data))
        {
            autocompleteHelper(node->left, prefix, results, count);
        }
        else
        {
            autocompleteHelper(node->right, prefix, results, count);
        }
    }

    void inOrderHelper(AVLNode *node)
    {
        if (node == nullptr)
            return;
        inOrderHelper(node->left);
        cout << "  " << node->data << endl;
        inOrderHelper(node->right);
    }

public:
    DirectoryAVL() { 
        root = nullptr; 
    }

    void insert(string value) { 
        root = insertHelper(root, value); 
    }

    void displayAll() { 
        inOrderHelper(root); 
    }

    void autocomplete(string prefix)
    {
        int count = 5;
        vector<string> results;
        autocompleteHelper(root, prefix, results, count);

        if (results.empty())
            cout << "  No matches found." << endl;
        else
        {
            for (const string &s : results)
                cout << "  -> " << s << endl;
        }
    }

    void search(string prefix)
    {
        int count = 5;
        vector<string> results;
        autocompleteHelper(root, prefix, results, count);
    }
};

DirectoryAVL avl;

class DirectoryBST
{
private:
    BSTNode *root;

    BSTNode *insertHelper(BSTNode *node, string value)
    {
        if (node == nullptr)
            return new BSTNode(value);

        if (value < node->data)
            node->left = insertHelper(node->left, value);
        else if (value > node->data)
            node->right = insertHelper(node->right, value);

        return node;
    }

    void inOrderHelper(BSTNode *node)
    {
        if (node == nullptr)
            return;
        inOrderHelper(node->left);
        cout << "  " << node->data << endl;
        inOrderHelper(node->right);
    }

    string toLower(string s)
    {
        for (char &c : s)
            c = tolower(c);
        return s;
    }

    bool startsWith(string data, string prefix)
    {
        if (data.length() < prefix.length())
            return false;
        string dataLower = toLower(data);
        string prefixLower = toLower(prefix);
        return dataLower.find(prefixLower) == 0;
    }

    
    void autocompleteHelper(BSTNode *node, string prefix, vector<string> &results, int &count)
    {
        if (node == nullptr || count == 0)
            return;

        if (startsWith(node->data, prefix))
        {
            results.push_back(node->data);
            count--;
            autocompleteHelper(node->left, prefix, results, count);
            autocompleteHelper(node->right, prefix, results, count);
        }
        else if (toLower(prefix) < toLower(node->data))
        {
            autocompleteHelper(node->left, prefix, results, count);
        }
        else
        {
            autocompleteHelper(node->right, prefix, results, count);
        }
    }

public:
    DirectoryBST() { 
        root = nullptr; 
    }

    void insert(string value) {
        root = insertHelper(root, value); 
    }

    void displayAll() {
        inOrderHelper(root); 
    }

    BSTNode *getRoot() {
        return root; 
    }

    void autocomplete(string prefix)
    {
        int count = 5;
        vector<string> results;
        autocompleteHelper(root, prefix, results, count);

        if (results.empty())
        {
            if (!prefix.empty())
                cout << "  No matches found." << endl;
        }
        else
        {
            for (const string &s : results)
            {
                cout << "  -> " << s << endl;
            }
        }
    }


    void search(string prefix)
    {
        int count = 5;
        vector<string> results;
        autocompleteHelper(root, prefix, results, count);
    }
};

DirectoryBST tree;

string stackToString(stack<char> s)
{
    string temp = "";

    while (!s.empty())
    {
        temp += s.top();
        s.pop();
    }

    reverse(temp.begin(), temp.end());
    return temp;
}

void runSearchUI()
{
    stack<char> typeStack;
    string currentSearch = "";

    while (true)
    {
        system("cls");

        cout << "==========================================" << endl;
        cout << "    DIRECTORY SEARCH (";
        if (currentengine == 1)
            cout << " BST)";
        else if (currentengine == 2)
            cout << " AVL)";
        else
            cout << " RB TREE)";
        cout << "    " << endl;
        cout << "==========================================" << endl;
        cout << " Press 'Esc' to Exit | 'Enter' to Select" << endl;


        currentSearch = stackToString(typeStack);

        cout << "Search: " << currentSearch << "_" << endl;
        cout << "---------------------" << endl;

        if (currentengine == 1)
        {
            tree.autocomplete(currentSearch);
        }
        else if (currentengine == 2)
        {
            avl.autocomplete(currentSearch);
        }
        else
        {
            rb.autocomplete(currentSearch);
        }
        cout << "---------------------" << endl;

        char keyPress = _getch();

        if (keyPress == 27)
        {
            break;
        }
        else if (keyPress == 8)
        {
            if (!typeStack.empty())
            {
                typeStack.pop();
            }
        }
        else if (keyPress == 13)
        {
            if (!currentSearch.empty())
            {
                cout << " You selected: " << currentSearch << endl;
                addToHistory(currentSearch);
                cout << " Press any key." << endl;
                _getch();
            }
        }
        else if (isprint(keyPress))
        {
            typeStack.push(keyPress);
        }
    }
}


struct Timer
{
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

    void Start()
    {
        start = std::chrono::high_resolution_clock::now();
    }

    void Stop()
    {
        end = std::chrono::high_resolution_clock::now();
    }

    long long ElapsedMicroseconds()
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }
};


void runBenchmark()
{
    system("cls");
    cout << "==========================================" << endl;
    cout << "      TREE PERFORMANCE BENCHMARK          " << endl;
    cout << "==========================================" << endl;
    cout << "Scenario: Inserting Sorted data (A-Z)." << endl;
    cout << "Sorted data forces a standard BST into a " << endl;
    cout << "straight line like linked list, killing speed." << endl;
    cout << "------------------------------------------" << endl;

    vector<string> sortedData = dataset;
    sort(sortedData.begin(), sortedData.end());

    DirectoryBST testBST;
    DirectoryAVL testAVL;
    DirectoryRB testRB;
    Timer timer;
    long long bstTime, avlTime, rbTime;

    cout << "[Test 1] Inserting " << sortedData.size() << " sorted items." << endl;

    timer.Start();
    for (string s : sortedData)
        testBST.insert(s);
    timer.Stop();
    bstTime = timer.ElapsedMicroseconds();
    cout << "  BST :  " << bstTime << " us (Slowest - Linear Growth)" << endl;

    timer.Start();
    for (string s : sortedData)
        testAVL.insert(s);
    timer.Stop();
    avlTime = timer.ElapsedMicroseconds();
    cout << "  AVL :    " << avlTime << " us (Slower Insert due to Rotations)" << endl;

    timer.Start();
    for (string s : sortedData)
        testRB.insert(s);
    timer.Stop();
    rbTime = timer.ElapsedMicroseconds();
    cout << "  RB  :    " << rbTime << " us (Fastest Balanced Insert)" << endl;

    string query = sortedData.back();
    cout << "[Test 2] Searching for last item ('" << query << "')." << endl;
    cout << "(Running 1000 searches to measure impact)" << endl;

    timer.Start();
    for (int i = 0; i < 1000; i++)
        testBST.search(query);
    timer.Stop();
    cout << "  BST Search: " << timer.ElapsedMicroseconds() << " us (Terrible - O(N))" << endl;

    timer.Start();
    for (int i = 0; i < 1000; i++)
        testAVL.search(query);
    timer.Stop();
    cout << "  AVL Search: " << timer.ElapsedMicroseconds() << " us (Perfect - O(log N))" << endl;

    timer.Start();
    for (int i = 0; i < 1000; i++)
        testRB.search(query);
    timer.Stop();
    cout << "  RB Search : " << timer.ElapsedMicroseconds() << " us (Great - O(log N))" << endl;

    cout << "\n------------------------------------------------" << endl;
    cout << "VERDICT: BST fails on sorted data. AVL/RB remain fast." << endl;
    cout << "Press any key to return..." << endl;
    _getch();
}


int main()
{

    loadData();
    if (dataset.empty())
    {
        cout << "Dataset is empty. Exiting." << endl;
        return 1;
    }

    loadHistory();

    cout << "Building Binary Search Tree." << endl;
    for (int i = 0; i < dataset.size(); ++i)
        tree.insert(dataset[i]);
    cout << "Building AVL Tree." << endl;
    for (int i = 0; i < dataset.size(); ++i)
        avl.insert(dataset[i]);
    cout << "Building Red-Black Tree." << endl;
    for (int i = 0; i < dataset.size(); ++i)
        rb.insert(dataset[i]);

    cout << "All structures built successfully." << endl;
    cout << "Press any key to start." << endl;
    _getch();


    char choice;
    while (true)
    {
        system("cls");
        cout << "==========================================" << endl;
        cout << "               AUTOCOMPLETE SYSTEM        " << endl;
        cout << "         By: ASHUTOSH PANDEY   2401030284" << endl;
        cout << "             LAVANYA RASTOGI   2401030289" << endl;
        cout << "             SHIVESH BAJPAI    2401030296" << endl;
        cout << "             UPANSHI MITTAL    2401030303" << endl;
        cout << "==========================================" << endl;

        cout << "Current Search Engine: ";
        if (currentengine == 1)
            cout << "BST)";
        else if (currentengine == 2)
            cout << "AVL)";
        else
            cout << "RB)";
        cout << endl;
        cout << "  [1] Search Directory (Autocomplete)" << endl;
        cout << "  [2] View Recent Search History" << endl;
        cout << "  [3] Display All Data (Alphabetical)" << endl;
        cout << "  [4] Run Performance Benchmark " << endl;
        cout << "  [5] Switch Search Engine (Current: ";
        if (currentengine == 1)
            cout << "BST)";
        else if (currentengine == 2)
            cout << "AVL)";
        else
            cout << "RB)";
        cout << endl;
        cout << "  [6] Exit" << endl;

        cout << "   Choose an option: " << endl;

        choice = _getch();

        switch (choice)
        {
        case '1':
            runSearchUI();
            break;

        case '2':
            displayHistory();
            break;

        case '3':
            system("cls");
            cout << "==========================================" << endl;
            cout << "         ALL DATA (Alphabetical)          " << endl;
            cout << "==========================================" << endl;
            tree.displayAll();
            cout << "Press any key to return to the menu." << endl;
            _getch();
            break;

        case '4':
            runBenchmark();
            break;
        case '5':
            currentengine++;
            if (currentengine > 3)
                currentengine = 1;
            cout << " Switched search engine. Press any key to continue." << endl;
            _getch();
            break;
        case '6':
            cout << "Saving history." << endl;
            saveHistory();
            cout << "Exiting. Goodbye!" << endl;
            return 0;

        default:
            cout << "Invalid choice. Please press 1, 2, 3, 4, 5 or 6" << endl;
            cout << " Press any key to continue." << endl;
            _getch();
            break;
        }
    }

    return 0;
}