#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

// 大根堆
template <typename T>
class Heap
{
public:
    // 数组从下标 1 开始存储， cnt 代表元素个数
    Heap() { h.push_back(0); cnt = 0; }

    bool empty() const { return !cnt; }
    int size() const { return cnt; }

    T top() const { return h[1]; }
    void push(const T element);
    void pop();

    void down(const int idx);
    void up(const int idx);
    // 交换堆内下标为 a 和 b 的值
    void Heap_swap(int a, int b);
    // 自动缩减大小
    void change_size();

private:
    vector<T> h;
    // top 所在下标为 1
    int cnt;
};

template <typename T>
void Heap<T>::Heap_swap(int a, int b)
{
    T temp = h[a];
    h[a] = h[b];
    h[b] = temp;
}

template <typename T>
void Heap<T>::down(const int idx)
{
    int max = idx;
    if (idx * 2 <= cnt && h[max] < h[2 * idx])
        max = 2 * idx;
    if (idx * 2 + 1 <= cnt && h[max] < h[2 * idx + 1])
        max = 2 * idx + 1;
    if (max != idx)
    {
        Heap_swap(max, idx);
        down(max);
    }
}

template <typename T>
void Heap<T>::change_size()
{
    if(h.size() < h.capacity() / 4)
    {
        vector<T> temp(h.begin(), h.end());
        h.swap(temp);
    }
}

template <typename T>
void Heap<T>::up(const int idx)
{
    // 将当前元素和父节点比较，并进行迭代直到当前节点为根节点
    while (idx / 2 && h[idx] > h[idx / 2])
    {
        // 子节点 > 父节点，不满足大根堆，对其进行交换
        Heap_swap(idx, idx / 2);
        idx >>= 1;
    }
}

template <typename T>
void Heap<T>::push(const T element)
{
    cnt++;
    h.push_back(element);
    // 新元素被添加在二叉树最底一层
    // 对其进行上浮操作
    up(cnt);
}

template <typename T>
void Heap<T>::pop()
{
    // 删除堆顶，为了避免将所有元素向前移动1位
    // 将最后一个叶子节点放到堆顶，并对其进行下沉操作
    Heap_swap(cnt, 1);
    cnt--;
    h.pop_back();
    down(1);
    // 自动检测是否需要剪裁大小
    change_size();
}

void test()
{
    Heap<int> my_heap;
    int n = 1000;
    for(int i = 0; i < n; ++ i)
    {
        my_heap.push(rand() % (n + 1));
    }
    while(my_heap.size())
    {
        auto t = my_heap.top();
        my_heap.pop();
        cout << t << endl;
    }
}

int main()
{
    test();
    return 0;
}