// Name: Na He Jeon
// This is the cpp file for the Heap priority queue class.
// Heap priority queue is a type of priority queue, which stores items with certain values
// and assigned priorities, and when it is dequeued, the first items to be dequeued are items with
// the top priorities. The heap priority queue uses a binary tree algorithm, where each item has one or two "children"
// items. Enqueuing and dequeuing therefore involves a bubbling-up from child to parent, and a bubbling-down from
// parent to child.
// A Linkedlist priority queue has a fast enqueuing, dequeuing, and peeking speed because it uses a logN algorithm.
// I received help from the lecture slides, Catherine Xu (classmate), and William Bradbury (benevolent dormmate).

#ifndef _heappriorityqueue_h
#define _heappriorityqueue_h

#include <iostream>
#include <string>
#include "PQEntry.h"
using namespace std;

/*
 * For documentation of each member, see VectorPriorityQueue.h.
 */
class HeapPriorityQueue {
public:
    HeapPriorityQueue();
    ~HeapPriorityQueue();
    void changePriority(string value, int newPriority);
    void clear();
    string dequeue();
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const HeapPriorityQueue& queue);

private:
    //the internal data storage of items, in an array
    PQEntry* queue;

    //the capacity of the array
    int capacity;

    //the size of the array
    int mysize;
};

#endif
