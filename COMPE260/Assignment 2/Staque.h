#include <iostream>
using namespace std;

#ifndef STAQUE
#define STAQUE

/* STAQUE NODE */
class StaqueNode {
    public:
        /* DATA MEMBERS*/
        int data;
        StaqueNode* next;
        StaqueNode* previous;

        /* CONSTRUCTORS*/
        // default constructor
        StaqueNode();
        // explicit value constructor
        StaqueNode(int initData);
};

/* STAQUE */
class Staque {
    public:
        /* CONSTRUCTORS */
        // default constructor
        Staque();

        /* DECONSTRUCTORS */
        // deconstructor
        ~Staque();

        /* ACCESSOR FUNCTIONS */
        // get even number from staque
        int GetEven();
        // get odd number from staque
        int GetOdd();

        /* MUTATOR FUNCTIONS */
        // insert numbers into staque; evens in the front, odds in the back [LIFO] 
        Staque& Insert(int data);
        // delete an even number in staque [LIFO]
        Staque& DeleteEven();
        // delete an odd number in staque [LIFO]
        Staque& DeleteOdd();
        // delete all nodes in staque
        Staque& Clear();
        
        /* DISPLAY FUNCTIONS */
        // helper function for <<
        void Display(ostream& out) const;

    private:
        /* DATA MEMBERS */
        StaqueNode* head;
        StaqueNode* tail;
};

// overloaded << operator, returns ostream&, can chain
ostream& operator<<(ostream& j, const Staque& s);

#endif