// Name: Na He Jeon
// This is the cpp file for the Linked list priority queue class. Look at .h file for documentation on member functions.
// Linked list priority queue is a type of priority queue, which stores items with certain values
// and assigned priorities, and when it is dequeued, the first items to be dequeued are items with
// the top priorities.
// A Linkedlist priority queue has somewhat cumberson enqueuing process but has fast dequeueing/peeking speed.
// I received help from the lecture slides, Catherine Xu (classmate), and William Bradbury (benevolent dormmate).

#include "LinkedPriorityQueue.h"

//O(1)
LinkedPriorityQueue::LinkedPriorityQueue() {
    front = NULL;
}

//O(N)
LinkedPriorityQueue::~LinkedPriorityQueue() {
    clear();
}

//O(N)
void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    ListNode* curr = front;
    ListNode* prev;
    while(curr->value != value){
        prev = curr;
        if(curr->next == NULL){
            throw "None of the elements match the given value.";
        }
        curr = curr->next;
    }
    if(curr->priority < newPriority){
        throw "Value is present in the queue and has a more urgent priority.";
    } else{
        curr->priority = newPriority;
    }
    prev->next = curr->next;    //take the changed item away from the queue
    enqueue(curr->value, curr->priority);       //and enqueue a new item, with the same
                                            //value and priority, and enqueue it back so it matches the order
    delete curr;    //delete the old item
}

//O(N)
void LinkedPriorityQueue::clear() {
    ListNode* curr = front;
    ListNode* prev;
    while(curr->next != NULL){
        prev = curr;
        curr = curr->next;
        delete prev;
    }
    delete curr;
    front = NULL;
}

//O(1)
string LinkedPriorityQueue::dequeue() {
    if(front == NULL){
        throw "Queue does not have any elements";
    }
    ListNode* temp = front;
    string s = temp->value;
    front = front->next;        //make the next item be the front
    delete temp;        //delete the object that got kicked out of the queue
    return s;
}

//O(N)
void LinkedPriorityQueue::enqueue(string value, int priority) {
    if(front == NULL){
        front = new ListNode(value, priority, NULL);
    } else{
        ListNode* insert = new ListNode(value, priority, NULL);
        ListNode* curr = front;
        ListNode* prev;
        if(curr->priority > priority || ((curr->priority == priority) && (curr->value >= value))){
            //if the priority is less than the priority of the first item
            //or even if the priority is the same, if the value is in earlier alphabets
            insert->next = curr;
            front = insert;
            return;
        }
        while((curr->priority < priority) || ((curr->priority == priority) && (curr->value < value))){
            //while the current priority is less than the given priority
            //or even if the priority is the same, if the given value is in later alphabets
            prev = curr;
            if(curr->next == NULL){
                break;
            }
            curr = curr->next;
        }
        if((curr->next == NULL) && ((curr->priority < priority) || ((curr->priority == priority) && (curr->value) < value))){
            curr->next = insert;
        } else{
            prev->next = insert;
            insert->next = curr;
        }
    }
}

//O(1)
bool LinkedPriorityQueue::isEmpty() const {
    if(front == NULL){
        return true;
    } else{
        return false;
    }
}

//O(1)
string LinkedPriorityQueue::peek() const {
    ListNode* temp = front;
    string s = temp->value;
    return s;
}

//O(1)
int LinkedPriorityQueue::peekPriority() const {
    ListNode* temp = front;
    int i = temp->priority;
    return i;
}

//O(N)
int LinkedPriorityQueue::size() const {
    int count = 0;
    ListNode* curr = front;
    while(curr != NULL){
        count++;
        curr = curr->next;
    }
    return count;
}

//O(N)
ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    out << "{";
    ListNode* curr = queue.front;
    while(curr != NULL){
        out << *curr;
        if(curr->next == NULL){
            break;
        }
        curr = curr->next;
        out << ", ";
    }
    out << "}";
    return out;
}
