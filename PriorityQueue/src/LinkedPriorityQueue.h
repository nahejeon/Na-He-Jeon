// Name: Na He Jeon
// This is the cpp file for the Linked list priority queue class.
// Linked list priority queue is a type of priority queue, which stores items with certain values
// and assigned priorities, and when it is dequeued, the first items to be dequeued are items with
// the top priorities.
// A Linkedlist priority queue has somewhat cumberson enqueuing process but has fast dequeueing/peeking speed.
// I received help from the lecture slides, Catherine Xu (classmate), and William Bradbury (benevolent dormmate).

#ifndef _linkedpriorityqueue_h
#define _linkedpriorityqueue_h

#include <cstddef>    // for NULL
#include <iostream>
#include <string>
#include "ListNode.h"
#include "PQEntry.h"
using namespace std;

/*
 * For documentation of each member, see VectorPriorityQueue.h.
 */
class LinkedPriorityQueue {
public:
    LinkedPriorityQueue();
    ~LinkedPriorityQueue();
    void changePriority(string value, int newPriority);
    void clear();
    string dequeue();
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const LinkedPriorityQueue& queue);

private:
    //a pointer that fronts to the frontmost object of the priority queue
    ListNode* front;

};

#endif
