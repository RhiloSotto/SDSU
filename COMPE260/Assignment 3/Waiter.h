#include <iostream>
using namespace std;

#include "Employee.h"

#ifndef WAITER
#define WAITER

class Waiter : public Employee {
    private:
        int myTips;
        int myYears;
    public:
        /* CONSTRUCTORS */
        Waiter(string name = "NoName", int id = 0, char employeeClass = 'W', int salary = 3000, int tips = 0, int years = 0);

        /* ACCESSOR FUNCTIONS */
        int getTips() const{
            return myTips;
        }

        int getYears() const{
            return myYears;
        }

        /* MUTATOR FUNCTIONS */
        // function included in base class to allow array access
        void setTips(int tips) {
            if (tips < 0)   // tips can't be negatives
            myTips = 0;
            else 
            myTips = tips;
        }

        void setYears(int years) {
            myYears = years;
        }
        
        void calculateSalary(int profit) override{
            mySalary = 3000 + myTips;
        }

        void editInfo() override{
            Employee::editInfo();
            cout << "New Years of Service - ";
            cin >> myYears;
        }

        /* DISPLAY FUNCTIONS */
        void Display(ostream& out) const override{
            Employee::Display(out);
            out << "Years of Service: " << myYears
            << endl;
        }
};

inline Waiter::Waiter(string name, int id, char employeeClass, int salary, int tips, int years) 
: Employee(name, id, 'W', 3000), myTips(tips), myYears(years) {

}

inline ostream& operator<<(ostream& j, const Waiter& w) {
    w.Display(j);
    return j;
}

#endif