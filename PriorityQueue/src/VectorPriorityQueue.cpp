// Name: Na He Jeon
// This is the cpp file for the Vector priority queue class. See .h file for documentation on member functions.
// Vector priority queue is a type of priority queue, which stores items with certain values
// and assigned priorities, and when it is dequeued, the first items to be dequeued are items with
// the top priorities.
// A Vector priority queue has fast enqueuing speed but has slow dequeueing/peeking speed, and overall
// has poor performance.
// I received help from the lecture slides, Catherine Xu (classmate), and William Bradbury (benevolent dormmate).

#include "VectorPriorityQueue.h"

//O(1)
VectorPriorityQueue::VectorPriorityQueue() {

}

//O(N)
VectorPriorityQueue::~VectorPriorityQueue() {
    clear();
}

//O(N)
void VectorPriorityQueue::changePriority(string value, int newPriority) {
    int count = 0;
    for(int i = 0; i < pq.size(); i++){
        if (pq[i].value == value){
            count++;
            if(pq[i].priority < newPriority){
                throw "Value is present in the queue and has a more urgent priority.";
            } else{
                pq[i].priority = newPriority;
                break;
            }
        }
    }
    if(count == 0){
        throw "None of the elements match the given value.";
    }

}

//O(N)
void VectorPriorityQueue::clear() {
    pq.clear();
}

//O(N)
string VectorPriorityQueue::dequeue() {
    if(pq.size() == 0){
        throw "Queue does not have any elements";
    }
    int rank = pq[0].priority;
    string first = pq[0].value;
    int bye = 0;
    for(int i = 0; i < pq.size(); i++){
        if(pq[i].priority < rank){   //if the priority is smaller than rank
            rank = pq[i].priority;   //make the rank that priority
            first = pq[i].value;    //and first is now that value
            bye = i;
        } else if((pq[i].priority == rank) && (pq[i].value < first)){
            first = pq[i].value;
            bye = i;
        }
    }
    pq.remove(bye);
    return first;
}

//O(1)
void VectorPriorityQueue::enqueue(string value, int priority) {
    pq.add(PQEntry(value, priority));

}

//O(1)
bool VectorPriorityQueue::isEmpty() const {
    if(pq.isEmpty()){
        return true;
    } else{
        return false;
    }
}

//O(N)
string VectorPriorityQueue::peek() const {
    int rank = pq[0].priority;
    string first = pq[0].value;
    for(int i = 0; i < pq.size(); i++){
        if(pq[i].priority < rank){
            rank = pq[i].priority;
            first = pq[i].value;
        } else if((pq[i].priority == rank) && (pq[i].value < first)){
            first = pq[i].value;
        }
    }
    return first;
}

//O(N)
int VectorPriorityQueue::peekPriority() const {
    int rank = pq[0].priority;
    for(int i = 0; i < pq.size(); i++){
        if(pq[i].priority < rank){
            rank = pq[i].priority;
        }
    }
    return rank;
}

//O(1)
int VectorPriorityQueue::size() const {
    return pq.size();
}

//O(N)
ostream& operator<<(ostream& out, const VectorPriorityQueue& queue) {
    out << "{";
    for(int i = 0; i < queue.pq.size(); i++){
        out << queue.pq[i];
        if(i == (queue.pq.size() - 1)){
            break;
        }
        out << ", ";
    }
    out << "}";
    return out;
}
