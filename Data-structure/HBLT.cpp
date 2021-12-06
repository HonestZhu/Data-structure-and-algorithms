#include <iostream>
#include <ctime>
using namespace std;

// 二叉树节点
template <typename T>
struct binaryTreeNode
{
    T element;
    binaryTreeNode<T> *leftChild, *rightChild;

    binaryTreeNode() { leftChild = rightChild = nullptr; }
    binaryTreeNode(const T &theElement)
    {
        element = theElement;
        leftChild = rightChild = nullptr;
    }
    binaryTreeNode(const T &theElement, binaryTreeNode *theLeftChild, binaryTreeNode *theRightChild)
    {
        element(theElement);
        leftChild = theLeftChild;
        rightChild = theRightChild;
    }
};

template <typename T>
class HBLT
{
public:
    HBLT() { root = nullptr; treeSize = 0; }

    void push(const T &theElement);
    void pop();
    T top() const
    {
        if (root)
            return root->element.second;
        else return INT_MAX;
    }
    void merge(binaryTreeNode<pair<int, T>> *&a, binaryTreeNode<pair<int, T>> *&b);
    void merge(HBLT<T> &theHBLT);

private:
    binaryTreeNode<pair<int, T>> *root;
    int treeSize;
};

template <typename T>
void HBLT<T>::merge(binaryTreeNode<pair<int, T>> *&a, binaryTreeNode<pair<int, T>> *&b)
{
    // 合并两个最大左高树，最后合并的新最大左高树的根为 a

    if (b == nullptr)
        return;
    if (a == nullptr)
    {
        a = b;
        return;
    }

    // 根一定为最大值, 较小值会放到待合并的树的根上
    // 不用担心这个较小值在待合并的树中不满足最大，在后面递归的过程中会将树进行调整
    if (a->element.second < b->element.second)
        swap(a, b);

    // 将待合并的树合并到树的右子树上
    merge(a->rightChild, b);

    // 如果 a 的左子树为空，则将右子树移到左子树，右子树一定非空
    if (a->leftChild == nullptr)
    {
        a->leftChild = a->rightChild;
        a->rightChild = nullptr;
        // a 到最近的空叶子结点的距离很显然就是到右子树的距离 1
        a->element.first = 1;
    }
    else
    {
        // 将高度较高的树放到左侧
        if (a->leftChild->element.first < a->rightChild->element.first)
            swap(a->leftChild, a->rightChild);

        a->element.first = a->rightChild->element.first + 1;
    }
}

template <typename T>
void HBLT<T>::merge(HBLT<T> &theHBLT)
{
    merge(root, theHBLT.root);
    treeSize += theHBLT.treeSize;
    theHBLT.root = nullptr;
    theHBLT.treeSize = 0;
}

template <typename T>
void HBLT<T>::push(const T &theElement)
{
    // 新建一个只有根节点的树，将其合并到 root 上
    auto newNode = new binaryTreeNode<pair<int, T>>(pair<int, T>(1, theElement));

    merge(root, newNode);
    treeSize++;
}

template <typename T>
void HBLT<T>::pop()
{
    if (root == nullptr)
        return;

    // 将 root 删除后，将 left 设置为 root, 并将 right 合并到 root 上
    auto left = root->leftChild;
    auto right = root->rightChild;

    delete root;
    root = left;
    merge(root, right);
    treeSize--;
}

int main()
{
    srand(time(NULL));
    HBLT<int> hblt;
    int n = 100;
    for (int i = 0; i < n; ++i)
        hblt.push(rand() % (n + 1));

    for (int i = 0; i < n; ++i)
    {
        cout << hblt.top() << endl;
        hblt.pop();
    }
    return 0;       
}
