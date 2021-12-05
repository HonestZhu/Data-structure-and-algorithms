#include <iostream>
#include <cmath>
#include <ctime>

using namespace std;

template <typename K, typename E>
struct SkipNode
{
    typedef pair<const K, E> pairType;

    pairType element;
    SkipNode<K, E> **next;

    SkipNode(const pairType &thePair, int size) : element(thePair)
    {
        next = new SkipNode<K, E> *[size];
    }
};

template <typename K, typename E>
class SkipList
{
public:
    SkipList(K largeKey, int maxPairs, float prob);

    int level() const;   // 获得新节点的层数
    SkipNode<K, E> *search(const K theKey) const;   // 寻找第一个关键字不小于 theKey 的节点，并将每一层最后一个节点存在 last 中
    void insert(const pair<const K, E> &thePair);   // 插入节点
    void erase(const K &theKey);   // 删除节点
    void output() const //顺序输出数据
    {
        SkipNode<K, E> *theNode = head->next[0];
        while (theNode != tail)
        {
            cout << theNode->element.second << endl;
            theNode = theNode->next[0];
        }
    }

private:
    K maxKey;              // 记录最大关键字
    int maxLevel;          // 记录最大层数
    int levels;            // 记录当前最大层数
    float cutOff;          // 值为 prob * RANDMAX,用来确定层数
    int size;              // 记录当前元素个数
    SkipNode<K, E> *head;  // 头结点，不存值，next指针指向tail
    SkipNode<K, E> *tail;  // 尾结点，值为maxKey
    SkipNode<K, E> **last; // 记录查找时，每一次经过的最后一个节点，用于插入和删除节点
};

template <typename K, typename E>
SkipList<K, E>::SkipList(K largeKey, int maxPairs, float prob)
{
    // largeKey 表示最大关键字
    // maxPairs 表示最多有多少个pair, prob 是0-1内的值, 创建 i 层的概率为 prob^i
    // 用maxPars 和 prob 可以计算出最多需要多少层
    maxKey = largeKey;
    levels = 0;
    cutOff = prob * RAND_MAX; // 将 prob 投射到 rand() 的值域上，等效于 rand() % 1
    size = 0;
    maxLevel = (int)ceil(logf((float)maxPairs) / logf(1 / prob)) - 1;

    pair<K, E> theTail;
    theTail.first = largeKey;
    head = new SkipNode<K, E>(theTail, maxLevel + 1);
    tail = new SkipNode<K, E>(theTail, 0);
    last = new SkipNode<K, E> *[maxLevel + 1];

    for (int i = 0; i <= maxLevel; ++i)
        head->next[i] = tail;
}

template <typename K, typename E>
SkipNode<K, E> *SkipList<K, E>::search(const K theKey) const
{
    // 查找前，会保证 theKey < maxKey

    SkipNode<K, E> *preNode = head;
    for (int i = levels; i >= 0; --i)
    {
        // 每一层遍历，直到 preNode->next[i]->element.first >= theKey 或者 下一个节点为尾结点
        while (preNode->next[i]->element.first < theKey)
            preNode = preNode->next[i];
        // preNode 记录的为上一步，将其加入 last 数组中
        last[i] = preNode;
    }
    // 返回的为第一个不小于 theKey 的节点的指针
    return preNode->next[0];
}

template <typename K, typename E>
int SkipList<K, E>::level() const
{
    int lev = 0;
    // lev 为 i 的概率为 prob ^ i, lev最大不会超过maxLevel
    while (rand() <= cutOff && lev < maxLevel)
        lev++;
    return lev;
}

template <typename K, typename E>
void SkipList<K, E>::insert(const pair<const K, E> &thePair)
{
    // 插入节点的关键字不能大于等于最大关键字
    if (thePair.first >= maxKey)
        return;

    SkipNode<K, E> *theNode = search(thePair.first);
    // 如果存在该节点，则修改节点的值
    if (theNode->element.first == thePair.first)
    {
        theNode->element.second == thePair.second;
        return;
    }

    // 否则为新节点确定层数，新层数最多比当前层数大 1，这是为了省空间
    int theLevel = level();
    if (theLevel > levels)
    {
        theLevel = ++levels;
        last[theLevel] = tail;
    }

    SkipNode<K, E> *newNode = new SkipNode<K, E>(thePair, theLevel + 1);
    for (int i = 0; i <= theLevel; ++i)
    {
        newNode->next[i] = last[i]->next[i];
        last[i]->next[i] = newNode;
    }
    size++;
}

template <typename K, typename E>
void SkipList<K, E>::erase(const K &theKey)
{
    if (theKey >= maxKey)
        return;

    SkipNode<K, E> *theNode = search(theKey);

    // 未查询到该节点则直接退出
    if (theNode->element.first != theKey)
        return;

    // 从低到高，遍历每一个存有theNode的层，将其删除
    for (int i = 0; i <= levels && last[i]->next[i] == theNode; ++i)
    {
        last[i]->next[i] = theNode->next[i];
    }

    // 等最高层无其他节点时，递归地减少高度
    while (levels && head->next[levels] == tail)
        --levels;

    delete theNode;
    size--;
}

void skipListTest()
{
    srand((unsigned int)time(NULL));
    SkipList<int, int> s(100, 10, 0.5);
    s.insert(pair<int, int>(1, 1));
    s.insert(pair<int, int>(5, 2));
    s.insert(pair<int, int>(3, 3));
    s.insert(pair<int, int>(2, 4));
    s.insert(pair<int, int>(4, 5));
    s.erase(1);
    s.output();
}

int main()
{
    skipListTest();
    return 0;
}