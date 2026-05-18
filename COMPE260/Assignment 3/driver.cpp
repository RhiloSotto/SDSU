#include <iostream>
using namespace std;

#include "Employee.h"
#include "Owner.h"
#include "Chef.h"
#include "Waiter.h"

/* CALCULATES SALARY FOR ALL EMPLOYEE CLASSES */
void CalculateSalary(Employee* database[]) {
    int profit = 0;
    int tip = 0;

    cout << "This month's profit? " << endl;
    // profit can be negative, salary taken away from owner and chefs
    cin >> profit;

    for (size_t i = 0; i < 6; ++i) {
        if (database[i]->getEmployeeClass() == 'W') {
            cout << "Tips earned by " << database[i]->getName() << " this month?" << endl;
            cin >> tip;
            database[i]->setTips(tip);
                
        }
        database[i]->calculateSalary(profit);
    }
}

/* PRINTS ALL EMPLOYEE */
void PrintEmployeeStats(const Employee* database[]) {
    cout << "[EMPLOYEE STATISTICS]" << endl;
    for (size_t i = 0; i < 6; ++i) {
        cout << "# " << i + 1 << " " << "---------------------------------" << endl;
        cout << *database[i];
    }
    cout << "    ---------------------------------" << endl;
}

/* calls derived classes editInfo functions */
void EditEmployeeInfo(Employee* database[]) {
    size_t employeeIndex = 0;
    cout << "Edit which employee #?" << endl;
    // employee number is printed in employee stats
    cin >> employeeIndex;
    if (employeeIndex < 1 || employeeIndex > 6) {
        cout << "invalid employee #" << endl;
    }
    else {
        database[employeeIndex - 1]->editInfo();
    }
}

int main() {    

    /* INITIALIZING DATABASE */
    Employee* employeeDatabase[6];
    
    employeeDatabase[0] = new Owner("Owner1", 12, 'O', 15000);
    employeeDatabase[1] = new Chef("Chef1", 29, 'C', 10000, "French");
    employeeDatabase[2] = new Chef("Chef2", 52, 'C', 10000, "Spanish");
    employeeDatabase[3] = new Waiter("Waiter1", 100, 'W', 3000, 0, 2);
    employeeDatabase[4] = new Waiter("Waiter2", 32, 'W', 3000, 100, 3);
    employeeDatabase[5] = new Waiter("Waiter3", 75, 'W', 3000, 0, 0);
    // due to how these class constructors are implemented, 
    // only name, id, expertise, and years of service parameters
    // can be initialized to different values at this stage
    // these data members are editable through user input 
    // employee class is unalterable and salary is changed
    // through the CalculateSalary subroutine in this file

    /* MAIN PROGRAM LOOP */
    char option = '0';    
    while (option != 'q') {
        /* MENU DISPLAY */
        cout << "EMPLOYEE DATABASE" << endl
        << "[OPTIONS]" << endl
        << "w - input profit and tips earned in $ for the month" << endl
        << "s - show employee statistics for this month" << endl
        << "e - edit employee information" << endl
        << "q - quit program" << endl;

        cin >> option;
        switch (option) {
            case 'w':
                CalculateSalary(employeeDatabase);
                // definition at top of file
                break;
            
            case 's':
                PrintEmployeeStats(employeeDatabase);
                // definition at top of file
                break;

            case 'e' :
                EditEmployeeInfo(employeeDatabase);
                // definition at top of file
                break;
            
            case 'q':
                cout << "[PROGRAM TERMINATING]" << endl;
                // ends loop
                break;
            
            default:
                cout << "Not a valid option" << endl;
                break;
        }
    }

    return 0;
}