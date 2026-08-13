#include "dispatch_queue.h"

void dispatch_queue::enqueue_train(const train& t)
{
    HeapItem item;
    item.priority = t.get_departure_time();  
    item.value = t;

    queue.insert(item);
}

train dispatch_queue::dispatch_next()
{
    return queue.extract_min().value;
}

train dispatch_queue::peek_next() const
{
    return queue.peek().value;
}

bool dispatch_queue::empty() const
{
    return queue.empty();
}

size_t dispatch_queue::size() const
{
    return queue.size();
}