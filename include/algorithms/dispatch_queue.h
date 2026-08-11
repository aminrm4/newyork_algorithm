#pragma once

#include "min_heap.h"
#include "train.h"

class dispatch_queue
{
private:
    MinHeap queue;

public:
    void enqueue_train(const train& t);
    train dispatch_next();
    train peek_next() const;

    bool empty() const;
    size_t size() const;
};