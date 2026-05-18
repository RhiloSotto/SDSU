#include <iostream>
using namespace std;

#include "Employee.h"

#ifndef OWNER
#define OWNER

class Owner : public Employee {
    private:
        
    public:
        /* CONSTRUCTORS */
        Owner(string name = "NoName", int id = 0, char employeeClass = 'O', int salary = 15000);

        /* ACCESSOR FUNCTIONS */
        
        /* MUTATOR FUNCTIONS */
        void calculateSalary(int profit) override{
            mySalary = 15000 + 0.60 * profit;
        }

        void editInfo() override{
            Employee::editInfo();
        }

        /* DISPLAY FUNCTIONS */
        void Display(ostream& out) const override{
            Employee::Display(out);
        }
};

inline Owner::Owner(string name, int id, char employeeClass, int salary) 
: Employee(name, id, 'O', 15000) {

} 

inline ostream& operator<<(ostream& j, const Owner& o) {
    o.Display(j);
    return j;
}

#endif