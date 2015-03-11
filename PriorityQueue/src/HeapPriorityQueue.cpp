// Name: Na He Jeon
// This is the cpp file for the Heap priority queue class. Look at .h file for documentation on member functions.
// Heap priority queue is a type of priority queue, which stores items with certain values
// and assigned priorities, and when it is dequeued, the first items to be dequeued are items with
// the top priorities. The heap priority queue uses a binary tree algorithm, where each item has one or two "children"
// items. Enqueuing and dequeuing therefore involves a bubbling-up from child to parent, and a bubbling-down from
// parent to child.
// A Linkedlist priority queue has a fast enqueuing, dequeuing, and peeking speed because it uses a logN algorithm.
// I received help from the lecture slides, Catherine Xu (classmate), and William Bradbury (benevolent dormmate).

#include "HeapPriorityQueue.h"

//O(1)
HeapPriorityQueue::HeapPriorityQueue() {
    capacity = 10;
    mysize = 0;
    queue = new PQEntry[capacity];
}

//O(N)
HeapPriorityQueue::~HeapPriorityQueue() {
    clear();
    delete[] queue;
}

//O(N)
void HeapPriorityQueue::changePriority(string value, int newPriority) {
    int count = 0;
    int j;
    for(int i = 1; i <= mysize; i++){
        if(queue[i].value == value){
            count++;
            if(queue[i].priority < newPriority){
                throw "Value is present in the queue and has a more urgent priority.";
            } else{
                queue[i].priority = newPriority;
                j = i;
                break;
            }
        }
    }
    if(count == 0){
        throw "None of the elements match the given value.";
    }
    while((j != 1) && (queue[j].priority < queue[j/2].priority)){
        PQEntry temp = queue[j/2];
        queue[j/2] = queue[j];
        queue[j] = temp;
        j = j/2;
    }

}

//O(NlogN)
void HeapPriorityQueue::clear() {
    for(int i = 1; i <= mysize; i++){
        dequeue();
    }
    mysize = 0;
}

//O(logN)
string HeapPriorityQueue::dequeue() {
    if(mysize == 0){
        throw "Queue does not have any elements";
    }
    string pr = queue[1].value;
    queue[1] = queue[mysize];
    mysize--;
    int i = 1;
    while((mysize != 0) && (i*2 <= mysize)){
        PQEntry parent = queue[i];
        PQEntry child1 = queue[i*2];
        PQEntry child2 = PQEntry("", (parent.priority + 1)); //construct a child2, but child2 might not exist, so leave empty for now
        if((i*2 + 1) <= mysize){
            child2 = queue[i*2 + 1]; //if child2 does exist, then make its value the value of child2
        }
        if((parent.priority > child1.priority) || (parent.priority > child2.priority)){
            cout << "c" << endl;
            if((child1.priority < child2.priority) || ((child1.priority == child2.priority) && (child1.value <= child2.value))){
                queue[i] = queue[i*2];
                queue[i*2] = parent; //switch parent and child1
                i = i*2;
            } else{
                queue[i] = queue[i*2 + 1];
                queue[i*2 + 1] = parent; //switch parent and child2
                i = i*2 + 1;
            }
        } else if((parent.priority == child1.priority) && (parent.value > child1.value)){
            queue[i] = queue[i*2];
            queue[i*2] = parent;
            i = i*2;
        } else if((parent.priority == child2.priority) && (parent.value > child2.value)){
            queue[i] = queue[i*2 + 1];
            queue[i*2 + 1] = parent;
            i = i*2 + 1;
        } else{
            break;
        }
    }
    return pr;
}

//O(logN)
void HeapPriorityQueue::enqueue(string value, int priority) {
    if(capacity == mysize){
        capacity *= 2;
    }
    PQEntry newElement = PQEntry(value, priority);
    mysize++;
    queue[mysize] = newElement;
    int i = mysize;
    while((i != 1) && ((queue[i].priority < queue[i/2].priority) ||
                       ((queue[i].priority == queue[i/2].priority) && (queue[i].value < queue[i/2].value)))){
        PQEntry temp = queue[i/2];
        queue[i/2] = newElement;
        queue[i] = temp; //swap i with i/2 element
        i = i/2;
    }
}

//O(1)
bool HeapPriorityQueue::isEmpty() const {
    if(mysize == 0){
        return true;
    } else{
        return false;
    }
}

//O(1)
string HeapPriorityQueue::peek() const {
    return queue[1].value;
}

//O(1)
int HeapPriorityQueue::peekPriority() const {
    return queue[1].priority;
}

//O(1)
int HeapPriorityQueue::size() const {
    return mysize;
}

//O(N)
ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out << "{";
    for(int i = 1; i <= queue.mysize; i++){
        out << queue.queue[i];
        if(i == queue.mysize){
            break;
        }
        out << ", ";
    }
    out << "}";
    return out;
}
