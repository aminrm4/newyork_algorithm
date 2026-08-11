#include "min_heap.h"
#include <algorithm>
#include <utility>
#include <stdexcept>
using namespace std;

void MinHeap::insert(const HeapItem &HI)
{
    heap.push_back(HI);
    size_t idx = size() -1;
    heapify_up(idx);
}

HeapItem MinHeap::peek() const
{
    if (!size()){
        throw std::out_of_range("heap is empty");
    }
    return heap.at(0);
}

HeapItem MinHeap::extract_min()
{
    if (empty())
    {
        throw std::out_of_range("heap is empty");
    }

    HeapItem minItem = heap[0];

    heap[0] = heap.back();
    heap.pop_back();

    if (!empty())
    {
        heapify_down(0);
    }

    return minItem;
}

void MinHeap::heapify_up(int idx)
{
    while (idx > 0)
    {
        int parent = (idx - 1) / 2;

        if (heap[idx].priority >= heap[parent].priority)
            break;

        std::swap(heap[parent], heap[idx]);
        idx = parent;
    }
}

bool MinHeap::empty() const
{
    return heap.empty();
}

std::size_t MinHeap::size() const
{
    return heap.size();
}
void MinHeap::heapify_down(int idx)
{
    while (true)
    {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;

        if (left >= static_cast<int>(heap.size()))
            break;

        int smallest = left;

        if (right < static_cast<int>(heap.size()) &&
            heap[right].priority < heap[left].priority)
        {
            smallest = right;
        }

        if (heap[idx].priority <= heap[smallest].priority)
            break;

        std::swap(heap[idx], heap[smallest]);
        idx = smallest;
    }
}