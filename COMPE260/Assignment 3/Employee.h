#include <iostream>
using namespace std;

#ifndef EMPLOYEE
#define EMPLOYEE

/* EMPLOYEE BASE CLASS */
/* PURELY VIRTUAL */
class Employee {
    protected:
        string myName;
        int myId;
        char myEmployeeClass;
        int mySalary;

    public:
        /* CONSTRUCTORS */
        Employee(string name = "NoName", int id = 0, char employeeClass = 'N', int salary = 0);

        /* ACCESSOR FUNCTIONS*/
        string getName() const{
            return myName;
        }

        int getId() const{
            return myId;
        }

        char getEmployeeClass() const{
            return myEmployeeClass;
        }

        int getSalary() const{
            return mySalary;
        }


        /* MUTATOR FUNCTIONS */
        void setName(string name) {
            myName = name;
        }
        
        void setId(int id) {
            myId = id;
        }

        void setEmployeeClass(char employeeClass) {
            myEmployeeClass = employeeClass;
        }

        void setSalary(int salary) { 
            mySalary = salary;
        }

        // different definition in derived classes
        virtual void calculateSalary(int profit) = 0;

        // base edit info function
        virtual void editInfo() {
            cout << "New Name - ";
            cin >> myName;
            cout << "New ID - ";
            cin >> myId;
        }

        // function included in base class to allow array access
        virtual void setTips(int tips) {
        }

        /* DISPLAY FUNCTIONS */
        virtual void Display(ostream& out) const{
            out << "Employee: " << myName
            << ", ID: " << myId 
            << ", Employee Class: " << myEmployeeClass << endl
            << "Salary: $" << mySalary 
            << endl;
        }
};

inline Employee::Employee(string name, int id, char employeeClass, int salary) 
: myName(name), myId(id), myEmployeeClass(employeeClass), mySalary(salary) {

}

inline ostream& operator<<(ostream& j, const Employee& e) {
    e.Display(j);
    return j;
}

#endif