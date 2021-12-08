#include <iostream>

using namespace std;

template <typename T>
struct node
{
    T val;
    bool is_red;
    node *leftChild;
    node *rightChlid;
    node *parent;

    node() : val(0), is_red(true)
    {
        leftChild = rightChlid = parent = nullptr;
    }
    node(const T &theVal) : val(theVal), is_red(true)
    {
        leftChild = rightChlid = parent = nullptr;
    }
    node(const T &theVal, node *theLeft, node *theRight, node *theParent) : val(theVal)
    {
        leftChild = theLeft;
        rightChlid = theRight;
        parent = theParentl
    }
};

template <typename T>
class BSTree
{
public:
    BSTree();

    void left_rotate(node<T>*& node);
    void right_rotate(node<T>*& node);
    void update_tree(node<T>*& node);

    void insert(const T& val) { insert(root, val); }
    void insert(node<T>*& root, const T& val);
    void erase(const T& val) { erase(root, val); }
    void erase(node<T>*& root, const T& val);
private:
    node<T>* root;
};

template <typename T>
void BSTree<T>::left_rotate(node<T>*& node)
{
    node<T>* theRight = node->rightChlid;

    if()

    
}