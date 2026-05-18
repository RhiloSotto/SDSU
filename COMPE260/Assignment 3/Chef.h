#include <iostream>
using namespace std;

#include "Employee.h"

#ifndef CHEF
#define CHEF

class Chef : public Employee {
    private:
        string myExpertise;

    public:
        /* CONSTRUCTORS */
        Chef(string name = "NoName", int id = 0, char employeeClass = 'C', int salary = 10000, string expertise = "None");

        /* ACCESSOR FUNCTIONS */
        string getExpertise() const{
            return myExpertise;
        }
        /* MUTATOR FUNCTIONS */
        void setExpertise(string expertise) {
            myExpertise = expertise;
        }

        void calculateSalary(int profit) override{
            mySalary = 10000 + 0.20 * profit;
        }

        void editInfo() override{
            Employee::editInfo();
            cout << "New Expertise - ";
            cin >> myExpertise;
        }

        /* DISPLAY FUNCTIONS */
        void Display(ostream& out) const override{
            Employee::Display(out);
            out << "Expertise: " << myExpertise
            << endl;
        }
};

inline Chef::Chef(string name, int id, char employeeClass, int salary, string expertise) 
: Employee(name, id, 'C', 10000), myExpertise(expertise) {

}

inline ostream& operator<<(ostream& j, const Chef& c) {
    c.Display(j);
    return j;
}

#endif