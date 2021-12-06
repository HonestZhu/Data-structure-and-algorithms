#include <iostream>
#include <ctime>
#include <queue>
using namespace std;

// 二叉树节点
template <typename T>
struct binaryTreeNode
{
    T element;
    binaryTreeNode<T> *leftChild, *rightChild;

    binaryTreeNode() { leftChild = rightChild = nullptr; }
    binaryTreeNode(const T &theElement) : element(theElement)
    {
        leftChild = rightChild = nullptr;
    }
    binaryTreeNode(const T &theElement, binaryTreeNode *theLeftChild, binaryTreeNode *theRightChild) : element(theElement)
    {
        leftChild = theLeftChild;
        rightChild = theRightChild;
    }
};

template <typename K, typename E>
class bsTree
{
public:
    bsTree()
    {
        root = nullptr;
        size = 0;
    };

    // 查找目标键值对，未查找到则返回NULL
    pair<const K, E> *find(const K &theKey) const;
    // 插入目标键值对，如果已经存在关键字，则修改键值对的值
    void insert(const pair<const K, E> &thePair);
    // 删除目标键值对
    void erase(const K &theKey);
    // 升序遍历,将inOrderOutput封装
    void ascend() const { inOrderOutput(root); }
    // 层序打印
    void showTree() const;

private:
    // 升序遍历函数
    void inOrderOutput(const binaryTreeNode<pair<const K, E>> *node) const;
    binaryTreeNode<pair<const K, E>> *root;
    int size;
};

template <typename K, typename E>
pair<const K, E> *bsTree<K, E>::find(const K &theKey) const
{
    // 二分查找
    binaryTreeNode<pair<const K, E>> *cur = root;
    while (cur)
    {
        if (cur->element.first == theKey)
            return &(cur->element);
        else if (cur->element.first < theKey)
            cur = cur->rightChild;
        else
            cur = cur->leftChild;
    }
    return nullptr;
}

template <typename K, typename E>
void bsTree<K, E>::insert(const pair<const K, E> &thePair)
{
    binaryTreeNode<pair<const K, E>> *cur = root, *pre = nullptr;

    while (cur)
    {
        // 结束循环的条件: 1.查找到目标键值对
        //               2.未查找到，且当前所在节点一定存在一个外部节点放入目标键值对
        pre = cur;
        if (cur->element.first > thePair.first)
            cur = cur->leftChild;
        else if (cur->element.first < thePair.first)
            cur = cur->rightChild;
        else
        {
            cur->element.second = thePair.second;
            return;
        }
    }

    binaryTreeNode<pair<const K, E>> *newNode = new binaryTreeNode<pair<const K, E>>(thePair);

    // 将新节点插入合适的位置
    if (root)
    {
        if (pre->element.first < thePair.first)
            pre->rightChild = newNode;
        else
            pre->leftChild = newNode;
    }
    else
        root = newNode;
    size++;
}

template <typename K, typename E>
void bsTree<K, E>::erase(const K &theKey)
{
    binaryTreeNode<pair<const K, E>> *cur = root, *pre = nullptr;

    while (cur && cur->element.first != theKey)
    {
        pre = cur;
        if (cur->element.first > theKey)
            cur = cur->leftChild;
        else if (cur->element.first < theKey)
            cur = cur->rightChild;
    }

    if (cur == nullptr)
        return;

    if (cur->leftChild && cur->rightChild)
    {
        binaryTreeNode<pair<const K, E>> *son = cur->leftChild, *partent = cur;
        // 寻找左子树中的最大值，并记录其父节点
        while (son->rightChild)
        {
            partent = son;
            son = son->rightChild;
        }
        binaryTreeNode<pair<const K, E>> *newNode = new binaryTreeNode<pair<const K, E>>(son->element, cur->leftChild, cur->rightChild);

        // 要删除的节点为根节点
        if (pre == nullptr)
            root = newNode;
        // 要删除的为左子树
        else if (cur == pre->leftChild)
            pre->leftChild = newNode;
        // 要删除的为右子树
        else
            pre->rightChild = newNode;

        // 左子树最大的节点为其根节点(其不存在右子树)
        if (partent == cur)
            pre = newNode;
        else
            pre = partent;

        // 删除需要删除的节点
        delete cur;
        cur = son;
    }
    // 结束时，cur 和 pre 分别指向原来左子树中最大的节点 及其 父节点
    // 其最多存在一个左子树，将其和只存在一个子树的删除一同处理

    binaryTreeNode<pair<const K, E>> *node = nullptr;
    if (cur->leftChild)
        node = cur->leftChild;
    else
        node = cur->rightChild;

    if (cur == root)
        root = node;
    else
    {
        if (cur = pre->leftChild)
            pre->leftChild = node;
        else
            pre->rightChild = node;
    }

    size--;
    delete cur;
}

template <typename K, typename E>
void bsTree<K, E>::showTree() const
{
    if (root == nullptr)
        cout << "NULL" << endl;
    cout << "size: " << size << endl;
    queue<binaryTreeNode<pair<const K, E>> *> q;
    q.push(root);
    while (!q.empty())
    {
        int n = q.size();
        for (int i = 0; i < n; ++i)
        {
            binaryTreeNode<pair<const K, E>> *node = q.front();
            q.pop();
            printf("%-3d", node->element.first);
            if (node->leftChild)
                q.push(node->leftChild);
            if (node->rightChild)
                q.push(node->rightChild);
        }
        cout << endl;
    }
}

template <typename K, typename E>
void bsTree<K, E>::inOrderOutput(const binaryTreeNode<pair<const K, E>> *node) const
{
    if (node)
    {
        if (node->leftChild)
            inOrderOutput(node->leftChild);
        cout << node->element.first << ' ' << node->element.second << endl;
        if (node->rightChild)
            inOrderOutput(node->rightChild);
    }
}

int main()
{
    //srand(time(NULL));
    bsTree<int, int> bt;
    int n = 20;
    // int idx[11] = {30, 2, 5, 40, 35, 32, 31, 33, 80, 60, 85};
    for (int i = 0; i < n; ++i)
        bt.insert({rand() % n, i});
        //bt.insert({idx[i], idx[i]});
    bt.erase(40);
    bt.erase(30);
    bt.showTree();
    //bt.ascend();
}