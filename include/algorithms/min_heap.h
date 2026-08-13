#pragma once
#include <vector>
#include "core/train.h"
struct HeapItem {
    train value;
    int priority;
};
class MinHeap {
private:
    std::vector<HeapItem> heap;
    void heapify_up(int idx);
    void heapify_down(int idx);
public:
    void insert(const HeapItem& HI);
    HeapItem peek() const;
    HeapItem extract_min();
    
    bool empty() const;
    std::size_t size() const;
};