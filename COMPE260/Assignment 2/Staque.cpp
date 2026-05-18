#include "Staque.h"
#include <iostream>
using namespace std;

/* STAQUE NODE */
// default constructor
StaqueNode::StaqueNode() {
    data = 0;
    next = nullptr;
    previous = nullptr;
}

// explicit value constructor
StaqueNode::StaqueNode(int initData) {
    data = initData;
    next = nullptr;
    previous = nullptr;
}


/* STAQUE */
// default constructor
Staque::Staque() {
    head = nullptr;
    tail = nullptr;
}

// deconstructor
Staque::~Staque() {
   while (head) {
      StaqueNode* toBeDeleted = head;
      head = head->next;
      delete toBeDeleted;
   }
}

// get even number from staque
int Staque::GetEven() {
    if (head->data %2 != 0) {
        cout << "No even numbers!" << endl;
        return 1;
    }
    else 
        return head->data;
}
// get odd number from staque
int Staque::GetOdd() {
    if (tail->data % 2 != 1) {
        cout << "No odd numbers!" << endl;
        return 0;
    }
    else
        return tail->data;
}


// insert numbers into staque; evens in the front, odds in the back [LIFO] 
Staque& Staque::Insert(int data) {
    StaqueNode* newNode = new StaqueNode(data);
    if (head == nullptr) {      // empty list
        head = newNode;
        tail = newNode;
    }
    else if (data % 2 == 0) {   // even number
        newNode->next = head;
        head->previous = newNode;
        head = newNode;
    }
    else {                      // odd number
        tail->next = newNode;
        newNode->previous = tail;
        tail = newNode;
    }
    return *this;
}

// delete an even number in staque [LIFO]
Staque& Staque::DeleteEven() {
    if (head == nullptr) {              // empty list
        cout << "List is empty!" << endl;
    }
    else if (head == tail) {            // last element
        StaqueNode* toBeDeleted = head;
        head = nullptr;
        tail = nullptr;
        delete toBeDeleted;
    }
    else if (head->data % 2 != 0) {     // no more even numbers
        cout << "No even numbers!" << endl;
    }
    else {                              // remove an even number
        StaqueNode* toBeDeleted = head;
        head = head->next;
        head->previous = nullptr;
        delete toBeDeleted;
    }
    return *this;
}

// delete an odd number in staque [LIFO]
Staque& Staque::DeleteOdd() {
    if (tail == nullptr) {              // empty list
        cout << "List is empty!" << endl;
    }
    else if (head == tail) {            // last element
        StaqueNode* toBeDeleted = tail;
        head = nullptr;
        tail = nullptr;
        delete toBeDeleted;
    }
    else if (tail->data % 2 != 1) {     // no more odd numbers
        cout << "No odd numbers!" << endl;
    }
    else {                              // remove an odd number
        StaqueNode* toBeDeleted = tail;
        tail = tail->previous;
        tail->next = nullptr;
        delete toBeDeleted;
    }
    return *this;
}

// delete all nodes in staque
Staque& Staque::Clear() {
    while (head) {
        StaqueNode* toBeDeleted = head;
        head = head->next;
        delete toBeDeleted;
    }
    tail = nullptr;
    cout << "Cleared staque!" << endl;
    return *this;
}

// display all staque nodes
void Staque::Display(ostream& out) const{
    StaqueNode* currentNode = head;
    while (currentNode) {
        out << currentNode->data << " "; 
        currentNode = currentNode->next;
    }
    out << endl;
}

// overloaded << operator, returns ostream&, can chain
ostream& operator<<(ostream& j, const Staque& s) {
    s.Display(j);
    return j;
}