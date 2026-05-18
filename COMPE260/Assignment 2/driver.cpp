#include "Staque.h"
#include <iostream>
using namespace std;

int main() {
    Staque test;
    
    cout << "input series 1: (1,3,2,4,6,8,9)" << endl;
    test.Insert(1);
    test.Insert(3);
    test.Insert(2);
    test.Insert(4);
    test.Insert(6);
    test.Insert(8);
    test.Insert(9);
    cout << test;
    
    cout << "removal series 1a: (2 evens, 1 odd)" << endl;
    test.DeleteEven();
    test.DeleteEven();
    test.DeleteOdd();
    cout << test;
    
    cout << "removal series 1b: (3 odds, 3 evens)" << endl;
    test.DeleteOdd().DeleteOdd().DeleteOdd();
    test.DeleteEven().DeleteEven().DeleteEven();
    cout << test;
    

    cout << "input series 2: (0,1,2,3,4,5,6,7)" << endl;
    for (int i = 0; i < 7; ++i) {
        test.Insert(i);
    }
    cout << test;

    cout << "removal series 2a: (3 odds, 3 evens)" << endl;
    test.DeleteOdd().DeleteOdd().DeleteOdd();
    test.DeleteEven().DeleteEven().DeleteEven();
    cout << test;

    cout << "removal series 2b: (3 evens, 3 odds)" << endl;
    test.DeleteOdd().DeleteOdd().DeleteOdd();
    test.DeleteEven().DeleteEven().DeleteEven();
    cout << test;


    cout << "input series 3: (4,3,2,1,0,-1,-2,-3)" << endl;
    for (int i = 4; i > -4; --i) {
        test.Insert(i);
    }
    cout << test;
    cout << "removal series 3: (clear function)" << endl;
    test.Clear();
    cout << endl;


    cout << "input series 4: (2,2,2,2,2)" << endl;
    test.Insert(2).Insert(2).Insert(2).Insert(2).Insert(2);
    cout << test;
    
    cout << "removal series 4a: (1 odd, 3 evens)" << endl;
    test.DeleteOdd().DeleteEven().DeleteEven().DeleteEven();
    cout << test;

    cout << "removal series 4b: (3 evens)" << endl;
    test.DeleteEven().DeleteEven().DeleteEven();
    cout << test;

    return 0;
}
