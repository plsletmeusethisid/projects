#include "iostream"
#include "vector"

template<class T>
class MinHeap {
public:
    MinHeap(int d);
    ~MinHeap();
    void add(T item, int priority);
    const T& peek() const;
    void remove();
    int priority();
    void updatePriority(T item, int priority);
    bool isEmpty() const;
    void trickleDown(int index);
    void bubbleUp(int index);
    // void updatePriority(T item, int newPriority);
private:
    std::vector<std::pair<T, int>> vec;
    int find;
};

template<class T>
MinHeap<T>::MinHeap(int d) : find(d) {}

template<class T>
MinHeap<T>::~MinHeap() {}
template<class T>
void MinHeap<T>::updatePriority(T item, int priority) {
    int index = 0;
    for (int i = 0; i < vec.size(); ++i) {
        if (item == vec[i].first) {
            index = i;
            break;
        }
    }
    if (vec[index].second > priority) {
        vec[index].second = priority;
        bubbleUp(index);
    } else if (vec[index].second < priority) {
        vec[index].second = priority;
        trickleDown(index);
    }
}
template<class T>
int MinHeap<T>::priority() {
    if (!isEmpty()) {
        return this->vec[0].second;
    } else {
        throw std::logic_error("nothing in the heap");
    }
}
// template<class T>
// MinHeap<T>::updatePriority(T item, int newPriority) {

// }

template<class T>
const T& MinHeap<T>::peek() const {
    if (!isEmpty()) {
        return this->vec[0].first;
    } else {
        throw std::logic_error("nothing in the heap");
    }
}

template<class T>
void MinHeap<T>::bubbleUp(int index) {
    if (index > 0 && vec[index].second < vec[(index - 1) / find].second) {
        swap(vec[index], vec[((index - 1) / find)]);
        bubbleUp((index - 1) / find);
    }
}
template<class T>
void MinHeap<T>::trickleDown(int index) {
    int childPriority = find * index + 1;
    int swapIndex = childPriority;
    if (childPriority < vec.size()) {
        for (int i = 1; i < find; ++i) {
            if (vec[swapIndex].second > vec[childPriority + i].second && childPriority + i < vec.size()) {
                swapIndex = childPriority + i;
            }
        }
        if (vec[index].second > vec[swapIndex].second) {
            swap(vec[index], vec[swapIndex]);
            trickleDown(swapIndex);
        }
    }
}

template<class T>
void MinHeap<T>::add(T item, int priority) {
    this->vec.push_back(std::make_pair(item, priority));
    if (this->vec.size() > 1) {
        this->bubbleUp(vec.size() - 1);
    }
}

template<class T>
void MinHeap<T>::remove() {
    if (!this->isEmpty()) {
        swap(vec[0], vec[vec.size() - 1]);
        vec.pop_back();
        trickleDown(0);
    } else {
        throw std::logic_error("nothing in the heap");
    }
}

template<class T>
bool MinHeap<T>::isEmpty() const {
    if (this->vec.empty()) {
        return true;
    }
    return false;
}
