#include <iostream>
#include <queue>
#include <ctime>
using namespace std;

template <typename T>
struct node
{
    T val;
    int height;
    node *leftChild, *rightChild;

    node() : val(0), height(0), leftChild(nullptr), rightChild(nullptr) {}
    node(T theVal) : val(theVal), height(0), leftChild(nullptr), rightChild(nullptr) {}
    node(T theVal, node *theLeftChild, node *theRightChild)
    {
        val = theVal;
        height = 0;
        leftChild = theLeftChild;
        rightChild = theRightChild;
    }
};

template <typename T>
class AVLTree
{
public:
    AVLTree() : root(nullptr), size(0) {}

    int getHeight(node<T> *root) const;
    int height() const { return getHeight(root); }

    // 左旋
    node<T> *left_rotate(node<T> *root);
    // 右旋
    node<T> *right_rotate(node<T> *root);
    // 获取平衡因子
    int getFactor(node<T> *root) const;
    // 自动平衡
    node<T> *treeRebalance(node<T> *root);

    void insert(const T &val) { insert(root, val); }
    void erase(const T &val) { erase(root, val); }

    void insert(node<T> *&root, const T &val);
    void erase(node<T> *&root, const T &val);

    void show() const;
    void ascend() const
    {
        cout << "升序: ";
        inOrderShow(root);
    }

private:
    // 中序遍历
    void inOrderShow(node<T> *root) const;
    node<T> *root;
    int size;
};

template <typename T>
void AVLTree<T>::inOrderShow(node<T> *root) const
{
    if (!root)
        return;
    inOrderShow(root->leftChild);
    cout << root->val << ' ';
    inOrderShow(root->rightChild);
}

template <typename T>
int AVLTree<T>::getHeight(node<T> *root) const
{
    if (root == nullptr)
        return 0;
    return max(getHeight(root->leftChild), getHeight(root->rightChild)) + 1;
}

template <typename T>
node<T> *AVLTree<T>::right_rotate(node<T> *root)
{
    node<T> *theLeft = root->leftChild;

    root->leftChild = theLeft->rightChild;
    theLeft->rightChild = root;

    root->height = max(getHeight(root->leftChild), getHeight(root->rightChild)) + 1;
    theLeft->height = max(getHeight(theLeft->leftChild), root->height) + 1;

    return theLeft;
}

template <typename T>
node<T> *AVLTree<T>::left_rotate(node<T> *root)
{
    node<T> *theRight = root->rightChild;

    root->rightChild = theRight->leftChild;
    theRight->leftChild = root;

    root->height = max(getHeight(root->leftChild), getHeight(root->rightChild)) + 1;
    theRight->height = max(getHeight(theRight->leftChild), root->height) + 1;

    return theRight;
}

template <typename T>
// 获取平衡因子
int AVLTree<T>::getFactor(node<T> *root) const
{
    if (root == nullptr)
        return 0;
    return getHeight(root->leftChild) - getHeight(root->rightChild);
}

template <typename T>
node<T> *AVLTree<T>::treeRebalance(node<T> *root)
{
    int factor = getFactor(root);
    if (factor > 1 && getFactor(root->leftChild) > 0)
        return right_rotate(root);
    else if (factor > 1 && getFactor(root->leftChild) <= 0)
    {
        root->leftChild = left_rotate(root->leftChild);
        return right_rotate(root);
    }
    else if (factor < -1 && getFactor(root->rightChild) <= 0)
        return left_rotate(root);
    else if (factor < -1 && getFactor(root->rightChild) > 0)
    {
        root->rightChild = right_rotate(root->rightChild);
        return left_rotate(root);
    }
    else
        return root;
}

template <typename T>
void AVLTree<T>::insert(node<T> *&root, const T &val)
{
    if (root == nullptr)
        root = new node<T>(val);
    else if (val == root->val)
        return;
    else if (val < root->val)
        insert(root->leftChild, val);
    else
        insert(root->rightChild, val);

    root = treeRebalance(root);
    root->height = getHeight(root);
}

// 层序遍历
template <typename T>
void AVLTree<T>::show() const
{
    if (root == nullptr)
        return;
    queue<node<T> *> q;
    q.push(root);
    while (!q.empty())
    {
        int n = q.size();
        for (int i = 0; i < n; ++i)
        {
            node<T> *node = q.front();
            q.pop();
            printf("%-3d", node->val);
            if (node->leftChild)
                q.push(node->leftChild);
            if (node->rightChild)
                q.push(node->rightChild);
        }
        cout << endl;
    }
}

template <typename T>
void AVLTree<T>::erase(node<T> *&root, const T &val)
{
    if (root == nullptr)
        return;

    if (val < root->val)
        erase(root->leftChild, val);
    else if (val > root->val)
        erase(root->rightChild, val);
    else
    {
        if (root->rightChild)
        {
            node<T> *pre = root;
            node<T> *cur = root->rightChild;
            while (cur->leftChild)
            {
                pre = cur;
                cur = cur->leftChild;
            }
            if (pre != root)
                pre->leftChild = cur->rightChild;
            else
                root = root->rightChild;

            root->val = cur->val;
            delete cur;
        }
        else
        {
            node<T> *tmp = root;
            root = root->leftChild;
            delete tmp;
        }
    }
    treeRebalance(root);
}


int main()
{
    AVLTree<int> t;
    srand(time(NULL));
    int n = 20;
    cout << "原始数据: ";
    for (int i = 0; i < n; ++i)
    {
        int num = rand() % n;
        cout << num << ' ';
        t.insert(num);
    }
    cout << endl;
    t.show();
    t.ascend();
}