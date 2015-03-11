// Name: Na He Jeon
// This is the header file for VectorPriorityQueue.cpp.
// Vector priority queue is a type of priority queue, which stores items with certain values
// and assigned priorities, and when it is dequeued, the first items to be dequeued are items with
// the top priorities.
// A Vector priority queue has fast enqueuing speed but has slow dequeueing/peeking speed, and overall
// has poor performance.
// I received help from the lecture slides, Catherine Xu (classmate), and William Bradbury (benevolent dormmate).

#ifndef _vectorpriorityqueue_h
#define _vectorpriorityqueue_h

#include <iostream>
#include <string>
#include "vector.h"
#include "PQEntry.h"
using namespace std;

class VectorPriorityQueue {
public:
    //Constructor for the VectorPriorityQueue class.
    //Input: nothing
    //Output: nothing
    VectorPriorityQueue();

    //Constructor for the VectorPriorityQueue class.
    //Input: nothing
    //Output: nothing
    ~VectorPriorityQueue();

    //Changes the priority of a given value to a given integer.
    //Input: string value: the value whose priority is to change.
    //       int newPriority: the integer to which the priority is to change.
    //Output: nothing
    void changePriority(string value, int newPriority);

    //Clears the internal data storage.
    //Input: nothing
    //Output: nothing
    void clear();

    //Removes the top priority item from the queue and returns its value.
    //Input: nothing
    //Output: nothing
    string dequeue();

    //Enqueues a new item with the given value and given priority.
    //Input: string value: the value of the item to be input.
    //       int priority: the priority of the item to be input.
    //Output: nothing
    void enqueue(string value, int priority);

    //Checks if the queue is empty.
    //Input: nothing
    //Output: the boolean value of whether or not the queue is empty.
    bool isEmpty() const;

    //Returns the value of the top priority item without removing it.
    //Input: nothing
    //Output: the value of the top priority item in the queue.
    string peek() const;

    //Returns the priority of the top priority item without removing it.
    //Input: nothing
    //Output: the priority of the top priority item in the queue.
    int peekPriority() const;

    //Returns the size of the queue.
    //Input: nothing
    //Output: the size of the queue.
    int size() const;

    //Prints out the contents of the queue.
    //Input: ostream out: the output stream
    //       VectorPriorityQueue queue: the queue to be printed out
    //Output: nothing
    friend ostream& operator <<(ostream& out, const VectorPriorityQueue& queue);

private:
    //this is the internal data storage
    Vector<PQEntry> pq;

};

#endif
